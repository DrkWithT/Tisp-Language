#ifndef EXPRVISITOR_HPP
#define EXPRVISITOR_HPP

namespace tisp::ast
{
    class Literal;
    class Unary;
    class Binary;

    template <typename Rt>
    class IExprVisitor
    {
    public:
        virtual ~IExprVisitor() = default;

        virtual Rt visitLiteral(const Literal &node) = 0;
        virtual Rt visitUnary(const Unary &node) = 0;
        virtual Rt visitBinary(const Binary &node) = 0;
    };
}

#endif