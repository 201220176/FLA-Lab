#ifndef PARSER_H
#define PARSER_H

#include<iostream>
#include<string.h>
using namespace std;

enum ErrorType{
    CommandError,
    FileOpenError,
    DefError,
    illegalChar,
    UnknownState,
    TransferFuncError
};

bool parseCommand(int argc, char** argv, string& filename, string& input);



#endif