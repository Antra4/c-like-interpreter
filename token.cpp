//
// Created by antfu on 3/24/2025.
//

#include "token.h"
#include <iostream>
#include <iomanip>

void token::printToken() {
    std::cout << "\nToken Type: " << tokenTypes.at(this->tType) << std::endl;
    std::cout << "Token:      " << this->tokenStr << std::endl;

}