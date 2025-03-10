//
// Created by Anthony on 3/9/2025.
//

#include "tokenizer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
}


void tokenizer::tokenize(std::fstream &file) {
    std::string line;
    std::string word;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::cout << line << std::endl;


        while(iss) {
            iss >> word;
            std::cout << word << std::endl;
        }

    }
}