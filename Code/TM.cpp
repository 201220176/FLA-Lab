#include"TM.h"
#include"parser.h"

TuringMachine::TuringMachine(string filename,bool verbose)
{
    this->verbose=verbose;
    ifstream fin(filename);

    int lineNum = 1;
    if(!fin.is_open())
        {
            errorReport(FileOpenError,-1,{"Cannot open file"});
            return;
        }
    else
        {
            string line;
            while(getline(fin,line))
            {
                //去除空行、首尾空格和注释
                {
                    int pos = line.find(';');
                    if (pos != line.npos)
                        line = line.erase(pos);
                    line.erase(0,line.find_first_not_of(" "));
                    line.erase(line.find_last_not_of(" ")+1);
                    if(line.empty())
                    {
                        ++lineNum;
                        continue;
                    }
                }
                if(line[0]=='#')
                {
                    //形如 #left = right,检查格式是否正确，并取出right
                    string value;
                    {
                        int assignPos = line.find_first_of('=');
                        if(assignPos==line.npos)
                            {
                                //缺少等号
                                errorReport(DefError,lineNum,{"Missing <=>"});
                                return;
                            }
                        if(line[assignPos-1]!=' '||assignPos+1>=line.size()||line[assignPos+1]!=' ')
                            {
                                //等号两边无空格
                                errorReport(DefError,lineNum,{"Missing space beside <=>"});
                                return;
                            }
                        value = line.substr(assignPos+2);
                        if(value.empty())
                            {
                                errorReport(DefError,lineNum,{"Missing value at the right of <=>"});
                                return;
                            }
                    }
                    //检查相关赋值
                    switch (line[1])
                    {
                    case 'Q':
                        {
                        bool errorFlag = false;
                        state = getSetFromValue(value,this,'Q',errorFlag,lineNum);
                        if(errorFlag)
                            return;
                        }
                        break;
                    case 'S':
                        {
                        bool errorFlag = false;
                        inputChar = getSetFromValue(value,this,'S',errorFlag,lineNum);
                        if(errorFlag)
                            return;
                        }
                        break;
                    case 'G':
                        {
                        bool errorFlag = false;
                        tapeChar = getSetFromValue(value,this,'G',errorFlag,lineNum);
                        if(errorFlag)
                            return;
                        }
                        break;
                    case 'q':
                        if(line[2]!='0')
                            {
                                errorReport(DefError,lineNum,{"Wrong target at the left of <=>",": ",line.substr(1,2)});
                                return;
                            }
                        else if(state.find(value)==state.end())
                            {
                                errorReport(UnknownState,lineNum,{"Unknown state:",value});
                                return;
                            }
                        else
                            startState=value;
                        break;
                     case 'B':
                        if(value!="_")
                            {
                                errorReport(illegalChar,lineNum,{"Wrong blank symbol:",value});
                                return;
                            }
                        break;
                     case 'F':
                        {
                        bool errorFlag = false;
                        finalState = getSetFromValue(value,this,'F',errorFlag,lineNum);
                        if(errorFlag)
                            return;
                        for(string s:finalState)
                            if(state.find(s)==state.end())
                                {
                                    errorReport(UnknownState,lineNum,{"Unknown state:",s});
                                    return;
                                }
                        }
                        break;
                     case 'N':
                        {
                        for(char tmp:value)
                            {
                                //不是纯整数
                                if(tmp<48||tmp>57)
                                {
                                    errorReport(DefError,lineNum,{"TapeNum error:",value});
                                    return;
                                }
                            }
                            numOfTape=stoi(value);
                        }
                        break;
                    default:
                        errorReport(DefError,lineNum,{"Wrong target at the left of <=>",": ",line});
                        return;
                        break;
                    }
                }
                //转换函数
                else
                {
                    //<旧状态> <旧符号组> <新符号组> <方向组> <新状态>
                    vector<string> list;
                    int left =0,right = line.find(' ');
                    while(right!=line.npos)
                        {
                            list.push_back(line.substr(left,right-left));
                            left=right+1;
                            right=line.find(' ',left);
                        }
                    if(!line.substr(left).empty())
                        list.push_back(line.substr(left));
                    if(list.size()!=5)
                        {
                            errorReport(TransferFuncError,lineNum,{"Wrong num of tokens where transfer Func:"});
                            return;
                        }
                    if(state.find(list[0])==state.end()||state.find(list[4])==state.end())
                        {
                            errorReport(UnknownState,lineNum,{"Unknow State in following:",list[0],list[4]});
                            return;
                        }
                    if(list[1].size()!=numOfTape||list[2].size()!=numOfTape||list[3].size()!=numOfTape)
                        {
                            errorReport(TransferFuncError,lineNum,{"Cannot match tapeNum",list[1],list[2],list[3]});
                            return;
                        }
                    for(int i =0;i<list[1].size();++i)
                        if(list[1][i]!='*'&&tapeChar.find(list[1].substr(i,1))==tapeChar.end())
                            {
                                string ch ="";
                                ch+= list[1][i];
                                errorReport(illegalChar,lineNum,{"illegalChar in old str:",ch});
                                return;
                            }
                    for(int i =0;i<list[2].size();++i)
                        if(list[2][i]!='*'&&tapeChar.find(list[2].substr(i,1))==tapeChar.end())
                            {
                                string ch ="";
                                ch+= list[2][i];
                                errorReport(illegalChar,lineNum,{"illegalChar in new str:",ch});
                                return;
                            }
                    for(int i =0;i<list[3].size();++i)
                        if(list[3][i]!='l'&&list[3][i]!='r'&&list[3][i]!='*')
                            {
                                string ch ="";
                                ch+= list[3][i];
                                errorReport(illegalChar,lineNum,{"illegalChar in direct:",ch});
                                return;
                            }
                    transferFunc[make_pair(list[0],list[1])]=list;
                }
                ++lineNum;
            }
        }
}
void TuringMachine::errorReport(int errorType,int lineNum,vector<string>message)
{
    if(verbose)
    {
        switch ((errorType))
        {
        case FileOpenError:
            cerr<<"Cannot open file"<<endl;
            break;
        case InputError:
            {
                int index = stoi(message[0]);
                cerr<<errorBoundary<<endl
                        <<"error: '"<<this->input[index]<<"'was not declared in the set of input symbols"<<endl
                        <<"input: "<<this->input<<endl
                        <<"       ";
                for(int i=0;i<index;++i)    cerr<<" ";
                cerr<<'^'<<endl<<endBoundary<<endl;
            }
            break;
        default:
            cerr<<errorBoundary<<endl
                    <<"Wrong line:"<<lineNum<<" ";
            for(string s:message)
                cerr<<s;
            cerr<<endl<<endBoundary<<endl;
            break;
        }
    }
    else
    {
        switch ((errorType))
        {
        case FileOpenError:
            cerr<<"Cannot open file"<<endl;
            break;
        case InputError:
            cerr<<"illegal input"<<endl;
        default:
            cerr<<"syntax error"<<endl;
            break;
        }
    }
    exit(errorType);
}
void TuringMachine::run(string& input)
{
    this->input=input;
    checkInput(input);
}
void TuringMachine::printResult()
{

}

void TuringMachine::checkInput(string &input)
{
    if(verbose)
        cerr<<"Input: "<<input<<endl;
    for(int i=0;i<input.size();++i)
    {
        if(inputChar.find(input.substr(i,1))==inputChar.end())
            {
                errorReport(InputError,-1,{to_string(i)});
            }
    }
    if(verbose)
        cerr<<beginBoundary<<endl;
}

unordered_set<string> getSetFromValue(string value,TuringMachine* tm,char type,bool errorFlag,int lineNum)
{
    if(value[0]!='{'||value[value.size()-1]!='}')
        {
            tm->errorReport(DefError,lineNum,{"Missing <{> or <}>"});
        }
    unordered_set<string> result;
    int left=1,right = value.find_first_of(',');
    if(right==value.npos)
        right=value.find_first_of('}');
    while(right!=value.npos)
    {
        string item = value.substr(left,right-left);
        checkSymbol(item,type,tm,lineNum);
        if(item.size()>1&&(type=='S'||type=='G'))
            {
                tm->errorReport(illegalChar,lineNum,{"illegalChar:",item});
                return result;
            }
        if(item.empty())
            {
                tm->errorReport(illegalChar,lineNum,{"No str between two <,>"});
                return result;
            }
        result.insert(item);
        left = right+1;
        right=value.find(',',left);
        if(right==value.npos)
            right=value.find('}',left);
    }
}

void checkSymbol(string token,char type,TuringMachine *tm,int lineNum)
{
    for(char c:token)
    {
        switch (type)
        {
        case 'Q':
            if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_')
                continue;
            else 
                {
                    string ch ="";ch+=c;
                    tm->errorReport(illegalChar,lineNum,{"illegalChar:",ch});
                }
            break;
        case 'S':
            {
                string prohibition = " ,;{}*_";
                if(c>=32&&c<=126&&prohibition.find(c)==prohibition.npos)
                    continue;
                else
                    {
                        string ch ="";ch+=c;
                        tm->errorReport(illegalChar,lineNum,{"illegalChar:",ch});
                    }
            }
            break;
        case 'G':
            {
                string prohibition = " ,;{}*";
                if(c>=32&&c<=126&&prohibition.find(c)==prohibition.npos)
                    continue;
                else
                    {
                        string ch ="";ch+=c;
                        tm->errorReport(illegalChar,lineNum,{"illegalChar:",ch});
                    }
            }
            break;
        default:
            return;
            break;
        }
    }
}
