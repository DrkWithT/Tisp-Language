#ifndef PARSER_HPP
#define PARSER_HPP

#include <initializer_list>
#include <memory>
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

    class Program
    {
    private:
        std::vector<std::unique_ptr<Stmt>> stmts;
        std::string name;

    public:
        Program(std::string name_str);

        const std::string& getName() const noexcept;
        const std::vector<std::unique_ptr<Stmt>>& getStatements() const noexcept;

        void addTopStatement(std::unique_ptr<Stmt> stmt);
    };

    class Parser
    {
    private:
        Lexer lexer;
        std::stack<ParseErrorInfo> errors;
        std::string_view source;
        Token previous;
        Token current;
        SyntaxConstruct construct;

        void pushError(ParseErrorInfo error);
        void popError();
        void processError(const ParseErrorInfo& error_dump, std::string_view msg);

        [[nodiscard]] const Token& peekPrevious() const noexcept;
        [[nodiscard]] const Token& peekCurrent() const noexcept;
        [[nodiscard]] bool isAtEOS() const noexcept;
        [[nodiscard]] bool matchCurrent(std::initializer_list<TokenType> types) const noexcept;
        [[nodiscard]] Token advanceToNext() noexcept;
        void consumeToken(std::initializer_list<TokenType> types);

        std::unique_ptr<Expr> parseLiteral();
        std::unique_ptr<Expr> parseUnary();
        std::unique_ptr<Expr> parseFactor();
        std::unique_ptr<Expr> parseTerm();
        std::unique_ptr<Expr> parseCompare();
        std::unique_ptr<Expr> parseConditional();
        std::unique_ptr<Expr> parseExpr();

        std::unique_ptr<Stmt> parseVariable();
        std::unique_ptr<Stmt> parseMutation();
        std::unique_ptr<Stmt> parseDefun();
        std::unique_ptr<Stmt> parseParam();
        std::unique_ptr<Stmt> parseBlock();
        std::unique_ptr<Stmt> parseInner();
        std::unique_ptr<Stmt> parseMatch();
        std::unique_ptr<Stmt> parseCase();
        std::unique_ptr<Stmt> parseReturn();
        std::unique_ptr<Stmt> parseWhile();
        std::unique_ptr<Stmt> parseOuter();
        std::unique_ptr<Stmt> parseGeneric();
        std::unique_ptr<Stmt> parseSubstitution();
        std::unique_ptr<Stmt> parseImport();

    public:
        Parser(std::string_view source_view);

        [[nodiscard]] Program parseAll();
    };
}

#endif