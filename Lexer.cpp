//Lexer Implementation
This converts the input SimpleLang code into a stream of tokens.
#include "SimpleLang.h"

class Lexer {
    std::string source;
    size_t pos = 0;

public:
    explicit Lexer(const std::string &src) : source(src) {}

    char peek() {
        return pos < source.size() ? source[pos] : '\0';
    }

    char advance() {
        return pos < source.size() ? source[pos++] : '\0';
    }

    Token nextToken() {
        while (isspace(peek())) advance();

        if (isalpha(peek())) { // Keywords or Identifiers
            std::string identifier;
            while (isalnum(peek())) identifier += advance();
            if (KEYWORDS.count(identifier))
                return {TokenType::KEYWORD, identifier};
            return {TokenType::IDENTIFIER, identifier};
        }

        if (isdigit(peek())) { // Numbers
            std::string number;
            while (isdigit(peek())) number += advance();
            return {TokenType::NUMBER, number};
        }

        if (OPERATORS.count(std::string(1, peek()))) { // Operators
            std::string op(1, advance());
            if (peek() == '=' && op == "=") { // Handle "=="
                op += advance();
            }
            return {TokenType::OPERATOR, op};
        }

        if (peek() == ';' || peek() == '(' || peek() == ')') { // Symbols
            return {TokenType::SYMBOL, std::string(1, advance())};
        }

        if (peek() == '\0') return {TokenType::END_OF_FILE, ""};

        throw std::runtime_error("Unknown token");
    }
};
