/**
 * @file parser.cpp
 * @author DrkWithT
 * @brief Implements parser.
 * @date 2024-04-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <memory>
#include <stdexcept>
#include <utility>
#include <iostream>
#include "frontend/info.hpp"
#include "frontend/parser.hpp"
#include "frontend/token.hpp"

namespace tisp::frontend
{
    using TispOp = tisp::ast::OpType;
    using TispLiteral = tisp::ast::Literal;
    using TispUnary = tisp::ast::Unary;
    using TispBinary = tisp::ast::Binary;
    using TispVariable = tisp::ast::Variable;
    using TispMutation = tisp::ast::Mutation;
    using TispDefun = tisp::ast::Function;
    using TispParam = tisp::ast::Parameter;
    using TispBlock = tisp::ast::Block;
    using TispMatch = tisp::ast::Match;
    using TispCase = tisp::ast::Case;
    using TispReturn = tisp::ast::Return;
    using TispWhile = tisp::ast::While;
    using TispGeneric = tisp::ast::Generic;
    using TispGenSubst = tisp::ast::Substitution;
    using TispImport = tisp::ast::Import;

    /* Program impl. */

    Program::Program(std::string name_str, std::vector<std::unique_ptr<Stmt>> stmts_arg)
    : stmts(std::move(stmts_arg)), name {name_str} {}

    const std::string& Program::getName() const noexcept
    {
        return name;
    }

    const std::vector<std::unique_ptr<Stmt>>& Program::getStatements() const noexcept
    {
        return stmts;
    }

    void Program::addTopStatement(std::unique_ptr<Stmt> stmt)
    {
        stmts.emplace_back(std::move(stmt));
    }

    /* Parser private impl. */

    void Parser::logMessage(const std::runtime_error& error)
    {
        std::cerr << error.what() << '\n';
    }

    const Token& Parser::peekPrevious() const noexcept
    {
        return previous;
    }

    const Token& Parser::peekCurrent() const noexcept
    {
        return current;
    }

    bool Parser::isAtEOS() const noexcept
    {
        return current.type == TokenType::eof;
    }

    bool Parser::matchCurrent(std::initializer_list<TokenType> types) const noexcept
    {
        /// Vacuous case: no match candidates means a free token passing.
        if (types.size() == 0)
            return true;

        for (const auto& option : types)
        {
            if (peekCurrent().type == option)
                return true;
        }

        return false;
    }

    bool Parser::matchLexeme(std::string_view lexeme_arg, Parser::TokenChoice choice) const noexcept
    {
        if (choice == Parser::TokenChoice::curr)
            return viewLexeme(peekCurrent(), source) == lexeme_arg;

        return viewLexeme(peekPrevious(), source) == lexeme_arg;
    }

    Token Parser::advanceToNext() noexcept
    {
        Token temp;

        do
        {
            temp = lexer.lexNext();

            if (temp.type == TokenType::unknown)
            {
                ParseErrorInfo error {peekCurrent(), construct, "Unknown token."};
                error_count++;

                std::cerr << stringifyParseError(error) << '\n';

                continue;
            }

            if (temp.type == TokenType::whitespace || temp.type == TokenType::comment)
                continue;

            break;
        } while (!isAtEOS());

        return temp;
    }

    void Parser::consumeToken(std::initializer_list<TokenType> types)
    {
        if (isAtEOS())
            return;

        if (!matchCurrent(types))
        {
            error_count++;
            throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unexpected token.")};
        }

        previous = current;
        current = advanceToNext();
    }

    std::unique_ptr<Expr> Parser::parseSeq(TispFullType complete_type)
    {
        construct = SyntaxConstruct::ex_literal;

        consumeToken({TokenType::lbrack});

        std::vector<std::unique_ptr<Expr>> x_items {};

        while (!isAtEOS())
        {
            if (matchCurrent({TokenType::rbrack}))
            {
                consumeToken({});
                break;
            }

            auto x_item = parseLiteral(complete_type);
            x_items.emplace_back(std::move(x_item));

            if (matchCurrent({TokenType::comma}))
            {
                consumeToken({});
                continue;
            }
            else if (!matchCurrent({TokenType::rbrack}))
            {
                error_count++;
                throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Expected comma or ']'.")};
            }
        }

        /// @note Maybe use std::move on the temporary arg for the sequence for this result? Dunno if unique_ptrs in the TispSeq will complain.
        return std::make_unique<TispLiteral>(TispSeq {std::move(x_items), complete_type.inner});
    }

    std::unique_ptr<Expr> Parser::parseLiteral(TispFullType complete_type)
    {
        construct = SyntaxConstruct::ex_literal;

        if (matchCurrent({TokenType::identifier}))
        {
            consumeToken({});
            return std::make_unique<TispLiteral>(peekPrevious());
        }

        if (matchCurrent({TokenType::keyword}) && (matchLexeme("true", Parser::TokenChoice::curr) || matchLexeme("false", Parser::TokenChoice::curr)))
        {
            bool flag = matchLexeme("true", Parser::TokenChoice::curr);
            consumeToken({});

            return std::make_unique<TispLiteral>(flag);
        }
        else if (matchCurrent({TokenType::keyword}) && matchLexeme("nil", Parser::TokenChoice::curr))
        {
            consumeToken({});

            return std::make_unique<TispLiteral>();
        }
        else if (matchCurrent({TokenType::num_int}))
        {
            auto x = std::stoi(getLexeme(peekCurrent(), source));
            consumeToken({});

            return std::make_unique<TispLiteral>(x);
        }
        else if (matchCurrent({TokenType::num_dbl}))
        {
            auto x = std::stod(getLexeme(peekCurrent(), source));
            consumeToken({});

            return std::make_unique<TispLiteral>(x);
        }
        else if (matchCurrent({TokenType::strbody}))
        {
            auto x = getLexeme(peekCurrent(), source);
            consumeToken({});

            return std::make_unique<TispLiteral>(x);
        }
        else if (matchCurrent({TokenType::lbrack}))
        {
            return parseSeq(complete_type);
        }

        error_count++;
        throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unknown literal.")};
    }

    std::unique_ptr<Expr> Parser::parseUnary()
    {
        construct = SyntaxConstruct::ex_unary;
        TokenType token_type = peekCurrent().type;

        if (matchCurrent({TokenType::identifier}))
        {
            return parseLiteral({.outer = TispDataType::tbd, .inner = TispDataType::tbd});
        }
        else if (!matchCurrent({TokenType::op_invoke, TokenType::op_minus, TokenType::op_access}))
        {
            error_count++;
            throw std::runtime_error {stringifyParseError( peekCurrent(), construct, "Invalid unary operator.")};
        }

        consumeToken({});

        TispOp op;

        switch (token_type)
        {
            case TokenType::op_invoke:
                op = TispOp::invoke;
                break;
            case TokenType::op_minus:
                op = TispOp::minus;
                break;
            case TokenType::op_access:
                op = TispOp::access;
                break;
            default:
                error_count++;
                throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unreachable logic hit!")};
        }

        std::vector<std::unique_ptr<Expr>> x_argv {};

        if (op == TispOp::minus)
        {
            consumeToken({TokenType::identifier});

            return std::make_unique<TispUnary>(std::move(x_argv), getLexeme(peekPrevious(), source), op);
        }

        consumeToken({TokenType::lparen});
        consumeToken({TokenType::identifier});

        auto x_identifier = getLexeme(peekPrevious(), source);

        while (!isAtEOS())
        {
            if (matchCurrent({TokenType::rparen}))
            {
                consumeToken({});
                break;
            }

            auto x_arg_expr = parseExpr();
            x_argv.emplace_back(std::move(x_arg_expr));
        }

        return std::make_unique<TispUnary>(std::move(x_argv), std::move(x_identifier), op);
    }

    std::unique_ptr<Expr> Parser::parseFactor()
    {
        construct = SyntaxConstruct::ex_factor;
        auto lhs = parseUnary();

        while (matchCurrent({TokenType::op_times, TokenType::op_slash}))
        {
            consumeToken({});

            auto factor_op = (peekPrevious().type == TokenType::op_times)
                ? TispOp::times : TispOp::slash ;

            auto rhs = parseUnary();

            lhs = std::make_unique<TispBinary>(std::move(lhs), std::move(rhs), factor_op);
        }

        return lhs;
    }

    std::unique_ptr<Expr> Parser::parseTerm()
    {
        construct = SyntaxConstruct::ex_term;
        auto lhs = parseFactor();

        while (matchCurrent({TokenType::op_plus, TokenType::op_minus}))
        {
            consumeToken({});

            auto term_op = (peekPrevious().type == TokenType::op_plus)
                ? TispOp::plus : TispOp::minus ;

            auto rhs = parseFactor();

            lhs = std::make_unique<TispBinary>(std::move(lhs), std::move(rhs), term_op);
        }

        return lhs;
    }

    std::unique_ptr<Expr> Parser::parseCompare()
    {
        construct = SyntaxConstruct::ex_compare;
        auto lhs = parseTerm();

        consumeToken({TokenType::op_equ, TokenType::op_neq, TokenType::op_gt, TokenType::op_gte, TokenType::op_lt, TokenType::op_lte});

        TispOp cmp_op;

        switch (peekPrevious().type)
        {
            case TokenType::op_equ:
                cmp_op = TispOp::equality;
                break;
            case TokenType::op_neq:
                cmp_op = TispOp::inequality;
                break;
            case TokenType::op_gt:
                cmp_op = TispOp::greater;
                break;
            case TokenType::op_gte:
                cmp_op = TispOp::atleast;
                break;
            case TokenType::op_lt:
                cmp_op = TispOp::lesser;
                break;
            case TokenType::op_lte:
            default:
                cmp_op = TispOp::atmost;
                break;
        }

        auto rhs = parseTerm();

        return std::make_unique<TispBinary>(std::move(lhs), std::move(rhs), cmp_op);
    }

    std::unique_ptr<Expr> Parser::parseConditional()
    {
        construct = SyntaxConstruct::ex_conditional;
        auto lhs = parseCompare();

        while (matchCurrent({TokenType::op_and, TokenType::op_or}))
        {
            consumeToken({});

            auto cond_op = (peekPrevious().type == TokenType::op_and)
                ? TispOp::logic_and : TispOp::logic_or ;

            auto rhs = parseCompare();

            lhs = std::make_unique<TispBinary>(std::move(lhs), std::move(rhs), cond_op);
        }

        return lhs;
    }

    std::unique_ptr<Expr> Parser::parseExpr()
    {
        return parseConditional();
    }

    TispDataType Parser::matchTypenameTokenEnum(TokenChoice choice)
    {
        if (matchLexeme("Boolean", choice))
            return TispDataType::boolean;
        else if (matchLexeme("Integer", choice))
            return TispDataType::integer;
        else if (matchLexeme("Double", choice))
            return TispDataType::ndouble;
        else if (matchLexeme("String", choice))
            return TispDataType::string;
        else if (matchLexeme("Seq", choice))
            return TispDataType::sequence;
        else if (matchLexeme("Nil", choice))
            return TispDataType::nil;
        else
            return TispDataType::unknown;
    }

    TispFullType Parser::parseTypename()
    {
        // 1. parse enclosing type as per grammar
        consumeToken({TokenType::tname, TokenType::identifier});

        TispDataType wrap_type = matchTypenameTokenEnum(Parser::TokenChoice::prev);

        // 2. parse enclosed type as per grammar
        if (wrap_type != TispDataType::sequence)
        {
            return {.outer = wrap_type, .inner = TispDataType::unknown};
        }

        consumeToken({TokenType::lparen});
        consumeToken({TokenType::tname, TokenType::identifier});

        TispDataType in_type = matchTypenameTokenEnum(Parser::TokenChoice::prev);

        consumeToken({TokenType::rparen});

        return {.outer = wrap_type, .inner = in_type};
    }

    std::unique_ptr<Stmt> Parser::parseVariable()
    {
        construct = SyntaxConstruct::st_variable;
        consumeToken({TokenType::keyword});

        auto keyword_sv = viewLexeme(peekPrevious(), source);
        bool is_mutable = keyword_sv != "const";

        if (keyword_sv != "const" && keyword_sv != "var")
            throw std::runtime_error {stringifyParseError(previous, construct, "Invalid keyword!")};
        
        auto var_name = getLexeme(peekCurrent(), source);

        consumeToken({TokenType::identifier});
        consumeToken({TokenType::colon});

        auto var_type = parseTypename();
        auto var_expr = parseExpr();

        return std::make_unique<TispVariable>(var_name, std::move(var_expr), var_type, is_mutable);
    }

    std::unique_ptr<Stmt> Parser::parseMutation()
    {
        construct = SyntaxConstruct::st_mutation;
        consumeToken({TokenType::identifier});

        auto lv_name = getLexeme(peekPrevious(), source);

        consumeToken({TokenType::op_set});

        auto rv_expr = parseExpr();

        return std::make_unique<TispMutation>(lv_name, std::move(rv_expr));
    }

    std::unique_ptr<Stmt> Parser::parseDefun()
    {
        construct = SyntaxConstruct::st_defun;
        consumeToken({});
        
        auto func_name = getLexeme(peekCurrent(), source);

        consumeToken({TokenType::lparen});

        std::vector<std::unique_ptr<Stmt>> func_params {};

        while (!isAtEOS())
        {
            if (matchCurrent({TokenType::rparen}))
            {
                consumeToken({});
                break;
            }

            auto x_param = parseParam();
            func_params.emplace_back(std::move(x_param));

            if (matchCurrent({TokenType::comma}))
            {
                consumeToken({TokenType::comma});
                continue;
            }
            else if (!matchCurrent({TokenType::rparen}))
            {
                error_count++;
                throw std::runtime_error {stringifyParseError( peekCurrent(), construct, "Unexpected symbol!")};
            }
        }

        consumeToken({TokenType::arrow});

        auto func_type = parseTypename();
        auto func_body = parseBlock();

        return std::make_unique<TispDefun>(func_name, std::move(func_params), std::move(func_body), func_type);
    }

    std::unique_ptr<Stmt> Parser::parseParam()
    {
        construct = SyntaxConstruct::st_param;
        consumeToken({TokenType::identifier});

        auto param_name = getLexeme(peekPrevious(), source);
        
        consumeToken({TokenType::colon});

        auto param_type = parseTypename();

        return std::make_unique<TispParam>(param_name, param_type);
    }

    std::unique_ptr<Stmt> Parser::parseBlock()
    {
        construct = SyntaxConstruct::st_block;
        consumeToken({TokenType::lbrace});

        std::vector<std::unique_ptr<Stmt>> items {};

        while (!matchCurrent({TokenType::rbrace}))
        {
            auto x_stmt = parseInner();
            items.emplace_back(std::move(x_stmt));
        }

        return std::make_unique<TispBlock>(std::move(items));
    }

    std::unique_ptr<Stmt> Parser::parseInner()
    {
        construct = SyntaxConstruct::st_inner;
        auto lexeme_sv = viewLexeme(peekCurrent(), source);

        if (matchCurrent({TokenType::identifier}))
            return parseMutation();

        if (lexeme_sv == "const" || lexeme_sv == "var")
            return parseVariable();
        else if (lexeme_sv == "defun")
            return parseDefun();
        else if (lexeme_sv == "match")
            return parseMatch();
        else if (lexeme_sv == "while")
            return parseWhile();
        else if (lexeme_sv == "return")
            return parseReturn();

        error_count++;
        throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Invalid statement begin!")};
    }

    std::unique_ptr<Stmt> Parser::parseMatch()
    {
        construct = SyntaxConstruct::st_match;
        consumeToken({});
        consumeToken({TokenType::lbrace});

        std::vector<std::unique_ptr<Stmt>> x_cases {};
        std::unique_ptr<Stmt> x_default = nullptr;
        bool has_case = false;
        bool has_default = false;

        while (!isAtEOS())
        {
            if (matchCurrent({TokenType::rbrace}))
            {
                consumeToken({});
                break;
            }

            if (viewLexeme(peekCurrent(), source) == "case")
            {
                auto x_case = parseCase();
                x_cases.emplace_back(std::move(x_case));
                has_case = true;
            }
            else if (has_case && !has_default && viewLexeme(peekCurrent(), source) == "default")
            {
                x_default = parseDefault();
                has_default = true;
            }
            else
            {
                error_count++;
                throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unexpected/missing case/default!")};
            }
        }

        if (!has_case || !has_default)
        {
            error_count++;
            throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unexpected/missing case/default!")};
        }

        return std::make_unique<TispMatch>(std::move(x_cases), std::move(x_default));
    }

    std::unique_ptr<Stmt> Parser::parseCase()
    {
        construct = SyntaxConstruct::st_case;
        consumeToken({});

        auto case_cond = parseExpr();
        auto case_body = parseBlock();

        return std::make_unique<TispCase>(std::move(case_cond), std::move(case_body));
    }

    std::unique_ptr<Stmt> Parser::parseReturn()
    {
        construct = SyntaxConstruct::st_return;
        consumeToken({});

        auto ret_result = parseExpr();

        return std::make_unique<TispReturn>(std::move(ret_result));
    }

    std::unique_ptr<Stmt> Parser::parseWhile()
    {
        construct = SyntaxConstruct::st_while;
        consumeToken({});
        
        auto while_cond = parseExpr();
        auto while_body = parseBlock();

        return std::make_unique<TispWhile>(std::move(while_cond), std::move(while_body));
    }

    std::unique_ptr<Stmt> Parser::parseOuter()
    {
        construct = SyntaxConstruct::st_outer;
        auto lexeme_sv = viewLexeme(peekCurrent(), source);

        if (lexeme_sv == "use")
            return parseImport();
        else if (lexeme_sv == "const" || lexeme_sv == "var")
            return parseVariable();
        else if (lexeme_sv == "defun")
            return parseDefun();
        else if (lexeme_sv == "generic")
            return parseGeneric();

        error_count++;
        throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unexpected outer statement.")};
    }

    std::unique_ptr<Stmt> Parser::parseGeneric()
    {
        construct = SyntaxConstruct::st_generic;
        consumeToken({});
        consumeToken({TokenType::lparen});

        std::vector<std::string> params {};

        while (!matchCurrent({TokenType::rparen}))
        {
            if (matchCurrent({TokenType::identifier}))
            {
                auto param_name = getLexeme(peekCurrent(), source);
                params.emplace_back(std::move(param_name));
                consumeToken({});
            }
            else
            {
                error_count++;
                throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unexpected token")};
            }
        }

        auto genified_func = parseDefun();

        return std::make_unique<TispGeneric>(std::move(params), std::move(genified_func));
    }

    std::unique_ptr<Stmt> Parser::parseSubstitution()
    {
        construct = SyntaxConstruct::st_substitution;
        auto subst_name = getLexeme(peekCurrent(), source);

        consumeToken({TokenType::identifier});
        consumeToken({TokenType::lparen});

        std::vector<std::string> subst_args {};

        while (!matchCurrent({TokenType::rparen}))
        {
            if (matchCurrent({TokenType::identifier}))
            {
                auto x_arg = getLexeme(peekCurrent(), source);
                subst_args.emplace_back(std::move(x_arg));
                consumeToken({});
            }
            else
            {
                error_count++;
                throw std::runtime_error {stringifyParseError(peekCurrent(), construct, "Unexpected token.")};
            }
        }

        return std::make_unique<TispGenSubst>(std::move(subst_name), std::move(subst_args));
    }

    std::unique_ptr<Stmt> Parser::parseImport()
    {
        construct = SyntaxConstruct::st_import;
        consumeToken({});
        
        std::vector<std::string> path_parts {};

        /// @todo Fix the logical error where dots can be spammed between import path names- yet accepted!
        while (!isAtEOS())
        {
            if (matchCurrent({TokenType::identifier}))
            {
                consumeToken({});
                auto part = getLexeme(peekCurrent(), source);
                path_parts.emplace_back(std::move(part));
            }
            else if (matchCurrent({TokenType::dot}))
            {
                consumeToken({});
                continue;
            }
            else
                break;
        }

        return std::make_unique<TispImport>(std::move(path_parts));
    }

    /* Parser public impl. */

    Parser::Parser(std::string_view source_view)
    : lexer {source_view}, source {source_view}, previous {Token {.type = TokenType::unknown}}, current {Token {.type = TokenType::unknown}}, construct {SyntaxConstruct::sc_unknown}, error_count {0}
    {
        consumeToken({});
    }

    [[nodiscard]] Program Parser::parseAll(std::string prgm_name)
    {
        std::vector<std::unique_ptr<Stmt>> outer_stmts {};

        try
        {
            while (!isAtEOS())
            {
                auto stmt = parseOuter();
                outer_stmts.emplace_back(std::move(stmt));
            }
        }
        catch (const std::runtime_error &error)
        {
            logMessage(error);
        }

        return Program {std::move(prgm_name), std::move(outer_stmts)};
    }
}
