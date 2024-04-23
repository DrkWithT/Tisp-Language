#ifndef EXPRS_HPP
#define EXPRS_HPP

#include <memory>
#include <string>
#include <vector>
#include "ast/exprbase.hpp"

namespace tisp::ast
{
    enum class OpType
    {
        invoke,
        access,
        plus,
        minus,
        times,
        slash,
        equality,
        inequality,
        greater,
        atmost,
        lesser,
        atleast,
        logic_and,
        logic_or
    };

    class Literal;

    class Unary;
    class Binary;
}

#endif