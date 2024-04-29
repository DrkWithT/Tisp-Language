/**
 * @file main.cpp
 * @author DrkWithT
 * @brief Implements main interpreter code: args checking, running interpreter, etc.
 * @version 0.1
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include "frontend/parser.hpp"
#include "utils/astprinter.hpp"

using MyToken = tisp::frontend::Token;
using MyParser = tisp::frontend::Parser;
using MyPrinter = tisp::utils::ASTPrinter;

[[nodiscard]] std::string readFile(const std::string& file_path)
{
    std::ifstream reader {file_path.c_str()};

    if (!reader.is_open())
    {
        return std::string {""};
    }

    reader.seekg(0, std::ios::end);
    size_t file_len = reader.tellg();
    reader.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(file_len + 1);
    std::fill(buffer.get(), buffer.get() + file_len + 1, '\0');

    bool read_ok = reader.read(buffer.get(), file_len).good();

    if (read_ok)
        return std::string {buffer.get()};

    return std::string {""};
}

std::ostream& operator<<(std::ostream& os, const MyToken& token) noexcept
{
    os << "{begin=" << token.begin << ", length = " << token.length << ", type = " << static_cast<int>(token.type) << "}\n";

    return os;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "usage: ./tipsi [--version | help] <file>\n";
        return 1;
    }

    std::string arg {argv[1]};

    if (arg == "--version")
    {
        std::cout << "Tipsi (Tisp v0.0.1)\nBy: DrkWithT at GitHub\n";
        return 0;
    }
    else if (arg == "--help")
    {
        std::cout << "usage: ./tipsi [--version | help] <file>\n";
        return 0;
    }

    std::string blob = readFile(arg);
    std::string file_name = arg.substr(arg.find_last_of('/') + 1);

    /// @todo do parsing and then VM runner later on.
    MyParser parser {blob};
    auto prgm_ast = parser.parseAll(file_name);
    MyPrinter printer {};

    for (const auto& stmt : prgm_ast.getStatements())
        printer.visitAnyStmt(stmt);
}
