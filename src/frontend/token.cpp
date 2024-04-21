/**
 * @file token.cpp
 * @author DrkWithT
 * @brief Implements Token helpers.
 * @date 2024-04-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <sstream>
#include "frontend/token.hpp"

namespace tisp::frontend
{
    [[nodiscard]] std::string_view viewLexeme(const Token& token, std::string_view source)
    {
        return source.substr(token.begin, token.length);
    }

    [[nodiscard]] std::string getLexeme(const Token& token, const std::string_view source)
    {
        auto slice_view = viewLexeme(token, source);
        std::ostringstream sout {};

        for (const char c : slice_view)
            sout << c;

        return sout.str();
    }
}

