#ifndef PARSER_H
#define PARSER_H

#include<iostream>
#include<string.h>
using namespace std;

enum ErrorType{
    CommandError
};

bool parseCommand(int argc, char** argv, string& file, string& input);



#endif