#ifndef ASTPRINTER_HPP
#define ASTPRINTER_HPP

#include <any>
#include <memory>
/// @note Use expr visitors later!
// #include "ast/exprvisitor.hpp"
#include "ast/stmtbase.hpp"
#include "ast/stmtvisitor.hpp"
#include "ast/stmts.hpp"

namespace tisp::utils
{
    using namespace tisp::ast;
    using AnyNodeVisitor = IStmtVisitor<std::any>;

    class ASTPrinter : public AnyNodeVisitor
    {
    public:
        ASTPrinter() = default;

        [[maybe_unused]] std::any visitAnyStmt(const std::unique_ptr<tisp::ast::IStatement>& stmt);

        [[maybe_unused]] std::any visitVariable(const Variable &node) override;
        [[maybe_unused]] std::any visitMutation(const Mutation &node) override;
        [[maybe_unused]] std::any visitFunction(const Function &node) override;
        [[maybe_unused]] std::any visitParameter(const Parameter &node) override;
        [[maybe_unused]] std::any visitBlock(const Block& node) override;
        [[maybe_unused]] std::any visitMatch(const Match &node) override;
        [[maybe_unused]] std::any visitCase(const Case &node) override;
        [[maybe_unused]] std::any visitDefault(const Default &node) override;
        [[maybe_unused]] std::any visitReturn(const Return &node) override;
        [[maybe_unused]] std::any visitWhile(const While &node) override;
        [[maybe_unused]] std::any visitGeneric(const Generic &node) override;
        [[maybe_unused]] std::any visitSubstitution(const Substitution &node) override;
        [[maybe_unused]] std::any visitImport(const Import &node) override;
    };
}

#endif