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
    unordered_set<string> state;    //状态集Q
    unordered_set<string> finalState;   //接收状态集F
    unordered_set<string> inputChar;    //输入符号集S
    unordered_set<string> tapeChar;     //纸袋符号集G
    string startState;  //开始状态
    string curState;    //当前状态
    unordered_map<pair<int,int>,char,pair_hash> tape;     //根据(磁道数,下标)来读取符号
    vector<int>head;   //磁头指向下标
    int step;   //记录移动步数
    vector<pair<int,int>> limit;    //维护所有磁道的左右边界,左闭右开，如(0,0)代表全空。注意会受到head的影响
    unordered_map<pair<string,string>,vector<string>,pair_hash> transferFunc;   //转移函数,<当前状态，当前符号组>为key,<新符号组，方向组，新状态>为value.
    void checkInput(string &input);
    //根据下标，来获取所有磁带符号
    string getTapestr(vector<int>index);   
    //(磁带数，下标)获取符号
    char getTapechar(pair<int,int>);
    //打印当前步骤
    void printCurStep();
    //写磁带并且移动读写头<新符号组，方向组，新状态>
    void wirteAndmove(vector<string>& tokens);
    //check字符串是否相同，str2中有通配符"*"
    bool checkMatch(string str1,string str2);
    
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