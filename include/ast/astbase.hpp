#ifndef ASTBASE_HPP
#define ASTBASE_HPP

#include <any>
#include "ast/astvisitor.hpp"

namespace tisp::ast
{
    class IAstNode
    {
    public:
        virtual ~IAstNode() = default;

        std::any virtual acceptVisitor(AstVisitor<std::any>& visitor) const = 0;
    };
}

#endif