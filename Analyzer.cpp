#include <stdio.h>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Token.cpp"
#include "SyntaxParser.cpp"

using namespace std;



vector<Token*>tokens;


// given file name, open file and write tokens in file to token vector
vector<Token*> getTokens(const char* fileName){
    vector<Token*>v;

    std::ifstream file(fileName);
    std::string str; 
    while (std::getline(file, str))
    {
        Token *t = new Token;

        string buf; // Have a buffer string
        stringstream ss(str); // Insert the string into a stream
        ss >> buf; // get token type
        
        t->token_type = buf;
        ss >> buf; // get lexeme
        t->lexeme = buf;
        v.push_back(t);
    }
    return v;
}



int main(int argc, char* argv[])
{
    char const* const fileName = "lo";
    tokens = getTokens(fileName); // read in tokens from lexed file 
    SyntaxParser syntaxParser = SyntaxParser(tokens); // init parser with tokens
    std::vector<StatementHolder*> AST = syntaxParser.getAST(); // generate syntax tree
    return 0;
}