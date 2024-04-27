#ifndef INFO_HPP
#define INFO_HPP

#include <array>
#include <string_view>
#include <sstream>
#include "frontend/token.hpp"

namespace tisp::frontend
{
    enum class SyntaxConstruct
    {
        ex_literal,
        ex_unary,
        ex_factor,
        ex_term,
        ex_compare,
        ex_conditional,
        st_variable,
        st_mutation,
        st_defun,
        st_param,
        st_block,
        st_inner,
        st_match,
        st_case,
        st_return,
        st_while,
        st_outer,
        st_generic,
        st_substitution,
        st_import,
        sc_unknown
    };

    template <size_t Count>
    [[nodiscard]] std::string_view fetchSyntaxConstructName(const std::array<std::string_view, Count>& names, SyntaxConstruct where_code)
    {
        int pos = static_cast<int>(where_code);

        return std::string_view {names.at(pos)};
    }

    struct ParseErrorInfo
    {
        Token suspect;          // the possibly offending token
        SyntaxConstruct where;  // where the token is syntactically
        std::string_view msg;   // contextual message 

        ParseErrorInfo(Token suspect_arg, SyntaxConstruct where_arg, std::string_view msg_sv);

        friend std::ostream& operator<<(std::ostream& os, const ParseErrorInfo& parse_dump) noexcept;
    };
}

#endif