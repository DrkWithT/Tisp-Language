/**
 * @file stmts.cpp
 * @author DrkWithT
 * @brief Implements AST statements.
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include "ast/stmts.hpp"

namespace tisp::ast
{
    /* Variable */

    Variable::Variable(std::string name_arg, std::unique_ptr<IExpression> rv_arg, FullDataType type_arg, bool is_var)
    : name(std::move(name_arg)), rv(std::move(rv_arg)), type {type_arg}, is_mutable {is_var} {}

    const std::string& Variable::getName() const noexcept
    {
        return name;
    }

    FullDataType Variable::getDataType() const noexcept
    {
        return type;
    }

    bool Variable::isMutable() const noexcept
    {
        return is_mutable;
    }

    std::any Variable::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitVariable(*this);
    }

    /* Mutation */

    Mutation::Mutation(std::string name_arg, std::unique_ptr<IExpression> rv_arg)
    : name(std::move(name_arg)), rv(std::move(rv_arg)) {}

    const std::string& Mutation::getName() const noexcept
    {
        return name;
    }

    const std::unique_ptr<IExpression>& Mutation::getExpression() const noexcept
    {
        return rv;
    }

    std::any Mutation::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitMutation(*this);
    }

    /* Function */

    Function::Function(std::string name_arg, std::vector<std::unique_ptr<IStatement>> params_arg, std::unique_ptr<IStatement> body_arg, FullDataType type_arg)
    : name(std::move(name_arg)), params(std::move(params_arg)), body(std::move(body_arg)), type {type_arg} {}

    const std::string& Function::getName() const noexcept
    {
        return name;
    }

    const std::vector<std::unique_ptr<IStatement>>& Function::getParams() const noexcept
    {
        return params;
    }

    const std::unique_ptr<IStatement>& Function::getBody() const noexcept
    {
        return body;
    }

    const FullDataType& Function::getFullDataType() const noexcept
    {
        return type;
    }

    [[nodiscard]] std::any Function::acceptVisitor(IStmtVisitor<std::any> &visitor) const
    {
        return visitor.visitFunction(*this);
    }

    /* Parameter */

    Parameter::Parameter(std::string name_arg, FullDataType type_arg)
    : name(std::move(name_arg)), type {type_arg} {}

    const std::string& Parameter::getName() const noexcept
    {
        return name;
    }

    const FullDataType& Parameter::getFullDataType() const noexcept
    {
        return type;
    }

    std::any Parameter::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitParameter(*this);
    }

    /* Block */

    Block::Block(std::vector<std::unique_ptr<IStatement>> stmts_arg)
    : stmts(std::move(stmts_arg)) {}

    const std::vector<std::unique_ptr<IStatement>>& Block::getStatements() const noexcept
    {
        return stmts;
    }

    std::any Block::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitBlock(*this);
    }

    /* Match */

    Match::Match(std::vector<std::unique_ptr<IStatement>> cases_arg, std::unique_ptr<IStatement> fallback_arg)
    : cases(std::move(cases_arg)), fallback(std::move(fallback_arg)) {}

    const std::vector<std::unique_ptr<IStatement>>& Match::getCases() const noexcept
    {
        return cases;
    }

    const std::unique_ptr<IStatement>& Match::getFallback() const noexcept
    {
        return fallback;
    }

    std::any Match::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitMatch(*this);
    }

    /* Case */

    Case::Case(std::unique_ptr<IExpression> condition_arg, std::unique_ptr<IStatement> body_arg)
    : condition(std::move(condition_arg)), body(std::move(body_arg)) {}

    const std::unique_ptr<IExpression>& Case::getCondition() const noexcept
    {
        return condition;
    }

    const std::unique_ptr<IStatement>& Case::getBody() const noexcept
    {
        return body;
    }

    std::any Case::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitCase(*this);
    }

    /* Default */
    Default::Default(std::unique_ptr<IStatement> body_arg)
    : body(std::move(body_arg)) {}

    const std::unique_ptr<IStatement>& Default::getBody() const noexcept
    {
        return body;
    }

    std::any Default::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitDefault(*this);
    }

    /* Return */

    Return::Return(std::unique_ptr<IExpression> result_arg)
    : result(std::move(result_arg)) {}

    const std::unique_ptr<IExpression>& Return::getResult() const noexcept
    {
        return result;
    }

    std::any Return::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitReturn(*this);
    }

    /* While */

    While::While(std::unique_ptr<IExpression> conditions_arg, std::unique_ptr<IStatement> body_arg)
    : conditions(std::move(conditions_arg)), body(std::move(body_arg)) {}

    const std::unique_ptr<IExpression>& While::getConditions() const noexcept
    {
        return conditions;
    }

    const std::unique_ptr<IStatement>& While::getBody() const noexcept
    {
        return body;
    }

    std::any While::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitWhile(*this);
    }

    /* Generic */

    Generic::Generic(std::vector<std::string> params_arg, std::unique_ptr<IStatement> item_arg)
    : params(std::move(params_arg)), item(std::move(item_arg)) {}

    const std::vector<std::string>& Generic::getParams() const noexcept
    {
        return params;
    }

    const std::unique_ptr<IStatement>& Generic::getItem() const noexcept
    {
        return item;
    }

    std::any Generic::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitGeneric(*this);
    }

    /* Substitution */

    Substitution::Substitution(std::string name_arg, std::vector<std::string> tname_args)
    : name(std::move(name_arg)), type_names(std::move(tname_args)) {}

    const std::string& Substitution::getName() const noexcept
    {
        return name;
    }

    const std::vector<std::string>& Substitution::getTypeNames() const noexcept
    {
        return type_names;
    }

    std::any Substitution::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitSubstitution(*this);
    }

    /* Import */

    Import::Import(std::vector<std::string> item_path_arg)
    : item_path(std::move(item_path_arg)) {}

    const std::vector<std::string>& Import::getItems() const
    {
        return item_path;
    }

    std::any Import::acceptVisitor(IStmtVisitor<std::any>& visitor) const
    {
        return visitor.visitImport(*this);
    }
}
