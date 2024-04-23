#ifndef EXPRBASE_HPP
#define EXPRBASE_HPP

#include <any>
#include "ast/exprvisitor.hpp"

namespace tisp::ast
{
    class IExpression
    {
    public:
        virtual ~IExpression() = default;

        std::any virtual acceptVisitor(IExprVisitor<std::any>& visitor) const = 0;
    };
}

#endif