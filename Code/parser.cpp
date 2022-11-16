#include"parser.h"

bool parseCommand(int argc, char** argv, string& file, string& input)
{
    int ErrorCode = -1;
    bool verbose = false;
    bool giveHelp = false;
    for(int i=1;i<argc;++i)
    {   
        string curArg = argv[i];
        if(curArg=="--help"||curArg=="-h")
            giveHelp = true;
        else if(!giveHelp&&(curArg=="--verbose"||curArg=="-v"))
            verbose = true;
        else if(!giveHelp&&file.empty()&&curArg.find(".tm"))
            file = curArg;
        else if(!giveHelp&&input.empty())
            input = curArg;
        else if(curArg[0]=='-')
            {
                ErrorCode=CommandError;
                cerr << "Unknown option: " << curArg << endl;
                exit(ErrorCode);
            }
        else
            {
                ErrorCode=CommandError;
                cerr << "Unknown command: " << curArg << endl;
                exit(ErrorCode);
            }
    }
    if(giveHelp)
        cout<<"usage: turing [-v|--verbose] [-h|--help] <tm> <input>"<<endl;
    return verbose;
}