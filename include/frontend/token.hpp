#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <string_view>

namespace tisp::frontend
{
    enum class TokenType
    {
        unknown,
        whitespace,
        comment,
        identifier,
        tname,
        keyword,
        num_int,
        num_dbl,
        op_invoke,
        op_access,
        op_set,
        op_plus,
        op_minus,
        op_times,
        op_slash,
        op_gt,
        op_gte,
        op_lt,
        op_lte,
        op_and,
        op_or,
        colon,
        arrow,
        lt_nil,
        lt_true,
        lt_false,
        dot,
        comma,
        strbody,
        lparen,
        rparen,
        lbrack,
        rbrack,
        lbrace,
        rbrace,
        eof
    };

    constexpr bool operator<=(TokenType lhs, TokenType rhs) noexcept
    {
        return static_cast<int>(lhs) <= static_cast<int>(rhs);
    }

    constexpr bool operator>=(TokenType lhs, TokenType rhs) noexcept
    {
        return static_cast<int>(lhs) >= static_cast<int>(rhs);
    }

    struct Token
    {
        size_t begin;
        size_t length;
        size_t line;
        TokenType type;
    };

    [[nodiscard]] std::string_view viewLexeme(const Token& token, std::string_view source);

    [[nodiscard]] std::string getLexeme(const Token& token, const std::string_view source);
}

#endif