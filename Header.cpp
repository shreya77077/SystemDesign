//Lexer: Tokenizes the SimpleLang code into meaningful symbols.
//Header File for Token Definitions
#ifndef SIMPLELANG_H
#define SIMPLELANG_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdexcept>

// Token types
enum class TokenType {
    KEYWORD,  // "int", "if", "then", "else"
    IDENTIFIER,  // Variable names
    NUMBER,  // Numeric constants
    OPERATOR,  // +, -, =, ==
    SYMBOL,  // ;, (, )
    END_OF_FILE
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
};

// Keywords and operators
const std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"int", TokenType::KEYWORD},
    {"if", TokenType::KEYWORD},
    {"then", TokenType::KEYWORD},
    {"else", TokenType::KEYWORD}
};

const std::unordered_map<std::string, TokenType> OPERATORS = {
    {"+", TokenType::OPERATOR},
    {"-", TokenType::OPERATOR},
    {"=", TokenType::OPERATOR},
    {"==", TokenType::OPERATOR}
};

#endif // SIMPLELANG_H
