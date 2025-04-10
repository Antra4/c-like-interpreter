//
// Created by Anthony on 3/9/2025.
//

#include "tokenizer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

tokenizer::tokenizer(std::fstream &inFile) {
    parseFile = &inFile;
}

void tokenizer::updateFile(std::fstream &file) {
    parseFile = &file;
}

void tokenizer::printFile() {
    parseFile->open("comments_removed.txt", std::ios::in);
    if (parseFile->is_open()) {
        std::string line;
        while (std::getline(*parseFile, line)) {
            std::cout << line << std::endl;
        }
        parseFile->close();
    }

    else {
        std::cerr << "printFile() was unable to open file" << std::endl;
    }
}

std::string tokenizer::removeArrSize(std::string &word) {
    // Find the opening bracket
    size_t bracketPos = word.find('[');

    // If no bracket found, return -1 or throw an exception
    if (bracketPos == std::string::npos) {
        return word;
    }

    // Find the closing bracket
    size_t closeBracketPos = word.find(']', bracketPos);

    // Check for invalid array notation
    if (closeBracketPos == std::string::npos) {
        throw std::invalid_argument("Invalid array notation: missing closing bracket");
    }

    // Extract the index substring
    std::string indexStr = word.substr(bracketPos + 1,
                                       closeBracketPos - bracketPos - 1);

    // Check if the index is a valid number
    for (char c : indexStr) {
        if (!std::isdigit(c)) {
            throw std::invalid_argument("Invalid array index: non-numeric characters");
        }
    }

    // Convert index to integer
    int index = std::stoi(indexStr);

    // Remove the array notation from the word
    word = word.substr(0, bracketPos);

    return std::to_string(index);
}

void tokenizer::processChar(std::string inStr, token &prevToken, token &prev2Token) {
    if(inStr[0] != '\'') {
        std::cerr << "Not in single quotes" << std::endl;
        return;
    }

    else if(inStr.length() != 3) {
        std::cerr << "Invalid String length in char process" << std::endl;
        return;
    }

    else{
        std::string s;
        s.push_back(inStr[1]);
        token newToken1{"'", token::SINGLE_QUOTE};
        token newToken2{s, token::STRING};
        token newToken3{"'", token::SINGLE_QUOTE};

        tokens.push_back(newToken1);
        tokens.push_back(newToken2);
        tokens.push_back(newToken3);

        prevToken = newToken3;
        prev2Token = newToken2;
    }
}


void tokenizer::tokenize() {
    std::string line;
    std::string word;
    token prevToken;
    token prev2Token;
    int lineNum = 0;
    bool inString = false;
    bool openQuote = false;

    parseFile->open("comments_removed.txt", std::ios::in);
    if (parseFile->is_open()) {
        std::string line;
        while (std::getline(*parseFile, line)) {
            std::istringstream iss(line);
            lineNum++;

            while(iss) {
                if (prevToken.getToken() == "\"" && openQuote) {
                    inString = true;
                }
                if (inString) {
                    //read all characters into a string token until we hit a double quote
                    std::string stringTok;
                    std::stringstream ss(line);

                    getline(ss, stringTok, '"');
                    getline(ss, stringTok, '"');
                    getline(ss, line);

                    iss.clear();
                    iss.str(line);

                    //std::cout << line << std::endl;
                    //std::cout << stringTok << std::endl;

                    token newToken{stringTok, token::STRING};
                    tokens.push_back(newToken);
                    prev2Token = newToken;

                    token newToken2{"\"", token::DOUBLE_QUOTE};
                    tokens.push_back(newToken2);
                    prevToken = newToken2;

                    inString = false;
                    openQuote = false;
                }

                if (iss >> word) {
//                    if (prevToken.getToken() == "\"" && openQuote) {
//                        inString = true;
//                    }
//                    if (inString) {
//                        //read all characters into a string token until we hit a double quote
//                        std::string stringTok;
//                        std::stringstream ss(line);
//
//                        getline(ss, stringTok, '"');
//                        getline(ss, stringTok, '"');
//                        getline(ss, line);
//
//                        iss.str(line);
//
//                        //std::cout << line << std::endl;
//                        //std::cout << stringTok << std::endl;
//
//                        token newToken{stringTok, token::STRING};
//                        tokens.push_back(newToken);
//                        prev2Token = newToken;
//
//                        token newToken2{"\"", token::DOUBLE_QUOTE};
//                        tokens.push_back(newToken2);
//                        prevToken = newToken2;
//
//                        inString = false;
//                        openQuote = false;
//                    }

//                    else {
                        if (std::find(reservedWords.begin(), reservedWords.end(), word) != reservedWords.end()) {
                            token newToken{word, typeMap[word]};
                            tokens.push_back(newToken);
                            prev2Token = prevToken;
                            prevToken = newToken;
                        }

                        //Assignment logic
                        else if (prevToken.getToken() == "=") {
                            std::string asType;
                            char endChar = word.back();
                            auto test = variableTokens.find(prev2Token.getToken());
                            if (test != variableTokens.end()) {
                                asType = test->second;
                            }
                            //Check if the word is an integer or a variable
                            if (asType == "int") {
                                if (endChar == ';') {
                                    word.pop_back();

                                    bool addParen = false;

                                    if(word.back() == ')') {
                                        word.pop_back();
                                        addParen = true;
                                    }
                                    if (isNumber(word)) {
                                        token newToken{word, token::INTEGER};
                                        tokens.push_back(newToken);
                                        prev2Token = newToken;
                                    }
                                    else {
                                        token newToken{word, token::IDENTIFIER};
                                        tokens.push_back(newToken);
                                        prev2Token = newToken;
                                    }

                                    if (addParen) {
                                        token newToken{")", token::R_PAREN};
                                        tokens.push_back(newToken);
                                        prev2Token = newToken;
                                    }

                                    token newToken2{";", token::SEMICOLON};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }


                                else if (endChar == ')') {
                                    word.pop_back();
                                    if (isNumber(word)) {
                                        token newToken{word, token::INTEGER};
                                        tokens.push_back(newToken);
                                        prev2Token = newToken;
                                    }
                                    else {
                                        token newToken{word, token::IDENTIFIER};
                                        tokens.push_back(newToken);
                                        prev2Token = newToken;
                                    }

                                    token newToken2{")", token::R_PAREN};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }

                                else {
                                    if (isNumber(word)) {
                                        token newToken{word, token::INTEGER};
                                        tokens.push_back(newToken);
                                        prev2Token = prevToken;
                                        prevToken = newToken;                                    }
                                    else {
                                        token newToken{word, token::IDENTIFIER};
                                        tokens.push_back(newToken);
                                        prev2Token = prevToken;
                                        prevToken = newToken;                                    }
                                }
                            }

                            else if (asType == "char") {
                                char dQuote = word[0];
                                if (dQuote != '"') {
                                    std::cerr << "error assigning string" << word << std::endl;
                                }
                                token newToken{"\"", token::DOUBLE_QUOTE};
                                tokens.push_back(newToken);
                                word.erase(0, 1);

                                prev2Token = prevToken;
                                prevToken = newToken;
                                openQuote = true;
                                continue;
                            }

                            else {
                                std::cerr << "error with assignment " << word << std::endl;
                            }
                        }

                        //Modulo Operator Logic
                        else if (prevToken.getToken() == "%" || prevToken.getToken() == "==" || prevToken.getToken() == "<=" || prevToken.getToken() == ">=" || prevToken.getToken() == ">" || prevToken.getToken() == "<")  {
                            char endChar = word.back();
                            if (endChar == ';') {
                                word.pop_back();

                                if(word[0] == '\'') {
                                    processChar(word, prevToken, prev2Token);
                                    prev2Token = prevToken;
                                    token newToken2{";", token::SEMICOLON};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }

                                else {
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;
                                    token newToken2{";", token::SEMICOLON};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }
                            }

                            else if (endChar == ')') {
                                word.pop_back();
                                bool doubleParen = false;

                                if(word.back() == ')') {
                                    word.pop_back();
                                    doubleParen = true;
                                }

                                if(word[0] == '\'') {
                                    processChar(word, prevToken, prev2Token);
                                    prev2Token = prevToken;
                                    token newToken2{")", token::R_PAREN};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }

                                else {
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;
                                    token newToken2{")", token::R_PAREN};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }

                                if (doubleParen) {
                                    token newToken3{")", token::R_PAREN};
                                    tokens.push_back(newToken3);
                                    prev2Token = newToken3;
                                }

                            }

                            else {
                                if(word[0] == '\'') {
                                    processChar(word, prevToken, prev2Token);
                                }

                                else {
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = prevToken;
                                    prevToken = newToken;
                                }
                            }
                        }

                        //Variable Logic
                        else if (variableTokens.find(word) != variableTokens.end()) {
                            token newToken{word, token::IDENTIFIER};
                            tokens.push_back(newToken);
                            prev2Token = prevToken;
                            prevToken = newToken;
                        }

                        else if (prevToken.getToken() == "procedure" || prevToken.getToken() == "function") {
                            token newToken{word, token::IDENTIFIER};
                            tokens.push_back(newToken);
                            variableTokens.emplace(word, "func");
                            prev2Token = prevToken;
                            prevToken = newToken;
                        }

                        else if (prevToken.getToken() == "int") {
                            char lastChar = word.back();
                            if (lastChar == ';' || lastChar == ')' || lastChar == ',') {
                                word.pop_back();
                            }
                            token newToken{word, token::IDENTIFIER};
                            variableTokens.emplace(word, "int");
                            tokens.push_back(newToken);
                            prev2Token = prevToken;
                            prevToken = newToken;

                            if (lastChar == ';') {
                                token newToken2{";", token::SEMICOLON};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }

                            if (lastChar == ')') {
                                token newToken2{")", token::R_PAREN};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }

                            if (lastChar == ',') {
                                token newToken2{",", token::COMMA};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }
                        }

                        else if (prevToken.getToken() == "char") {
                            char lastChar = word.back();
                            if (lastChar == ';' || lastChar == ')') {
                                word.pop_back();
                            }
                            char penultimateChar = word.back();
                            if (penultimateChar == ']') {
                                std::string num = removeArrSize(word);
                                token newToken{word, token::IDENTIFIER};
                                variableTokens.emplace(word, "char");
                                tokens.push_back(newToken);

                                token newToken2{"[", token::L_BRACKET};
                                tokens.push_back(newToken2);

                                token newToken3{num, token::INTEGER};
                                tokens.push_back(newToken3);
                                prev2Token = newToken3;

                                token newToken4{"]", token::R_BRACKET};
                                tokens.push_back(newToken4);
                                prevToken = newToken4;
                            }

                            token newToken{word, token::IDENTIFIER};
                            variableTokens.emplace(word, "char");
                            tokens.push_back(newToken);
                            prev2Token = prevToken;
                            prevToken = newToken;

                            if (lastChar == ';') {
                                token newToken2{";", token::SEMICOLON};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }

                            if (lastChar == ')') {
                                token newToken2{")", token::R_PAREN};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }
                        }

                        //Listed Declarations
                        else if (prevToken.getToken() == "," && variableTokens.find(prev2Token.getToken()) != variableTokens.end()) {
                            char lastChar = word.back();
                            if (lastChar == ';' || lastChar == ')' || lastChar == ',') {
                                word.pop_back();
                            }
                            token newToken{word, prev2Token.getTokenType()};

                            std::string asType;
                            auto test = variableTokens.find(prev2Token.getToken());
                            if (test != variableTokens.end()) {
                                asType = test->second;
                            }

                            variableTokens.emplace(word, asType);
                            tokens.push_back(newToken);
                            prev2Token = prevToken;
                            prevToken = newToken;

                            if (lastChar == ';') {
                                token newToken2{";", token::SEMICOLON};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }

                            if (lastChar == ')') {
                                token newToken2{")", token::R_PAREN};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }

                            if (lastChar == ',') {
                                token newToken2{",", token::COMMA};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
                            }
                        }

                        //problem with '0'
                        else if (prevToken.getToken() == "*" || prevToken.getToken() == "+" || prevToken.getToken() == "-") {
                            char endChar = word.back();
                            if (endChar == ';') {
                                word.pop_back();
                                if (isNumber(word)) {
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;
                                }

                                else if(word[0] == '\'') {
                                    processChar(word, prevToken, prev2Token);
                                }

                                else {
                                    token newToken{word, token::IDENTIFIER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;
                                }


                                token newToken2{";", token::SEMICOLON};
                                tokens.push_back(newToken2);
                                prevToken = newToken2;
                            }

                            else if (endChar == ')') {
                                word.pop_back();
                                if (isNumber(word)) {
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;
                                }

                                else if(word[0] == '\'') {
                                    processChar(word, prevToken, prev2Token);
                                }

                                else {
                                    token newToken{word, token::IDENTIFIER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;
                                }

                                token newToken2{")", token::R_PAREN};
                                tokens.push_back(newToken2);
                                prevToken = newToken2;
                            }

                            else {
                                if (isNumber(word)) {
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = prevToken;
                                    prevToken = newToken;
                                }

                                else if(word[0] == '\'') {
                                    processChar(word, prevToken, prev2Token);
                                }

                                else {
                                    token newToken{word, token::IDENTIFIER};
                                    tokens.push_back(newToken);
                                    prev2Token = prevToken;
                                    prevToken = newToken;                                    }
                            }
                        }

                        //Look for tokens not separated by whitespace
                        else {
                            std::string parseWord;
                            for (int i =0; i < word.size(); i++) {
                                parseWord += word[i];
                                if (std::find(reservedWords.begin(), reservedWords.end(), parseWord) != reservedWords.end()) {
                                    token newToken{parseWord, typeMap[parseWord]};
                                    tokens.push_back(newToken);
                                    prev2Token = prevToken;
                                    prevToken = newToken;
                                    parseWord.clear();

                                    if (prevToken.getToken() == "\"") {
                                        openQuote = true;
                                        break;
                                    }
                                }

                                else if (variableTokens.find(parseWord) != variableTokens.end()) {
                                    token newToken{parseWord, token::IDENTIFIER};
                                    tokens.push_back(newToken);
                                    prev2Token = prevToken;
                                    prevToken = newToken;
                                    parseWord.clear();
                                }

                            }
                            if (!parseWord.empty()) {
                                std::cerr << "Unexpected token \"" << parseWord << "\" on line " << lineNum << std::endl;
                            }
                        }
                   // }
                }
            }
        }
    }

    else {
        std::cerr << "tokenize() was unable to open file" << std::endl;
    }
}

void tokenizer::processWord(std::string &prevWord, std::string &word, std::string &nextWord) {

}

void tokenizer::testPopulateTokens() {
    for (int i = 0; i < 20; i++) {
        token newToken;
        tokens.push_back(newToken);
    }
}
void tokenizer::printTokens() {
    std::cout << "\nToken list:" << std::endl;
    for(auto token : tokens) {
        token.printToken();
    }
    std::cout << std::endl << std::endl;
}

bool tokenizer::isNumber(const std::string& str) {
    // Empty string is not a number
    if (str.empty()) {
        return false;
    }

    // Track if we've seen a decimal point
    bool decimalPointSeen = false;

    // Start index to handle potential sign
    size_t start = 0;

    // Check for optional sign at the beginning
    if (str[0] == '+' || str[0] == '-') {
        start = 1;

        // String with only a sign is not a number
        if (str.length() == 1) {
            return false;
        }
    }

    // Check each character after the potential sign
    for (size_t i = start; i < str.length(); ++i) {
        char c = str[i];

        // Check if character is a digit
        if (std::isdigit(c)) {
            continue;
        }

        // Check for decimal point
        if (c == '.' && !decimalPointSeen) {
            decimalPointSeen = true;

            // Decimal point can't be the last character
            if (i == str.length() - 1) {
                return false;
            }

            continue;
        }

        // Any other character means it's not a number
        return false;
    }

    return true;
}

