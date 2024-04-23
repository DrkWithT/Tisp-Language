#ifndef STMTBASE_HPP
#define STMTBASE_HPP

#include <any>
#include "ast/stmtvisitor.hpp"

namespace tisp::ast
{
    class IStatement
    {
    public:
        virtual ~IStatement() = default;

        std::any virtual acceptVisitor(IStmtVisitor<std::any>& visitor) const = 0;
    };
}

#endif