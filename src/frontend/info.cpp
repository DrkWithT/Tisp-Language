/**
 * @file info.cpp
 * @author DrkWithT
 * @brief Implements parser info.
 * @date 2024-04-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "frontend/info.hpp"
#include <sstream>

namespace tisp::frontend
{
    constexpr int syntax_construct_count = static_cast<int>(SyntaxConstruct::sc_unknown) + 1;

    constexpr std::array<std::string_view, syntax_construct_count> sc_names {
        "Literal",
        "Unary",
        "Factor",
        "Term",
        "Comparison",
        "Conditional",
        "Variable",
        "Mutation",
        "Defun",
        "Parameter",
        "Block",
        "Inner",
        "Match",
        "Case",
        "Return",
        "While",
        "Outer",
        "Generic",
        "Substitution",
        "Import",
        "Anything"
    };

    ParseErrorInfo::ParseErrorInfo(Token suspect_arg, SyntaxConstruct where_arg, std::string_view msg_sv)
    : suspect {suspect_arg}, where {where_arg}, msg {msg_sv} {}

    std::ostream& operator<<(std::ostream& os, const ParseErrorInfo& parse_dump) noexcept
    {
        os << "ParseError [line " << parse_dump.suspect.line << ", " << fetchSyntaxConstructName<syntax_construct_count>(sc_names, parse_dump.where) << "]:\n" << parse_dump.msg;

        return os;
    }

    std::string stringifyParseError(const Token& suspect_arg, SyntaxConstruct where_arg, std::string_view msg_arg)
    {
        std::ostringstream sout {};

        sout << "ParseError [line " << suspect_arg.line << ", " << fetchSyntaxConstructName<syntax_construct_count>(sc_names, where_arg) << "]:\n" << msg_arg;

        return sout.str();
    }

    std::string stringifyParseError(const ParseErrorInfo& error) noexcept
    {
        std::ostringstream sout {};

        sout << error << '\n';

        return sout.str();
    }
}
