//
// Created by Anthony on 3/9/2025.
//

#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <fstream>
#include <vector>
#include "token.h"
#include <map>


class tokenizer {

public:
    tokenizer(std::fstream &inFile);

    void updateFile(std::fstream &file);
    void printFile();
    void processWord(std::string &prevWord, std::string &word, std::string &nextWord);
    void testPopulateTokens();
    void printTokens();
    bool isNumber(const std::string& str);
    std::string removeArrSize(std::string &word);
    void processChar(std::string inStr, token &prevToken, token &prev2Token);
    bool tokener();
//    bool tester();
    void processString(std::istringstream &iss, token &prevToken);
    bool checkNum(std::istringstream &iss, token &prevToken, char c);
    bool checkChar(std::istringstream &iss, token &prevToken, char c);




private:
    std::fstream* parseFile;
    std::vector<token> tokens;
    std::vector<std::string> reservedWords {
        "procedure",
        "void",
        "\"",
        "int",
        "(",
        ")",
        "{",
        "}",
        ";",
        "=",
        "printf",
        ",",
        "if",
        "*",
        "+",
        "%",
        "else",
        "while",
        "==",
        "<=",
        "function",
        "char",
        ">=",
        "&&",
        "-",
        "return",
        "for",
        "<",
        ">",
        "[",
        "]",
        "'",
        "bool",
        "read",
        "sizeof",
        "/",
        "TRUE",
        "FALSE"

    };

    std::map<std::string, token::tokenType> typeMap = {
        {"procedure", token::IDENTIFIER},
        {"=", token::ASSIGNMENT_OPERATOR},
        {"{", token::L_BRACE},
        {"int", token::IDENTIFIER},
        {"printf", token::IDENTIFIER},
        {"}", token::R_BRACE},
        {"(", token::L_PAREN},
        {")", token::R_PAREN},
        {"void", token::IDENTIFIER},
        {"\"", token::DOUBLE_QUOTE},
        {",", token::COMMA},
        {";", token::SEMICOLON},
        {"if", token::IDENTIFIER},
        {"%", token::MODULO},
        {"*", token::ASTERISK},
        {"+", token::PLUS},
        {"else", token::IDENTIFIER},
        {"while", token::IDENTIFIER},
        {"==", token::BOOLEAN_EQUAL},
        {"<=", token::LT_EQUAL},
        {"function", token::IDENTIFIER},
        {">=", token::GT_EQUAL},
        {"&&", token::BOOLEAN_AND},
        {"-", token::MINUS},
        {"return", token::IDENTIFIER},
        {"for", token::IDENTIFIER},
        {"<", token::LT},
        {">", token::GT},
        {"[", token::L_BRACKET},
        {"]", token::R_BRACKET},
        {"'", token::SINGLE_QUOTE},
        {"char", token::IDENTIFIER},
        {"bool", token::IDENTIFIER},
        {"read", token::IDENTIFIER},
        {"sizeof", token::IDENTIFIER},
        {"/", token::DIVIDE},
        {"TRUE", token::IDENTIFIER},
        {"FALSE", token::IDENTIFIER}
    };

    std::map<std::string, std::string> variableTokens {};


};


#endif //TOKENIZE_H
