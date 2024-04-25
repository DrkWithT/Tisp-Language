#ifndef STMTVISITOR_HPP
#define STMTVISITOR_HPP

namespace tisp::ast
{
    class Variable;
    class Mutation;
    class Function;
    class Parameter;
    class Block;
    class Match;
    class Case;
    class Return;
    class While;
    class Generic;
    class Substitution;
    class Import;

    template <typename Rt>
    class IStmtVisitor
    {
    public:
        virtual ~IStmtVisitor() = default;

        virtual Rt visitVariable(const Variable &node) = 0;
        virtual Rt visitMutation(const Mutation &node) = 0;
        virtual Rt visitFunction(const Function &node) = 0;
        virtual Rt visitParameter(const Parameter &node) = 0;
        virtual Rt visitBlock(const Block& node) = 0;
        virtual Rt visitMatch(const Match &node) = 0;
        virtual Rt visitCase(const Case &node) = 0;
        virtual Rt visitDefault(const Default &node) = 0;
        virtual Rt visitReturn(const Return &node) = 0;
        virtual Rt visitWhile(const While &node) = 0;
        virtual Rt visitGeneric(const Generic &node) = 0;
        virtual Rt visitSubstitution(const Substitution &node) = 0;
        virtual Rt visitImport(const Import &node) = 0;
    };
}

#endif