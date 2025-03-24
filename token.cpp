//
// Created by antfu on 3/24/2025.
//

#include "token.h"
#include <iostream>
#include <iomanip>

void token::printToken() {
    std::cout << "TOKEN TYPE: " << tokenTypes.at(this->tType) << std::endl;
    std::cout << "Token: " << std::right << std::setw(6) << this->tokenStr << std::endl;

}