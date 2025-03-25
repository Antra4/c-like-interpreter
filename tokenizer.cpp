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
    int lineNum = 0;

    parseFile->open("comments_removed.txt", std::ios::in);
    if (parseFile->is_open()) {
        std::string line;
        while (std::getline(*parseFile, line)) {
            std::istringstream iss(line);
            lineNum++;
            //std::cout << line << std::endl;

            while(iss) {
                if (iss >> word) {

                    if (std::find(reservedWords.begin(), reservedWords.end(), word) != reservedWords.end()) {
                        token newToken{word, typeMap[word]};
                        tokens.push_back(newToken);
                        prevToken = newToken;
                    }

                    else if (prevToken.getToken() == "procedure") {
                        token newToken{word, token::IDENTIFIER};
                        tokens.push_back(newToken);
                        prevToken = newToken;
                    }

                    else if (prevToken.getToken() == "int") {
                        char lastChar = word.back();
                        if (lastChar == ';') {
                            word.pop_back();
                        }
                        token newToken{word, token::IDENTIFIER};
                        tokens.push_back(newToken);
                        prevToken = newToken;

                        if (lastChar == ';') {
                            token newToken2{";", token::SEMICOLON};
                            tokens.push_back(newToken2);
                            prevToken = newToken2;
                        }
                    }

                    else {
                        std::string parseWord;
                        for (int i =0; i < word.size(); i++) {
                            parseWord += word[i];
                            if (std::find(reservedWords.begin(), reservedWords.end(), parseWord) != reservedWords.end()) {
                                token newToken{parseWord, typeMap[parseWord]};
                                tokens.push_back(newToken);
                                prevToken = newToken;
                                parseWord.clear();
                            }

                        }
                        if (!parseWord.empty()) {
                            std::cerr << "Unexpected token \"" << parseWord << "\" on line " << lineNum << std::endl;
                        }
                    }

                }    //Do thing
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

