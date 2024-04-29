/**
 * @file exprs.cpp
 * @author DrkWithT
 * @brief Implements AST expressions.
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include "ast/exprs.hpp"

namespace tisp::ast
{
    /* Sequence aggregate */

    Sequence::Sequence()
    : items {}, homogen_type {DataType::unknown} {}

    Sequence::Sequence(std::vector<std::unique_ptr<IExpression>> item_args, DataType type)
    : items(std::move(item_args)), homogen_type {type} {}

    /* Literal */

    Literal::Literal()
    : value {Nil {}}, data_type {DataType::nil, DataType::unknown} {}

    Literal::Literal(bool b)
    : value {b}, data_type {DataType::boolean, DataType::unknown} {}

    Literal::Literal(int i)
    : value {i}, data_type {DataType::integer, DataType::unknown} {}

    Literal::Literal(double dbl)
    : value {dbl}, data_type {DataType::ndouble, DataType::unknown} {}

    Literal::Literal(std::string str)
    : value {std::move(str)}, data_type {DataType::string, DataType::unknown} {}

    Literal::Literal(Sequence seq)
    : value {std::move(seq)}, data_type {DataType::sequence, seq.homogen_type} {}

    Literal::Literal(TispToken tok)
    : value {tok}, data_type {DataType::tbd, DataType::tbd} {}

    std::any Literal::acceptVisitor(IExprVisitor<std::any>& visitor) const
    {
        return visitor.visitLiteral(*this);
    }

    /* Unary */

    Unary::Unary(std::vector<std::unique_ptr<IExpression>> argv_arg, std::string identifier_arg, OpType op_arg)
    : argv(std::move(argv_arg)), identifier(std::move(identifier_arg)), op {op_arg} {}

    std::any Unary::acceptVisitor(IExprVisitor<std::any>& visitor) const
    {
        return visitor.visitUnary(*this);
    }

    /* Binary */

    Binary::Binary(std::unique_ptr<IExpression> lhs, std::unique_ptr<IExpression> rhs, OpType op_arg)
    : left(std::move(lhs)), right(std::move(rhs)), op {op_arg} {}

    std::any Binary::acceptVisitor(IExprVisitor<std::any>& visitor) const
    {
        return visitor.visitBinary(*this);
    }
}
