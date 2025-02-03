//#pragma once
//#ifndef TOKENS_H
//#define TOKENS_H
//
//#include <map>
//#include <string>
//
//std::map<std::string, std::string> tokenTypes = {
//    {"{", "OPEN_BRACE"},
//    {"}", "CLOSE_BRACE"},
//    {"(", "OPEN_PAREN"},
//    {")", "CLOSE_PAREN"},
//    {"<", "LESS_THAN"},
//    {">", "GREATER_THAN"},
//    {"<<", "OPERATOR"},
//    {">>", "OPERATOR"},
//    {"%", "PERCENT"},
//    {":", "COLON"},
//    {";", "SEMICOLON"},
//    {".", "DOT"},
//    {"?", "QUESTION_MARK"},
//    {"*", "ASTERISK"},
//    {"+", "PLUS"},
//    {"-", "MINUS"},
//    {"^", "CARET"},
//    {"/", "FORWARD_SLASH"},
//    {"&", "AMPERSAND"},
//    {"|", "PIPE"},
//    {"~", "TILDE"},
//    {"!", "EXCLAMATION_MARK"},
//    {"=", "EQUALS"},
//    {"[]", "SQUARE_BRACKETS"},
//    {"\".*\"", "STRING_LITERAL"},
//    {"#include <iostream>", "LIBRARY"},
//    {"#include <fstream>", "LIBRARY"},
//    {"#include <string>", "LIBRARY"},
//    {"#include <regex>", "LIBRARY"},
//    {"tokens.h", "Class File"},
//    {"include", "INCLUDE"},
//    {"using", "USING"},
//    {"namespace", "NAMESPACE"},
//    {"std", "STD"},
//    {"int", "INT"},
//    {"char", "CHAR"},
//    {"double", "DOUBLE"},
//    {"float", "FLOAT"},
//    {"bool", "BOOL"},
//    {"void", "VOID"},
//    {"main", "MAIN"},
//    {"cout", "COUT"},
//    {"if", "IF"},
//    {"else", "ELSE"},
//    {"while", "WHILE"},
//    {"for", "FOR"},
//    {"return", "RETURN"},
//    {"0", "INTEGER_LITERAL"},
//    {"\\d+", "INTEGER_LITERAL"}, // matches integers
//    {"\\w+", "IDENTIFIER"} // matches words
//    // add nalang kayo if need pa ng keywords or what
//};
//
//#endif // TOKENS_H


#pragma once
#pragma once
#ifndef TOKENS_H
#define TOKENS_H

#include <map>
#include <string>

std::map<std::string, std::string> tokenTypes = {
    {"#include <iostream>", "PREPROCESSOR"},
    {"using namespace std", "NAMESPACE"},
    {"\\d+", "INTEGER_LITERAL"}, // matches integers
    {"\"[^\"]*\"", "STRING"},
    {"<<", "OPERATOR"},
    {">>", "OPERATOR"},
    {"[]", "SYMBOL"},
    {"{", "SYMBOL"},
    {"}", "SYMBOL"},
    {"()", "SYMBOL"},
    {"(", "SYMBOL"},
    {")", "SYMBOL"},
    {"<", "OPERATOR"},
    {"0", "INTEGER_LITERAL"},
    {">", "OPERATOR"},
    {"%", "OPERATOR"},
    {":", "SYMBOL"},
    {";", "SYMBOL"},
    {".", "SYMBOL"},
    {"?", "QUESTION_MARK"},
    {"*", "OPERATOR"},
    {"+", "OPERATOR"},
    {"-", "OPERATOR"},
    {"^", "OPERATOR"},
    {"/", "OPERATOR"},
    {"&", "OPERATOR"},
    {"|", "OPERATOR"},
    {"~", "OPERATOR"},
    {"!", "OPERATOR"},
    {"=", "OPERATOR"},
    {"//", "COMMENTS"},
    {",", "SYMBOL"},
    {"include", "INCLUDE"},
    {"using", "USING"},
    {"namespace", "NAMESPACE"},
    {"std", "STD"},
    {"int", "KEYWORD"},
    {"char", "KEYWORD"},
    {"double", "KEYWORD"},
    {"float", "KEYWORD"},
    {"bool", "KEYWORD"},
    {"void", "KEYWORD"},
    {"std::cout", "IDENTIFIER"},
    {"std::cin", "IDENTIFIER"},
    {"main", "FUNCTION_NAME"},
    {"cout", "IDENTIFIER"},
    {"cin" , "IDENTIFIER"},
    {"if", "IF"},
    {"else", "ELSE"},
    {"while", "WHILE"},
    {"for", "FOR"},
    {"return", "KEYWORD"},
    {"\\b[a-zA-Z_][a-zA-Z0-9_]*\\b", "VARIABLE"},
    {"first_number", "VARIABLE"},
    {"second_number", "VARIABLE"},
    {"sum", "VARIABLE"},

};



#endif // TOKENS_H