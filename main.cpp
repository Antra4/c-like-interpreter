#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "tokenizer.h"

//First step: Get the file as input, output it as a file
std::fstream removeComments(std::ifstream &file) {
    std::string outFileName = "comments_removed.txt";
    std::fstream writeFile;
    writeFile.open(outFileName, std::ios::out);

    // Check if output file opened successfully
    if (!writeFile.is_open()) {
        std::cerr << "Error: Could not open output file '" << outFileName << "'" << std::endl;
        file.close();
        exit(1);
    }

    std::string line;
    bool inLineComment = false;
    bool inBlockComment = false;
    bool inString = false;
    int lineCount = 0;
    int blockLine = 0;

    //Processing Stream
    while (std::getline(file, line)) {
        lineCount++;

        for(int i = 0; i < line.length(); i++) {
            char c = line.at(i);
            //Keep track of whether or not we are inside of a string
            if(c == '"') {
                inString = !inString;
            }

            //Remove in line comments
            if(inLineComment) {
                if(i != line.size()-1) {
                    line.at(i) = ' ';
                }

                else {
                    line.at(i) = ' ';
                    inLineComment = !inLineComment;
                }
            }

            if(c == '/' && !inString) {
                if(line[i-1] == '/' && !inString) {
                    line.at(i) = ' ';
                    line.at(i-1) = ' ';
                    inLineComment = true;
                }
            }

            //Remove Block Comments
            if(inBlockComment) {
                if(c == '*' && line[i+1] == '/') {
                    line[i] = ' ';
                    line[i+1] = ' ';
                    inBlockComment = !inBlockComment;
                }

                else {
                    line.at(i) = ' ';
                }
            }

            if((c == '*' && line[i+1] == '/') && !inBlockComment && !inString && !inLineComment) {
                blockLine = lineCount;
                inBlockComment = !inBlockComment;
                break;
            }

            if(c == '*' && !inString && !inBlockComment) {
                if(line[i-1] == '/') {
                    line.at(i) = ' ';
                    line.at(i-1) = ' ';
                    blockLine = lineCount;
                    inBlockComment = true;
                }
            }

        }

        writeFile << line << std::endl;
        // DELETE BEFORE FINAL: Also display to console
        //std::cout << line << std::endl;
    }

    if(inLineComment || inBlockComment) {
        std::cerr << "ERROR: Program contains C-style, unterminated comment on line " << blockLine << std::endl;
        exit(1);
    }

    writeFile.close();

    return writeFile;
}

void printFile(std::fstream &file, std::string fileName) {
    file.open(fileName, std::ios::in);
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Open the input file
    std::ifstream inFile(argv[1]);

    // Check if input file opened successfully
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open input file '" << argv[1] << "'" << std::endl;
        return 1;
    }

    std::fstream noCommentsCode = removeComments(inFile);

    //printFile(noCommentsCode, fileName);
    //set up tokenizer object to have a file as a member variable then call the tokenizer on that

    tokenizer noComments(noCommentsCode);

    noComments.tokenize();
    inFile.close();
    return 0;
}
