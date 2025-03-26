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
                if (iss >> word) {
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

                        std::cout << stringTok << std::endl;
                        std::cout << line << std::endl;

                        iss.str(line);

                        token newToken{stringTok, token::STRING};
                        tokens.push_back(newToken);
                        prev2Token = newToken;

                        token newToken2{"\"", token::DOUBLE_QUOTE};
                        tokens.push_back(newToken2);
                        prevToken = newToken2;

                        inString = false;
                        openQuote = false;
                    }

                    else {
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
                            if (asType == "int") {
                                if (endChar == ';') {
                                    word.pop_back();
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;

                                    token newToken2{";", token::SEMICOLON};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }

                                else if (endChar == ')') {
                                    word.pop_back();
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = newToken;

                                    token newToken2{")", token::SEMICOLON};
                                    tokens.push_back(newToken2);
                                    prevToken = newToken2;
                                }

                                else {
                                    token newToken{word, token::INTEGER};
                                    tokens.push_back(newToken);
                                    prev2Token = prevToken;
                                    prevToken = newToken;
                                }
                            }

                            else {
                                std::cerr << "error with assignment " << word << std::endl;
                            }
                        }

                        else if (variableTokens.find(word) != variableTokens.end()) {
                            token newToken{word, token::IDENTIFIER};
                            tokens.push_back(newToken);
                            prev2Token = prevToken;
                            prevToken = newToken;
                        }

                        else if (prevToken.getToken() == "procedure") {
                            token newToken{word, token::IDENTIFIER};
                            tokens.push_back(newToken);
                            prev2Token = prevToken;
                            prevToken = newToken;
                        }

                        else if (prevToken.getToken() == "int") {
                            char lastChar = word.back();
                            if (lastChar == ';' || lastChar == ')') {
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
                                token newToken2{";", token::SEMICOLON};
                                tokens.push_back(newToken2);
                                prev2Token = prevToken;
                                prevToken = newToken2;
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
                    }
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
    std::cout << "\nToken List: " << std::endl;
    for(auto token : tokens) {
        token.printToken();
    }
}

