#include"parser.h"

int main(int argc, char** argv) {
    string file, input;
    bool verbose = parseCommand(argc, argv, file, input);
    return 0;
}