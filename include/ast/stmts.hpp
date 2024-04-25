#ifndef STMTS_HPP
#define STMTS_HPP

#include <memory>
#include <string>
#include <vector>
#include "ast/exprs.hpp"
#include "ast/stmtbase.hpp"

namespace tisp::ast
{
    class Variable : public IStatement
    {
    private:
        std::string name;
        std::unique_ptr<IExpression> rv;
        DataType type;
        bool is_mutable;

    public:
        Variable() = delete;
        Variable(std::string name_arg, std::unique_ptr<IExpression> rv_arg, DataType type_arg, bool is_var);

        const std::string& getName() const noexcept;
        [[nodiscard]] DataType getDataType() const noexcept;
        [[nodiscard]] bool isMutable() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Mutation : public IStatement
    {
    private:
        std::string name;
        std::unique_ptr<IExpression> rv;

    public:
        Mutation() = delete;
        Mutation(std::string name_arg, std::unique_ptr<IExpression> rv_arg);

        const std::string& getName() const noexcept;
        const std::unique_ptr<IExpression>& getExpression() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Function : public IStatement
    {
    private:
        std::string name;
        std::vector<std::unique_ptr<IStatement>> params;
        std::unique_ptr<IStatement> body;
        DataType type;

    public:
        Function() = delete;
        Function(std::string name_arg, std::vector<std::unique_ptr<IStatement>> params_arg, std::unique_ptr<IStatement> body_arg, DataType type_arg);

        const std::string& getName() const noexcept;
        const std::vector<std::unique_ptr<IStatement>>& getParams() const noexcept;
        const std::unique_ptr<IStatement>& getBody() const noexcept;
        [[nodiscard]] DataType getDataType() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Parameter : public IStatement
    {
    private:
        std::string name;
        DataType type;

    public:
        Parameter() = delete;
        Parameter(std::string name_arg, DataType type_arg);

        const std::string& getName() const noexcept;
        [[nodiscard]] DataType getDataType() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Block : public IStatement
    {
    private:
        std::vector<std::unique_ptr<IStatement>> stmts;

    public:
        Block() = delete;
        Block(std::vector<std::unique_ptr<IStatement>> stmts_arg);

        const std::vector<std::unique_ptr<IStatement>>& getStatements() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Match : public IStatement
    {
    private:
        std::string input_name;
        std::vector<std::unique_ptr<IStatement>> cases;
        std::unique_ptr<IStatement> fallback;

    public:
        Match() = delete;
        Match(std::string input_arg, std::vector<std::unique_ptr<IStatement>> cases_arg, std::unique_ptr<IStatement> fallback_arg);

        const std::string& getName() const noexcept;
        const std::vector<std::unique_ptr<IStatement>>& getCases() const noexcept;
        const std::unique_ptr<IStatement>& getFallback() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Case : public IStatement
    {
    private:
        std::unique_ptr<IExpression> condition;
        std::unique_ptr<IStatement> body;

    public:
        Case() = delete;
        Case(std::unique_ptr<IExpression> condition_arg, std::unique_ptr<IStatement> body_arg);

        const std::unique_ptr<IExpression>& getCondition() const noexcept;
        const std::unique_ptr<IStatement>& getBody() const noexcept; 

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Return : public IStatement
    {
    private:
        std::unique_ptr<IExpression> result;

    public:
        Return() = delete;
        Return(std::unique_ptr<IExpression> result_arg);

        const std::unique_ptr<IExpression>& getResult() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class While : public IStatement
    {
    private:
        std::unique_ptr<IExpression> conditions;
        std::unique_ptr<IStatement> body;

    public:
        While() = delete;
        While(std::unique_ptr<IExpression> conditions_arg, std::unique_ptr<IStatement> body_arg);

        const std::unique_ptr<IExpression>& getConditions() const noexcept;
        const std::unique_ptr<IStatement>& getBody() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Generic : public IStatement
    {
    private:
        std::vector<std::string> params;
        std::unique_ptr<IStatement> item;

    public:
        Generic() = delete;
        Generic(std::vector<std::string> params_arg, std::unique_ptr<IStatement> item_arg);

        const std::vector<std::string>& getParams() const noexcept;
        const std::unique_ptr<IStatement>& getItem() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Substitution : public IStatement
    {
    private:
        std::string name;
        std::vector<std::string> type_names;

    public:
        Substitution() = delete;
        Substitution(std::string name_arg, std::vector<std::string> tname_args);

        const std::string& getName() const noexcept;
        const std::vector<std::string>& getTypeNames() const noexcept;

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };

    class Import : public IStatement
    {
    private:
        std::vector<std::string> item_path;
    
    public:
        Import() = delete;
        Import(std::vector<std::string> item_path_arg);

        [[nodiscard]] std::any acceptVisitor(IStmtVisitor<std::any>& visitor) const override;
    };
}

#endif