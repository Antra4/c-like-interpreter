//
// Created by antfu on 3/24/2025.
//

#ifndef CS460INTERPRETER_TOKEN_H
#define CS460INTERPRETER_TOKEN_H

#include <string>
#include <vector>


class token {

public:

    token() {
        tokenStr = "DEFAULT";
        tType = UNKNOWN;
    }

    enum tokenType {
        // Character Sets
        CHARACTER,              // Basic character set
        ESCAPED_CHARACTER,      // Escaped characters like \n, \t, etc.
        LETTER,                 // Alphabetic characters A-Z, a-z
        DIGIT,                  // Numeric digits 0-9
        HEX_DIGIT,              // Hexadecimal digits 0-9, A-F, a-f

        // Delimiters
        L_PAREN,                // (
        R_PAREN,                // )
        L_BRACKET,              // [
        R_BRACKET,              // ]
        L_BRACE,                // {
        R_BRACE,                // }
        DOUBLE_QUOTE,           // "
        SINGLE_QUOTE,           // '
        SEMICOLON,              // ;
        COMMA,                  // ,

        // Operators
        ASSIGNMENT_OPERATOR,    // =
        PLUS,                   // +
        MINUS,                  // -
        ASTERISK,               // *
        DIVIDE,                 // /
        MODULO,                 // %
        CARET,                  // ^
        LT,                     // <
        GT,                     // >
        LT_EQUAL,               // <=
        GT_EQUAL,               // >=

        // Boolean operators
        BOOLEAN_AND,            // &&
        BOOLEAN_OR,             // ||
        BOOLEAN_NOT,            // !
        BOOLEAN_EQUAL,          // ==
        BOOLEAN_NOT_EQUAL,      // !=
        BOOLEAN_TRUE,           // TRUE
        BOOLEAN_FALSE,          // FALSE

        // Compound tokens
        STRING,                 // Sequence of characters
        DOUBLE_QUOTED_STRING,   // String in double quotes
        SINGLE_QUOTED_STRING,   // String in single quotes
        LETTER_UNDERSCORE,      // Letter or underscore
        LETTER_DIGIT_UNDERSCORE, // Letter, digit, or underscore
        WHOLE_NUMBER,           // Sequence of digits
        INTEGER,                // Whole number with optional sign
        IDENTIFIER_TAIL,        // Tail part of an identifier
        IDENTIFIER,             // Complete identifier

        // Lists
        IDENTIFIER_LIST,        // List of identifiers
        IDENTIFIER_ARRAY_LIST,  // List of array identifiers
        IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST, // Combined list

        // Language keywords and types
        DATATYPE_SPECIFIER,     // char, bool, int

        // Expressions
        NUMERICAL_OPERAND,      // Operand in numerical expression
        NUMERICAL_OPERATOR,     // Operator in numerical expression
        BOOLEAN_OPERATOR,       // Boolean operator
        EQUALITY_EXPRESSION,    // == or !=
        RELATIONAL_EXPRESSION,  // <, <=, >, >=, ==, !=
        NUMERICAL_EXPRESSION,   // Expression with numerical result
        BOOLEAN_EXPRESSION,     // Expression with boolean result
        INITIALIZATION_EXPRESSION, // Assignment in initialization
        EXPRESSION,             // Generic expression

        // Statements
        SELECTION_STATEMENT,    // if-then-else statement
        ITERATION_STATEMENT,    // for or while loop
        ASSIGNMENT_STATEMENT,   // Assignment with semicolon
        PRINTF_STATEMENT,       // printf function call
        GETCHAR_FUNCTION,       // getchar function call
        USER_DEFINED_FUNCTION,  // Custom function call
        DECLARATION_STATEMENT,  // Variable declaration
        RETURN_STATEMENT,       // Return statement
        STATEMENT,              // Generic statement
        COMPOUND_STATEMENT,     // Multiple statements
        BLOCK_STATEMENT,        // Block in braces

        // Function-related
        PARAMETER_LIST,         // Function parameters
        FUNCTION_DECLARATION,   // Function definition
        PROCEDURE_DECLARATION,  // Procedure definition
        MAIN_PROCEDURE,         // Main procedure

        // Program structure
        PROGRAM,                // Complete program

        // BNF-specific tokens (not in the language but in the meta-notation)
        DEFINITION,             // ::=
        ALTERNATION,            // |

        // Special tokens
        END_OF_FILE,            // End of input
        UNKNOWN                 // Unrecognized token
    };

    std::vector<std::string> tokenTypes = {{
        // Character Sets
        "CHARACTER",
        "ESCAPED_CHARACTER",
        "LETTER",
        "DIGIT",
        "HEX_DIGIT",

        // Delimiters
        "L_PAREN",
        "R_PAREN",
        "L_BRACKET",
        "R_BRACKET",
        "L_BRACE",
        "R_BRACE",
        "DOUBLE_QUOTE",
        "SINGLE_QUOTE",
        "SEMICOLON",
        "COMMA",

        // Operators
        "ASSIGNMENT_OPERATOR",
        "PLUS",
        "MINUS",
        "ASTERISK",
        "DIVIDE",
        "MODULO",
        "CARET",
        "LT",
        "GT",
        "LT_EQUAL",
        "GT_EQUAL",

        // Boolean operators
        "BOOLEAN_AND",
        "BOOLEAN_OR",
        "BOOLEAN_NOT",
        "BOOLEAN_EQUAL",
        "BOOLEAN_NOT_EQUAL",
        "BOOLEAN_TRUE",
        "BOOLEAN_FALSE",

        // Compound tokens
        "STRING",
        "DOUBLE_QUOTED_STRING",
        "SINGLE_QUOTED_STRING",
        "LETTER_UNDERSCORE",
        "LETTER_DIGIT_UNDERSCORE",
        "WHOLE_NUMBER",
        "INTEGER",
        "IDENTIFIER_TAIL",
        "IDENTIFIER",

        // Lists
        "IDENTIFIER_LIST",
        "IDENTIFIER_ARRAY_LIST",
        "IDENTIFIER_AND_IDENTIFIER_ARRAY_LIST",

        // Language keywords and types
        "DATATYPE_SPECIFIER",

        // Expressions
        "NUMERICAL_OPERAND",
        "NUMERICAL_OPERATOR",
        "BOOLEAN_OPERATOR",
        "EQUALITY_EXPRESSION",
        "RELATIONAL_EXPRESSION",
        "NUMERICAL_EXPRESSION",
        "BOOLEAN_EXPRESSION",
        "INITIALIZATION_EXPRESSION",
        "EXPRESSION",

        // Statements
        "SELECTION_STATEMENT",
        "ITERATION_STATEMENT",
        "ASSIGNMENT_STATEMENT",
        "PRINTF_STATEMENT",
        "GETCHAR_FUNCTION",
        "USER_DEFINED_FUNCTION",
        "DECLARATION_STATEMENT",
        "RETURN_STATEMENT",
        "STATEMENT",
        "COMPOUND_STATEMENT",
        "BLOCK_STATEMENT",

        // Function-related
        "PARAMETER_LIST",
        "FUNCTION_DECLARATION",
        "PROCEDURE_DECLARATION",
        "MAIN_PROCEDURE",

        // Program structure
        "PROGRAM",

        // BNF-specific tokens
        "DEFINITION",
        "ALTERNATION",

        // Special tokens
        "END_OF_FILE",
        "UNKNOWN"
    }};

    token(std::string contents, tokenType type) {
        tokenStr = contents;
        tType = type;
    }


    void printToken();



private:
    std::string tokenStr;
    tokenType tType;

};


#endif //CS460INTERPRETER_TOKEN_H
