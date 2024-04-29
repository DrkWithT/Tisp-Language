#ifndef PARSER_HPP
#define PARSER_HPP

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include "frontend/token.hpp"
#include "frontend/lexer.hpp"
#include "frontend/info.hpp"
#include "ast/exprs.hpp"
#include "ast/stmts.hpp"

namespace tisp::frontend
{
    using Expr = tisp::ast::IExpression;
    using Stmt = tisp::ast::IStatement;
    using TispDataType = tisp::ast::DataType;
    using TispFullType = tisp::ast::FullDataType;
    using TispSeq = tisp::ast::Sequence;

    class Program
    {
    private:
        std::vector<std::unique_ptr<Stmt>> stmts;
        std::string name;

    public:
        Program(std::string name_str, std::vector<std::unique_ptr<Stmt>> stmts_arg);

        const std::string& getName() const noexcept;
        const std::vector<std::unique_ptr<Stmt>>& getStatements() const noexcept;

        void addTopStatement(std::unique_ptr<Stmt> stmt);
    };

    class Parser
    {
    private:
        Lexer lexer;
        std::string_view source;
        Token previous;
        Token current;
        SyntaxConstruct construct;
        int error_count;

        enum class TokenChoice
        {
            prev,
            curr
        };

        void logMessage(const std::runtime_error& error);

        [[nodiscard]] const Token& peekPrevious() const noexcept;
        [[nodiscard]] const Token& peekCurrent() const noexcept;
        [[nodiscard]] bool isAtEOS() const noexcept;
        [[nodiscard]] bool matchCurrent(std::initializer_list<TokenType> types) const noexcept;
        [[nodiscard]] bool matchLexeme(std::string_view lexeme_arg, TokenChoice choice) const noexcept;
        [[nodiscard]] Token advanceToNext() noexcept;
        void consumeToken(std::initializer_list<TokenType> types);

        std::unique_ptr<Expr> parseSeq(TispFullType complete_type);
        std::unique_ptr<Expr> parseLiteral(TispFullType complete_type);
        std::unique_ptr<Expr> parseUnary();
        std::unique_ptr<Expr> parseFactor();
        std::unique_ptr<Expr> parseTerm();
        std::unique_ptr<Expr> parseCompare();
        std::unique_ptr<Expr> parseConditional();
        std::unique_ptr<Expr> parseExpr();

        [[nodiscard]] TispDataType matchTypenameTokenEnum(TokenChoice choice);
        [[nodiscard]] TispFullType parseTypename();
        std::unique_ptr<Stmt> parseVariable();
        std::unique_ptr<Stmt> parseMutation();
        std::unique_ptr<Stmt> parseDefun();
        std::unique_ptr<Stmt> parseParam();
        std::unique_ptr<Stmt> parseBlock();
        std::unique_ptr<Stmt> parseInner();
        std::unique_ptr<Stmt> parseMatch();
        std::unique_ptr<Stmt> parseCase();
        std::unique_ptr<Stmt> parseDefault();
        std::unique_ptr<Stmt> parseReturn();
        std::unique_ptr<Stmt> parseWhile();
        std::unique_ptr<Stmt> parseOuter();
        std::unique_ptr<Stmt> parseGeneric();
        std::unique_ptr<Stmt> parseSubstitution();
        std::unique_ptr<Stmt> parseImport();

    public:
        Parser(std::string_view source_view);

        [[nodiscard]] Program parseAll(std::string prgm_name);
    };
}

#endif