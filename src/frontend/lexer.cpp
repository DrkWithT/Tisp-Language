/**
 * @file lexer.cpp
 * @author DrkWithT
 * @brief Implements tokenizer.
 * @date 2024-04-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "frontend/lexer.hpp"

namespace tisp::frontend
{
    /* Lexical config for Lexer */

    static const LexicalEntry entries[] {
        {.lexeme = "Boolean", .type = TokenType::tname},
        {.lexeme = "Integer", .type = TokenType::tname},
        {.lexeme = "Double", .type = TokenType::tname},
        {.lexeme = "String", .type = TokenType::tname},
        {.lexeme = "Seq", .type = TokenType::tname},
        {.lexeme = "Nil", .type = TokenType::tname},
        {.lexeme = "const", .type = TokenType::keyword},
        {.lexeme = "var", .type = TokenType::keyword},
        {.lexeme = "defun", .type = TokenType::keyword},
        {.lexeme = "match", .type = TokenType::keyword},
        {.lexeme = "case", .type = TokenType::keyword},
        {.lexeme = "return", .type = TokenType::keyword},
        {.lexeme = "while", .type = TokenType::keyword},
        {.lexeme = "generic", .type = TokenType::keyword},
        {.lexeme = "use", .type = TokenType::keyword},
        {.lexeme = "$", .type = TokenType::op_invoke},
        {.lexeme = "@", .type = TokenType::op_access},
        {.lexeme = "=", .type = TokenType::op_set},
        {.lexeme = "+", .type = TokenType::op_plus},
        {.lexeme = "-", .type = TokenType::op_minus},
        {.lexeme = "*", .type = TokenType::op_times},
        {.lexeme = "/", .type = TokenType::op_slash},
        {.lexeme = ">", .type = TokenType::op_gt},
        {.lexeme = ">=", .type = TokenType::op_gte},
        {.lexeme = "<", .type = TokenType::op_lt},
        {.lexeme = "<=", .type = TokenType::op_lte},
        {.lexeme = "&&", .type = TokenType::op_and},
        {.lexeme = "||", .type = TokenType::op_or},
        {.lexeme = ":", .type = TokenType::colon},
        {.lexeme = "->", .type = TokenType::arrow}
    };

    static const size_t entry_count = 30;

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
    : symbols {}, kwords {}, tnames {}, source {source_view}, limit {source_view.length()}, pos {0}
    {
        for (size_t entries_pos = 0; entries_pos < entry_count; entries_pos++)
        {
            auto entry_type = entries[entries_pos].type;

            if (entry_type == TokenType::tname)
                tnames.insert(entries[entries_pos].lexeme);
            else if (entry_type == TokenType::keyword)
                kwords.insert(entries[entries_pos].lexeme);
            else if (entry_type >= TokenType::op_invoke && entry_type <= TokenType::arrow)
                symbols[entries[entries_pos].lexeme] = entry_type;
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
