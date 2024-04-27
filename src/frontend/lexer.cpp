/**
 * @file lexer.cpp
 * @author DrkWithT
 * @brief Implements tokenizer.
 * @date 2024-04-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <array>
#include "frontend/lexer.hpp"

namespace tisp::frontend
{
    /* Lexical config for Lexer */

    LexicalEntry::LexicalEntry(const char *cstr, TokenType type_arg)
    : lexeme {cstr}, type {type_arg} {}

    constexpr size_t entry_count = 33;

    static std::array<LexicalEntry, entry_count> entries = {
        LexicalEntry {"Boolean", TokenType::tname},
        {"Integer", TokenType::tname},
        {"Double", TokenType::tname},
        {"String", TokenType::tname},
        {"Seq", TokenType::tname},
        {"Nil", TokenType::tname},
        {"const", TokenType::keyword},
        {"var", TokenType::keyword},
        {"defun", TokenType::keyword},
        {"match", TokenType::keyword},
        {"case", TokenType::keyword},
        {"return", TokenType::keyword},
        {"while", TokenType::keyword},
        {"generic", TokenType::keyword},
        {"use", TokenType::keyword},
        {"$", TokenType::op_invoke},
        {"@", TokenType::op_access},
        {"=", TokenType::op_set},
        {"+", TokenType::op_plus},
        {"-", TokenType::op_minus},
        {"*", TokenType::op_times},
        {"/", TokenType::op_slash},
        {">", TokenType::op_gt},
        {">=", TokenType::op_gte},
        {"<", TokenType::op_lt},
        {"<=", TokenType::op_lte},
        {"&&", TokenType::op_and},
        {"||", TokenType::op_or},
        {":", TokenType::colon},
        {"->", TokenType::arrow},
        {"nil", TokenType::lt_nil},
        {"true", TokenType::lt_true},
        {"false", TokenType::lt_false}
    };

    /* Lexer private impl. */

    bool Lexer::isAtEnd() const noexcept
    {
        return pos >= limit;
    }

    char Lexer::peekSymbol() const
    {
        return source[pos];
    }

    Token Lexer::lexWhitespace() noexcept
    {
        size_t lex_begin = pos;
        size_t lex_len = 0;
        size_t lex_line = line;
        char temp;
        bool found_lf = false;

        while (!isAtEnd())
        {
            temp = peekSymbol();

            if (temp == '\n' && !found_lf)
            {
                line += 1;
                found_lf = true;
                lex_line = line;
            }

            if (!matchWhitespace(temp))
                break;

            lex_len++;
            pos++;
        }

        return {.begin = lex_begin, .length = lex_len, .line = lex_line, .type = TokenType::whitespace};
    }

    Token Lexer::lexOtherWord() noexcept
    {
        size_t lex_begin = pos;
        size_t lex_len = 0;
        char temp;

        while (!isAtEnd())
        {
            temp = peekSymbol();

            if (!matchAlphabetic(temp))
                break;

            lex_len++;
            pos++;
        }

        Token result {.begin = lex_begin, .length = lex_len, .line = line, .type = TokenType::unknown};

        std::string lexeme = getLexeme(result, source);

        if (tnames.find(lexeme) != tnames.end())
            result.type = TokenType::tname;
        else if (kwords.find(lexeme) != kwords.end())
            result.type = TokenType::keyword;
        else if (specials.find(lexeme) != specials.end())
            result.type = specials.at(lexeme);
        else
            result.type = TokenType::identifier;

        return result;
    }

    Token Lexer::lexNumber() noexcept
    {
        size_t lex_begin = pos;
        size_t lex_len = 0;
        int dots = 0;
        char temp;

        while (!isAtEnd())
        {
            temp = peekSymbol();

            if (!matchNumeric(temp))
                break;

            if (temp == '.')
                dots++;

            lex_len++;
            pos++;
        }

        if (dots < 1)
            return {.begin = lex_begin, .length = lex_len, .line = line, .type = TokenType::num_int};
        else if (dots == 1)
            return {.begin = lex_begin, .length = lex_len, .line = line, .type = TokenType::num_dbl};

        return {.begin = lex_begin, .length = lex_len, .line = line, .type = TokenType::unknown};
    }

    Token Lexer::lexPunctuation() noexcept
    {
        size_t lex_begin = pos;
        size_t lex_len = 0;
        char temp;

        while (!isAtEnd())
        {
            temp = peekSymbol();

            if (!matchOpSymbol(temp))
                break;

            lex_len++;
            pos++;
        }

        Token result {.begin = lex_begin, .length = lex_len, .line = line, .type = TokenType::unknown};
        std::string lexeme = getLexeme(result, source);

        if (symbols.find(lexeme) == symbols.end())
            result.type = TokenType::unknown;
        else
            result.type = symbols.at(lexeme);

        return result;
    }

    [[nodiscard]] Token Lexer::lexSingle(TokenType type) noexcept
    {
        size_t lex_begin = pos;

        pos++;

        return {.begin = lex_begin, .length = 1, .line = line, .type = type};
    }

    [[nodiscard]] Token Lexer::lexBetween(char delim, TokenType type) noexcept
    {
        pos++; // skip 1st delim after it's peeked

        size_t lex_begin = pos;
        size_t lex_len = 0;
        char temp;

        while (!isAtEnd())
        {
            temp = peekSymbol();

            if (temp == delim)
            {
                pos++;
                break;
            }

            lex_len++;
            pos++;
        }

        return {.begin = lex_begin, .length = lex_len, .line = line, .type = type};
    }

    /* Lexer public impl. */

    Lexer::Lexer(std::string_view source_view)
    : symbols {}, kwords {}, tnames {}, specials {}, source {source_view}, limit {source_view.length()}, pos {0}
    {
        for (const auto& item : entries)
        {
            const auto& [lexeme, type] = item;

            if (type == TokenType::tname)
                tnames.insert(lexeme);
            else if (type == TokenType::keyword)
                kwords.insert(lexeme);
            else if (type >= TokenType::op_invoke && type <= TokenType::arrow)
                symbols[lexeme] = type;
            else if (type >= TokenType::lt_nil && type <= TokenType::lt_false)
                specials[lexeme] = type;
        }
    }

    Token Lexer::lexNext()
    {
        if (isAtEnd())
            return {.begin = limit, .length = 1, .line = line, .type = TokenType::eof};

        char c = peekSymbol();

        switch (c)
        {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                return lexWhitespace();
            case '#':
                return lexBetween('#', TokenType::comment);
            case '.':
                return lexSingle(TokenType::dot);
            case ',':
                return lexSingle(TokenType::comma);
            case '\"':
                return lexBetween('\"', TokenType::strbody);
            case '(':
                return lexSingle(TokenType::lparen);
            case ')':
                return lexSingle(TokenType::rparen);
            case '[':
                return lexSingle(TokenType::lbrack);
            case ']':
                return lexSingle(TokenType::rbrack);
            case '{':
                return lexSingle(TokenType::lbrace);
            case '}':
                return lexSingle(TokenType::rbrace);
            default:
                break;
        }

        if (matchOpSymbol(c))
            return lexPunctuation();
        else if (matchAlphabetic(c))
            return lexOtherWord();
        else if (matchNumeric(c))
            return lexNumber();

        pos += 1;

        return {.begin = pos - 1, .length = 1, .line = line, .type = TokenType::unknown};
    }
}
