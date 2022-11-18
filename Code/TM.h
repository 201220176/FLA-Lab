#ifndef TM_H
#define TM_H
#include<iostream>
#include<cstdlib>
#include<string.h>
#include<unordered_set>
#include<unordered_map>
#include <fstream>
#include<vector>

using namespace std;

#define errorBoundary   "==================== ERR ===================="
#define endBoundary     "==================== END ===================="
#define beginBoundary  "==================== RUN ===================="
#define Boundary              "---------------------------------------------"

struct pair_hash{
    template<class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

class TuringMachine{
private:
    bool verbose;
    string input;
    int numOfTape;
    unordered_set<string> state;
    unordered_set<string> finalState;
    unordered_set<string> inputChar;
    unordered_set<string> tapeChar;
    string startState;
    unordered_map<pair<string,string>,vector<string>,pair_hash> transferFunc;
    void checkInput(string &input);
    
public:
    TuringMachine(string filename,bool verbose);
    void run(string &input);
    void printResult();
    void errorReport(int errorType,int lineNum,vector<string>message);
};


//将形如{q1,q2}的字符串转为集合，当然也要检查是否格式错误
unordered_set<string> getSetFromValue(string value,TuringMachine* tm,char type,bool errorFlag,int lineNum);
//检查Q、S、G的token中是否都为允许的字符
void checkSymbol(string token,char type,TuringMachine *tm,int lineNum);


#endif