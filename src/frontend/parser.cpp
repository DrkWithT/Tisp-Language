/**
 * @file parser.cpp
 * @author DrkWithT
 * @brief Implements parser.
 * @date 2024-04-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <utility>
#include <iostream>
#include "frontend/parser.hpp"

namespace tisp::frontend
{
    /* Program impl. */

    Program::Program(std::string name_str)
    : name {name_str}, stmts {} {}

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

    void Parser::pushError(ParseErrorInfo error)
    {
        errors.push(error);
    }

    void Parser::popError()
    {
        errors.pop();
    }

    void Parser::processError(const ParseErrorInfo& error_dump, std::string_view msg)
    {
        std::ostringstream sout {};
        
        sout << error_dump << msg;

        std::cerr << sout.str() << '\n';
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
        for (const auto& option : types)
        {
            if (peekCurrent().type == option)
                return true;
        }

        return false;
    }

    Token Parser::advanceToNext() noexcept
    {
        Token temp;

        do
        {
            temp = lexer.lexNext();

            if (temp.type == TokenType::unknown)
            {
                pushError(ParseErrorInfo {temp, construct, "Unknown token."});
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
            pushError(ParseErrorInfo {peekCurrent(), construct, "Unexpected token."});
            return;
        }

        previous = current;
        current = advanceToNext();
    }

    // std::unique_ptr<Expr> parseLiteral();
    // std::unique_ptr<Expr> parseUnary();
    // std::unique_ptr<Expr> parseFactor();
    // std::unique_ptr<Expr> parseTerm();
    // std::unique_ptr<Expr> parseCompare();
    // std::unique_ptr<Expr> parseConditional();
    // std::unique_ptr<Expr> parseExpr();

    // std::unique_ptr<Stmt> parseVariable();
    // std::unique_ptr<Stmt> parseMutation();
    // std::unique_ptr<Stmt> parseDefun();
    // std::unique_ptr<Stmt> parseParam();
    // std::unique_ptr<Stmt> parseBlock();
    // std::unique_ptr<Stmt> parseInner();
    // std::unique_ptr<Stmt> parseMatch();
    // std::unique_ptr<Stmt> parseCase();
    // std::unique_ptr<Stmt> parseReturn();
    // std::unique_ptr<Stmt> parseWhile();
    // std::unique_ptr<Stmt> parseOuter();
    // std::unique_ptr<Stmt> parseGeneric();
    // std::unique_ptr<Stmt> parseSubstitution();
    // std::unique_ptr<Stmt> parseImport();

    /* Parser public impl. */

    // Parser(std::string_view source_view);

    // [[nodiscard]] Program parseAll();
}
