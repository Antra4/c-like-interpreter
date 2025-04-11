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

bool tokenizer::tokener() {
    std::string line;
    token prevToken;
    token prev2Token;
    int lineNum = 0;
    bool inString = false;
    bool startQuote = false;
    bool wasIdentifier = false;
    std::string inQuotes;

    parseFile->open("comments_removed.txt", std::ios::in);
    if (parseFile->is_open()) {
        std::string line;
        char c;

        while (std::getline(*parseFile, line)) {
            std::istringstream iss(line);
            lineNum++;
            std::string word = "";

            //begin checking for tokens
            while(iss.get(c)) {
                //look ahead at the next character
                char nextChar = iss.peek();

                if (prevToken.getTokenType() == token::IDENTIFIER || prevToken.getTokenType() == token::COMMA) {
                    wasIdentifier = true;
                }

                else {
                    wasIdentifier = false;
                }

                //if we hit whitespace
                if (c == ' ') {
                    //do we have a non-empty word?
                    if (!word.empty()) {
                        //check if we're declaring a variable
                        if (wasIdentifier) {
                            token newToken{word, token::IDENTIFIER};
                            tokens.push_back(newToken);
                            variableTokens.emplace(word, "var");
                            prevToken = newToken;
                            word.clear();
                        }

                        else {
                            //if a token was not found, flush the invalid token
                            std::cerr << "Unable to find token: " << word << " on line: " << lineNum << std::endl;
                            word.clear();
                        }
                    }
                }

                //parse line
                else {
                    //build words
                    word += c;

                    //check if we're seeing a number
                    if (word[0] == '-' || isdigit(word[0])) {
                        if (checkNum(iss, prevToken, c)) {
                            word.clear();
                            continue;
                        }
                    }

                    //check if we're reading a character
                    if (c == '\'') {
                        checkChar(iss, prevToken, c);
                        word.clear();
                        continue;
                    }

                    //check for boolean ops
                    if (word == "<" || word == ">" || word == "=") {
                        if (nextChar == '=') {
                            iss.get(c);
                            nextChar = iss.peek();
                            word += c;
                        }
                    }

                    //check if word is in our reserved list, if so add it to the token list
                    if (std::find(reservedWords.begin(), reservedWords.end(), word) != reservedWords.end()) {
                        token newToken{word, typeMap[word]};
                        tokens.push_back(newToken);
                        prevToken = newToken;
                        word.clear();
                    }

                    //is the word one of our variables?
                    else if (variableTokens.find(word) != variableTokens.end() && (nextChar == ' ' || nextChar == '[' || nextChar == ';' || nextChar == ')' || nextChar == ']' || nextChar ==',')) {
                        token newToken{word, token::IDENTIFIER};
                        tokens.push_back(newToken);
                        prevToken = newToken;
                        word.clear();
                        continue;
                    }
                }

                //are we about to go into a string?
                if (nextChar == '"') {
                    processString(iss, prevToken);
                }

                //are we declaring a variable
                if (wasIdentifier) {
                    if ((nextChar == ')' || nextChar == ';' || nextChar == '[' || nextChar == ',') && (c != ';' && c != ')' && c != ']')) {
                        token newToken{word, token::IDENTIFIER};
                        tokens.push_back(newToken);
                        variableTokens.emplace(word, "var");
                        prevToken = newToken;
                        word.clear();
                    }
                }
            }
            //test printing
            //std::cout << word << std::endl;
        }
    }
    return true;
}

void tokenizer::processString(std::istringstream &iss, token &prevToken) {
    char openQuote;
    char c;

    std::string inQuotes;

    iss.get(openQuote);

    if (openQuote != '"') {
        std::cerr << "Expected token '\"', saw token '" << openQuote << "'" << std::endl;
        return;
    }

    while (iss.get(c)) {
        if (c == '"') {
            char closeQuote = '"';
            break;
        }
        inQuotes += c;
    }
    token newToken{"\"", token::DOUBLE_QUOTE};
    tokens.push_back(newToken);
    token newToken2{inQuotes, token::STRING};
    tokens.push_back(newToken2);
    token newToken3{"\"", token::DOUBLE_QUOTE};
    tokens.push_back(newToken3);
    prevToken = newToken3;
}


bool tokenizer::checkNum(std::istringstream &iss, token &prevToken, char c) {
    if (c != '-' && !isdigit(c)) {
        std::cerr << "checkNum called on invalid input" << std::endl;
        return false;
    }

    //we hit a minus, not a negative
    if (c == '-' && !isdigit(iss.peek())) {
        return false;
    }

    std::string number;
    number += c;
    char nextChar;

    while (isdigit(iss.peek())) {
        iss.get(nextChar);
        number += nextChar;
    }

    token newToken{number, token::INTEGER};
    tokens.push_back(newToken);

    return true;
}


bool tokenizer::checkChar(std::istringstream &iss, token &prevToken, char c) {
    char openQuote = c;
    char inChar;
    bool escape = false;

    if (openQuote != '\'') {
        std::cerr << "checkChar called on invalid input" << std::endl;
        return false;
    }

    if (iss.peek() == '\\') {
        escape = true;
        iss.get(inChar);
    }

    iss.get(inChar);
    std::string outChar;
    if (escape) {
        outChar += '\\';
    }
    outChar += inChar;
    //grab last "'"
    iss.get(inChar);


    token newToken{"'", token::SINGLE_QUOTE};
    tokens.push_back(newToken);
    token newToken2{outChar, token::STRING};
    tokens.push_back(newToken2);
    token newToken3{"'", token::SINGLE_QUOTE};
    tokens.push_back(newToken3);
    prevToken = newToken3;

    return true;
}