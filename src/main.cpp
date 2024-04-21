/**
 * @file main.cpp
 * @author DrkWithT
 * @brief Implements main interpreter code.
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
#include "frontend/token.hpp"
#include "frontend/lexer.hpp"

using MyToken = tisp::frontend::Token;
using MyLexType = tisp::frontend::TokenType;
using MyLexer = tisp::frontend::Lexer;

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
    os << "begin=" << token.begin << ", length = " << token.length << ", type = " << static_cast<int>(token.type) << '\n';

    return os;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "usage: ./tipsi [--version | help] <file>\n";
        return 1;
    }

    std::string source_path {argv[1]};
    std::string blob = readFile(source_path);

    MyLexer lexer {};
    auto tokens = lexer.tokenizeSource(blob);

    for (const auto& tk : tokens)
        std::cout << tk;

    // todo: add parsing and then VM runner.
}
