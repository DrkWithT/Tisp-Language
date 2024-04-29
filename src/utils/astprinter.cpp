/**
 * @file astprinter.cpp
 * @author DrkWithT
 * @brief Implements AST printing utility.
 * @date 2024-04-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <any>
#include <iostream>
#include "utils/astprinter.hpp"

namespace tisp::utils
{
    std::any ASTPrinter::visitAnyStmt(const std::unique_ptr<tisp::ast::IStatement>& stmt)
    {
        return stmt->acceptVisitor(*this);
    }

    std::any ASTPrinter::visitVariable(const Variable &node)
    {
        std::cout << "Variable " << node.getName() << '\n';
        return {};
    }

    std::any ASTPrinter::visitMutation(const Mutation &node)
    {
        std::cout << "Mutation of " << node.getName() << '\n';
        return {};
    }

    std::any ASTPrinter::visitFunction(const Function &node)
    {
        std::cout << "Function " << node.getName() << '\n';

        node.getBody()->acceptVisitor(*this);

        return {};
    }

    std::any ASTPrinter::visitParameter(const Parameter &node)
    {
        std::cout << "Parameter " << node.getName() << '\n';
        return {};
    }

    std::any ASTPrinter::visitBlock(const Block& node)
    {
        std::cout << "Block\n";
        
        for (const auto& item : node.getStatements())
            item->acceptVisitor(*this);

        return {};
    }

    std::any ASTPrinter::visitMatch(const Match &node)
    {
        std::cout << "Match\n";

        for (const auto& item : node.getCases())
            item->acceptVisitor(*this);

        return {};
    }

    std::any ASTPrinter::visitCase(const Case &node)
    {
        std::cout << "Case\n";
        node.getBody()->acceptVisitor(*this);

        return {};
    }

    std::any ASTPrinter::visitDefault(const Default &node)
    {
        std::cout << "Default\n";
        node.getBody()->acceptVisitor(*this);

        return {};
    }

    std::any ASTPrinter::visitReturn(const Return &node)
    {
        std::cout << "Return expr " << node.getResult().get() << '\n';
        return {};
    }

    std::any ASTPrinter::visitWhile(const While &node)
    {
        std::cout << "While\n";
        node.getBody()->acceptVisitor(*this);

        return {};
    }

    std::any ASTPrinter::visitGeneric(const Generic &node)
    {
        std::cout << "Generic\n";
        
        for (const auto& param : node.getParams())
            std::cout << "Param-name: " << param << '\n';

        return {};
    }

    std::any ASTPrinter::visitSubstitution(const Substitution &node)
    {
        std::cout << "Substitution " << node.getName() << '\n';
        return {};
    }

    std::any ASTPrinter::visitImport(const Import &node)
    {
        std::cout << "Import\n";

        for (const auto& path_item : node.getItems())
            std::cout << "path_item " << path_item << ' ';

        std::cout << '\n';

        return {};
    }
}
