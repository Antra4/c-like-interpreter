//
// Created by Anthony on 3/9/2025.
//

#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <fstream>
#include <array>



class tokenizer {

public:
    tokenizer(std::fstream &inFile);

    void tokenize();
    void updateFile(std::fstream &file);
    void printFile();
    void processWord(std::string &prevWord, std::string &word, std::string &nextWord);


private:
    std::fstream* parseFile;
};


#endif //TOKENIZE_H
