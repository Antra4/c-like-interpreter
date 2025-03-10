//
// Created by Anthony on 3/9/2025.
//

#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <fstream>



class tokenizer {

public:
    tokenizer(std::fstream &inFile);

    void tokenize(std::fstream &file);
    void updateFile(std::fstream &file);
    void printFile();

private:
    std::fstream* parseFile;
};


#endif //TOKENIZE_H
