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

    void tokenize();
    void updateFile(std::fstream &file);
    void printFile();
    void processWord(std::string &prevWord, std::string &word, std::string &nextWord);
    void testPopulateTokens();
    void printTokens();



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
        ","

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
        {"\"", token::DOUBLE_QUOTE}





    };

};


#endif //TOKENIZE_H
