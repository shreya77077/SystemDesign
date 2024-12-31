//Parser: Converts tokens into an Abstract Syntax Tree (AST).
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

// Abstract Syntax Tree (AST) Node Types
enum class ASTNodeType {
    PROGRAM,
    DECLARATION,
    ASSIGNMENT,
    EXPRESSION,
    CONDITIONAL
};

// AST Node structure
struct ASTNode {
    ASTNodeType type;
    std::string value;
    std::vector<ASTNode*> children;

    ASTNode(ASTNodeType t, const std::string& val) : type(t), value(val) {}

    ~ASTNode() {
        for (ASTNode* child : children) {
            delete child;
        }
    }
};

class Parser {
    std::vector<Token> tokens;
    size_t current = 0;

public:
    explicit Parser(const std::vector<Token>& tokenStream) : tokens(tokenStream) {}

    Token peek() {
        return current < tokens.size() ? tokens[current] : Token{TokenType::END_OF_FILE, ""};
    }

    Token advance() {
        return current < tokens.size() ? tokens[current++] : Token{TokenType::END_OF_FILE, ""};
    }

    bool match(TokenType type) {
        if (peek().type == type) {
            advance();
            return true;
        }
        return false;
    }

    ASTNode* parse() {
        ASTNode* root = new ASTNode(ASTNodeType::PROGRAM, "");
        while (peek().type != TokenType::END_OF_FILE) {
            root->children.push_back(parseStatement());
        }
        return root;
    }

private:
    ASTNode* parseStatement() {
        if (match(TokenType::KEYWORD)) {
            if (peek().value == "int") {
                return parseDeclaration();
            } else if (peek().value == "if") {
                return parseConditional();
            }
        }
        return parseAssignment();
    }

    ASTNode* parseDeclaration() {
        advance(); // Consume 'int'
        Token identifier = advance();
        if (identifier.type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected variable name after 'int'");
        }
        if (!match(TokenType::SYMBOL) || peek().value != ";") {
            throw std::runtime_error("Expected ';' after variable declaration");
        }
        advance(); // Consume ';'
        return new ASTNode(ASTNodeType::DECLARATION, identifier.value);
    }

    ASTNode* parseAssignment() {
        Token identifier = advance();
        if (identifier.type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected variable name for assignment");
        }
        if (!match(TokenType::OPERATOR) || peek().value != "=") {
            throw std::runtime_error("Expected '=' in assignment");
        }
        advance(); // Consume '='
        ASTNode* expr = parseExpression();
        if (!match(TokenType::SYMBOL) || peek().value != ";") {
            throw std::runtime_error("Expected ';' after assignment");
        }
        advance(); // Consume ';'
        ASTNode* assignmentNode = new ASTNode(ASTNodeType::ASSIGNMENT, identifier.value);
        assignmentNode->children.push_back(expr);
        return assignmentNode;
    }

    ASTNode* parseExpression() {
        Token number = advance();
        if (number.type != TokenType::NUMBER) {
            throw std::runtime_error("Expected a number in expression");
        }
        ASTNode* expr = new ASTNode(ASTNodeType::EXPRESSION, number.value);
        if (peek().type == TokenType::OPERATOR) {
            Token op = advance();
            ASTNode* right = parseExpression();
            ASTNode* opNode = new ASTNode(ASTNodeType::EXPRESSION, op.value);
            opNode->children.push_back(expr);
            opNode->children.push_back(right);
            return opNode;
        }
        return expr;
    }

    ASTNode* parseConditional() {
        advance(); // Consume 'if'
        ASTNode* condition = parseExpression();
        if (!match(TokenType::KEYWORD) || peek().value != "then") {
            throw std::runtime_error("Expected 'then' after condition");
        }
        advance(); // Consume 'then'
        ASTNode* thenBranch = parseStatement();
        ASTNode* elseBranch = nullptr;
        if (match(TokenType::KEYWORD) && peek().value == "else") {
            advance(); // Consume 'else'
            elseBranch = parseStatement();
        }
        ASTNode* conditionalNode = new ASTNode(ASTNodeType::CONDITIONAL, "if");
        conditionalNode->children.push_back(condition);
        conditionalNode->children.push_back(thenBranch);
        if (elseBranch) {
            conditionalNode->children.push_back(elseBranch);
        }
        return conditionalNode;
    }
};

#endif // SIMPLELANG_H
