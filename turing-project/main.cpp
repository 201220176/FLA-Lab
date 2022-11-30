#include"parser.h"
#include"TM.h"

int main(int argc, char** argv) {
    string filename, input;
    bool verbose = parseCommand(argc, argv, filename, input);
    if(filename=="")
        return 0;
    TuringMachine *tm = new TuringMachine(filename,verbose);
    tm -> run(input);
    return 0;
}