#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <set>
#include <map>
#include <vector>
#include "frontend/token.hpp"

namespace tisp::frontend
{
    constexpr bool matchWhitespace(char c) noexcept
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    constexpr bool matchOpSymbol(char c) noexcept
    {
        return c == '$' || c == '@' || c == '='
            || c == '+' || c == '-' || c == '*' || c == '/'
            || c == '>' || c == '<' || c == '&' || c == '|'
            || c == ':';
    }

    constexpr bool matchNumeric(char c) noexcept
    {
        return (c >= '0' && c <= '9') || c == '.';
    }

    constexpr bool matchAlphabetic(char c) noexcept
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
    }

    struct LexicalEntry
    {
        std::string lexeme;
        TokenType type;
    };

    class Lexer
    {
    private:
        std::map<std::string, TokenType> symbols; // operators and punctuation
        std::set<std::string> kwords;
        std::set<std::string> tnames;
        std::string_view source;
        size_t limit;
        size_t pos;
        size_t line;

        [[nodiscard]] bool isAtEnd() const noexcept;
        [[nodiscard]] char peekSymbol() const;

        [[nodiscard]] Token lexWhitespace() noexcept;
        [[nodiscard]] Token lexOtherWord() noexcept;
        [[nodiscard]] Token lexNumber() noexcept;
        [[nodiscard]] Token lexPunctuation() noexcept;
        [[nodiscard]] Token lexSingle(TokenType type) noexcept;
        [[nodiscard]] Token lexBetween(char delim, TokenType type) noexcept;

    public:
        Lexer(std::string_view source_view);

        [[nodiscard]] Token lexNext();
    };
}

#endif