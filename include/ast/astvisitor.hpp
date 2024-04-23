#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

namespace tisp::ast
{
    class Literal;
    class Unary;
    class Binary;

    class Variable;
    class Mutation;
    class Function;
    class Parameter;
    class Match;
    class Case;
    class Default;
    class Return;
    class While;
    class Generic;
    class Substitution;
    class Import;

    template <typename Rt>
    class AstVisitor
    {
    public:
        virtual ~AstVisitor() = default;

        virtual Rt visitNode(const Literal &node) = 0;
        virtual Rt visitNode(const Unary &node) = 0;
        virtual Rt visitNode(const Binary &node) = 0;

        virtual Rt visitNode(const Variable &node) = 0;
        virtual Rt visitNode(const Mutation &node) = 0;
        virtual Rt visitNode(const Function &node) = 0;
        virtual Rt visitNode(const Parameter &node) = 0;
        virtual Rt visitNode(const Match &node) = 0;
        virtual Rt visitNode(const Case &node) = 0;
        virtual Rt visitNode(const Default &node) = 0;
        virtual Rt visitNode(const Return &node) = 0;
        virtual Rt visitNode(const While &node) = 0;
        virtual Rt visitNode(const Generic &node) = 0;
        virtual Rt visitNode(const Substitution &node) = 0;
        virtual Rt visitNode(const Import &node) = 0;
    };
}

#endif