#include <bits/stdc++.h>

#include <utility>
#include "NFA.h"

using namespace std;

/*
    part 1 关于字符集的函数
    字符集有：数字、 26 个字母(大写小写)，为了检验程序的正确性，先只用"abcd"    
*/

set<char> createSet() {
    set<char> mySet;
    const string alphabet =
        "!%+,-./"
        "0123456789;<=>@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]_"
        "abcdefghijklmnopqrstuvwxyz{}";
    // const string alphabet = "abcd";
    for (char op : alphabet) {
        mySet.insert(op);
    }
    return mySet;
}
const set<char> ALPHABET = createSet();

bool isAlphabet(char c) {
    if (ALPHABET.count(c)) {
        return true;
    } else
        return false;
}

/*
    part 2 正则表达式的识别
    正则表达式支持符号：( and ) and | and * and &(通过一定方法识别并可以省略) 
*/
// priority of operators
int Priority(char c) {
    if (c == '|')
        return 1;
    else if (c == '&')
        return 2;
    else if (c == '*')
        return 3;
    else
        return -1;
}

// 正则表达式转后缀表达式(这个函数不够的地方是不能识别、返回一些输出的错误，因此必须保证正则表达式输出正确)
string Regex2Postfix(string regex) {
    // 加入&符号
    string regex2 = "";
    for (int i = 0; i < regex.size() - 1; i++) {
        if (regex[i] == '\\') {
            regex2 += regex2[i++];
            regex2 += regex2[i];
        } else if ((isAlphabet(regex[i]) && isAlphabet(regex[i + 1])) ||
                   (regex[i] == '*' && regex[i + 1] == '(') ||
                   (regex[i] == '*' && isAlphabet(regex[i + 1])) ||
                   (regex[i] == ')' && isAlphabet(regex[i + 1])) ||
                   (isAlphabet(regex[i]) && regex[i + 1] == '(') ||
                   (regex[i] == ')' && regex[i + 1] == '(')) {
            regex2 += regex[i];
            regex2 += '&';
        } else {
            regex2 += regex[i];
        }
    }
    regex2 += regex.back();

    // 将中缀转化成后缀
    string res = "";
    stack<char> ops;
    bool lastWasEscape = false;
    for (int i = 0; i < regex2.size(); i++) {
        if (lastWasEscape) {
            res += regex2[i];
            lastWasEscape = false;
            continue;
        }
        if (regex2[i] == '\\') {
            lastWasEscape = true;
            continue;
        }
        // number or letter, add it to res
        if (isAlphabet(regex2[i])) {
            res += regex2[i];
        }
        // operator
        else if (regex2[i] == '*' || regex2[i] == '|' || regex2[i] == '&') {
            while (!(ops.empty() || ops.top() == '(')) {
                if (Priority(ops.top()) >= Priority(regex2[i])) {
                    res += ops.top();
                    ops.pop();
                } else
                    break;
            }
            ops.push(regex2[i]);
        }
        // bracket
        else if (regex2[i] == '(') {
            ops.push(regex2[i]);
        } else if (regex2[i] == ')') {
            while (ops.top() != '(') {
                res += ops.top();
                ops.pop();
            }
            ops.pop();
        } else {
            cout << "REGULAR EXPRESSION ERROR!" << endl;
            break;
        }
    }
    // 剩余符号出栈
    while (!ops.empty()) {
        res += ops.top();
        ops.pop();
    }
    return res;
}

/*
    part 3 NFA的构建与转化
*/
class NFA {
   public:
    vector<map<char, vector<int>>> G;
    int start;
    int end;
    const char eps = '#';
    // default constructor
    NFA() {
        G.clear();
        start = 0;
        end = 1;
    }
    // constructor with an char as input
    NFA(char ch) {
        start = 0;
        end = 1;
        G.resize(2);
        G[start][ch] = vector<int>{end};
    }
    void print() const {
        cout << "start:" << start << " ";
        cout << "end:" << end << endl;
        // create edges
        set<char> edges = ALPHABET;
        edges.insert(eps);
        cout << "NFA transition table: \n";
        cout << left << setw(10) << "State";
        for (char c : edges)
            cout << setw(10) << c;
        cout << endl;
        for (int i = 0; i < G.size(); i++) {
            std::cout << std::setw(10) << i;
            for (char ch : edges) {
                if (!G[i].count(ch))
                    std::cout << std::setw(10) << "-";
                else {
                    std::string output = "";
                    for (const auto& s : G[i].at(ch)) {
                        output += std::to_string(s) +
                                  ((&s == &G[i].at(ch).back()) ? "" : ",");
                    }
                    std::cout << std::setw(10) << output;
                }
            }
            std::cout << std::endl;
        }
    }
    void Print2File() const {
        std::ofstream out("output.txt");    // 创建并打开文件

        // 使用文件流代替标准输出
        out << "start:" << start << " ";
        out << "end:" << end << std::endl;

        // 创建边缘集合
        std::set<char> edges = ALPHABET;
        edges.insert(eps);
        out << "NFA transition table: \n";
        out << std::left << std::setw(10) << "State";
        for (char c : edges)
            out << std::setw(10) << c;
        out << std::endl;

        for (int i = 0; i < G.size(); i++) {
            out << std::setw(10) << i;
            for (char ch : edges) {
                if (!G[i].count(ch))
                    out << std::setw(10) << "-";
                else {
                    std::string output = "";
                    for (const auto& s : G[i].at(ch)) {
                        output += std::to_string(s) +
                                  ((&s == &G[i].at(ch).back()) ? "" : ",");
                    }
                    out << std::setw(10) << output;
                }
            }
            out << std::endl;
        }

        out.close();    // 关闭文件
    }
    // NFA 的运算
    void OR(NFA t) {
        // 将 t 中图的连接添加到 s 中
        int offset = G.size();
        int t_size = t.G.size();
        for (int i = 0; i < t_size; i++) {
            G.push_back(map<char, vector<int>>{});
            // 将 t.G 中的所有元素复制到 s，并更改序号。当前s 中的序号为offset+i
            for (auto& item : t.G[i]) {    // item:map<char,vector>
                for (int to : item.second) {
                    G[offset + i][item.first].push_back(offset + to);
                }
            }
        }
        int newStart = G.size();
        int newEnd = G.size() + 1;
        G.push_back(map<char, vector<int>>{});
        G.push_back(map<char, vector<int>>{});
        G[newStart]['#'].push_back(start);
        G[newStart]['#'].push_back(t.start + offset);
        G[t.end + offset]['#'].push_back(newEnd);
        G[end]['#'].push_back(newEnd);
        start = newStart;
        end = newEnd;
    }
    void AND(NFA t) {
        int offset = G.size();
        int t_size = t.G.size();
        // 将当前NFA的结束状态通过空转换 ('#') 连接到被添加的NFA的开始状态。这里不添加新的状态，而是使用空转换连接现有状态
        if (t_size > 0) {
            G[end]['#'].push_back(offset + t.start);
        }
        for (int i = 0; i < t_size; i++) {
            G.push_back(map<char, vector<int>>{});
            for (auto& item : t.G[i]) {
                for (int to : item.second) {
                    G[offset + i][item.first].push_back(offset + to);
                }
            }
        }
        if (t_size > 0) {
            end = offset + t.end;
        }
    }
    void Closure() {
        int newStart = G.size();
        int newEnd = G.size() + 1;
        G.push_back(map<char, vector<int>>{});
        G.push_back(map<char, vector<int>>{});

        G[newStart]['#'].push_back(start);
        G[newStart]['#'].push_back(newEnd);
        G[end]['#'].push_back(start);
        G[end]['#'].push_back(newEnd);

        start = newStart;
        end = newEnd;
    }
};


// 将后缀表达式转化为 NFA
NFA Postfix2NFA(string regex) {
    stack<NFA> operands;
    for (int i = 0; i < regex.size(); i++) {
        if (isAlphabet(regex[i])) {
            operands.push(NFA(regex[i]));
        } else {
            switch (regex[i]) {
                case '|': {
                    NFA op1 = operands.top();
                    operands.pop();
                    NFA op2 = operands.top();
                    operands.pop();
                    op2.OR(op1);    // 注意顺序，op2 是先出栈的
                    operands.push(op2);
                    break;
                }
                case '*': {
                    NFA op = operands.top();
                    operands.pop();
                    op.Closure();    // 应用闭包
                    operands.push(op);
                    break;
                }
                case '&': {
                    NFA op1 = operands.top();
                    operands.pop();
                    NFA op2 = operands.top();
                    operands.pop();
                    op2.AND(op1);
                    operands.push(op2);
                    break;
                }
                default: {
                    cout << "Unsupported operation: " << regex[i] << endl;
                    break;
                }
            }
        }
    }
    // 最终，堆栈应该只有一个NFA，这就是后缀表达式代表的NFA
    if (!operands.empty()) {
        return operands.top();
    } else {
        // 如果堆栈为空，则返回一个空的NFA实例或者抛出一个错误
        // 这里返回空的NFA实例作为例子
        cout << "Error: Invalid regular expression." << endl;
        return NFA();    // 返回一个默认构造的NFA
    }
}
/*
    part 4 DFA的构建、从 NFA 转化为 DFA
*/
class DFA {
   public:
    vector<map<char, int>> transitionTable;    // DFA的转换表
    int startState;
    set<int> endStates;
    DFA() {
        startState = 0;    // 默认起始状态
    }

    // 添加转换
    void addTransition(int fromState, char inputChar, int toState) {
        if (transitionTable.size() <= fromState) {
            transitionTable.resize(fromState + 1);
        }
        transitionTable[fromState][inputChar] = toState;
    }
    void print() const {
        // cout << "size:" << transitionTable.size() << endl;
        cout << "START:" << startState << " " << endl;
        cout << "END:";
        for (int i : endStates) {
            cout << i << ", ";
        }
        cout << endl;
        // create edges
        cout << "DFA transition table: \n";
        cout << left << setw(10) << "State";
        for (char c : ALPHABET)
            cout << setw(10) << c;
        cout << endl;
        for (int i = 0; i < transitionTable.size(); i++) {
            std::cout << std::setw(10) << i;
            for (char ch : ALPHABET) {
                if (!transitionTable[i].count(ch))
                    std::cout << std::setw(10) << "-";
                else {
                    cout << setw(10) << transitionTable[i].at(ch);
                }
            }
            std::cout << std::endl;
        }
    }
    void Minimize();
    void rebuildDFA(const vector<set<int>>& partitions);
};

// ε闭包,输入为nfa对象、状态的集合
set<int> EpsClosure(const NFA& nfa, const set<int>& NFAstate) {
    // NFA 定义：vector<map<char, vector<int>>> G;
    queue<int> StateQueue;    // 用于存储要计算ε闭包的 NFA 状态
    set<int> closure;
    // 当前元素全部进入closure
    for (int state : NFAstate) {
        closure.insert(state);
        StateQueue.push(state);
    }
    // 查找当前元素的ε闭包
    while (!StateQueue.empty()) {
        int currentState = StateQueue.front();
        StateQueue.pop();

        if (nfa.G[currentState].count('#') != 0) {
            for (int nextState : nfa.G[currentState].at('#')) {
                // Only add the state to the queue if it's not already in the closure
                if (closure.insert(nextState).second) {
                    StateQueue.push(nextState);
                }
            }
        }
    }
    return closure;
}

// 从NFA转换到DFA
DFA ConvertNFAtoDFA(const NFA& nfa) {
    DFA dfa;
    map<set<int>, int> stateMapping;    // 用于保存NFA状态集合到DFA状态的映射
    queue<set<int>> stateQueue;    // 用于保存待处理的NFA状态集合

    // 计算NFA起始状态的ε闭包，并设置为DFA的起始状态
    set<int> startClosure = EpsClosure(nfa, {nfa.start});
    stateQueue.push(startClosure);
    stateMapping[startClosure] = 0;    // DFA的起始状态
    dfa.startState = 0;
    dfa.transitionTable.push_back({});    // 确保为起始状态分配空转换表

    while (!stateQueue.empty()) {
        set<int> currentState = stateQueue.front();
        stateQueue.pop();

        // 这里不再需要检查currentState是否已在stateMapping中，因为它一定在
        int dfaState = stateMapping[currentState];
        // 确保为每个DFA状态分配空转换表，即使它可能没有任何转换
        while (dfaState >= dfa.transitionTable.size()) {
            dfa.transitionTable.push_back({});
        }

        // 对于当前状态下，通过所有字符到达的新状态加入DFA的状态转换表中
        for (char c : ALPHABET) {
            set<int> newState;
            for (int state : currentState) {
                if (nfa.G[state].count(c)) {
                    for (auto nextState : nfa.G[state].at(c)) {
                        newState.insert(nextState);
                    }
                }
            }
            newState = EpsClosure(nfa, newState);

            // 检查这个新状态集合，为它分配一个新的DFA状态
            if (!newState.empty() &&
                stateMapping.find(newState) == stateMapping.end()) {
                int newDfaState = stateMapping.size();
                stateMapping[newState] = newDfaState;
                stateQueue.push(newState);
            }

            // 添加DFA的转换
            if (!newState.empty()) {
                dfa.addTransition(dfaState, c, stateMapping[newState]);
            }
        }
    }

    // NFA 里只有一个结束状态，因此可以根据stateMapping中的 key 找
    int NfaEndState = nfa.end;
    for (auto& item : stateMapping) {
        if (item.first.count(NfaEndState)) {
            dfa.endStates.insert(item.second);
        }
    }
    return dfa;
}

/*
    part 5 最小化 DFA
*/
void DFA::Minimize() {
    // 初始化分区：接受状态和非接受状态
    vector<set<int>> partitions = {endStates};    // 接受状态集
    set<int> nonAccepting;                        // 非接受状态集
    for (int i = 0; i < transitionTable.size(); ++i) {
        if (endStates.count(i) == 0) {
            nonAccepting.insert(i);
        }
    }

    if (!nonAccepting.empty()) {
        partitions.push_back(nonAccepting);
    }

    bool updated = true;    // 用于跟踪分区是否更新
    while (updated) {
        updated = false;                   // 假设没有更新发生
        vector<set<int>> newPartitions;    // 存储新的分区

        for (const auto& partition : partitions) {
            // 用于存储新的分区
            map<map<char, int>, set<int>> partitionMap;

            // 遍历当前分区中的每个状态,{1,2,3,4}
            for (int state : partition) {
                // 为状态创建转移映射
                map<char, int> transitions;
                for (const auto& trans : transitionTable[state]) {
                    // 查找状态转移所属的当前分区
                    int target = trans.second;
                    for (int p = 0; p < partitions.size(); ++p) {
                        if (partitions[p].count(target)) {
                            transitions[trans.first] =
                                p;    // 使用分区编号替代状态
                            break;
                        }
                    }
                }
                // 添加状态到对应的新分区
                partitionMap[transitions].insert(state);
            }

            // 检查是否有新的分区被创建（分区被细分）
            if (partitionMap.size() > 1)
                updated = true;
            // 添加新的分区
            for (const auto& p : partitionMap) {
                newPartitions.push_back(p.second);
            }
        }
        // 更新分区
        partitions = move(newPartitions);
    }

    // 重建DFA
    rebuildDFA(partitions);
}

void DFA::rebuildDFA(const vector<set<int>>& partitions) {
    // 新的转换表
    vector<map<char, int>> newTransitionTable(partitions.size());

    // 新的开始状态和结束状态
    int newStartState = -1;
    set<int> newEndStates;

    // 用于映射旧状态到新状态
    map<int, int> stateMap;

    // 遍历每个分区以构建新的状态映射
    for (int i = 0; i < partitions.size(); ++i) {
        for (int oldState : partitions[i]) {
            stateMap[oldState] = i;    // 映射旧状态到新的分区编号

            // 检查旧的开始状态是否在当前分区中
            if (oldState == startState) {
                newStartState = i;
            }
            // 检查旧的结束状态是否在当前分区中
            if (endStates.count(oldState)) {
                newEndStates.insert(i);
            }
        }
    }

    // 构建新的转换表
    for (int i = 0; i < partitions.size(); ++i) {
        // 取当前分区的一个代表状态（任何一个都可以）
        int representative = *partitions[i].begin();

        // 遍历该状态的所有转换
        for (const auto& trans : transitionTable[representative]) {
            char inputChar = trans.first;
            int targetState = trans.second;

            // 使用旧状态到新状态的映射来更新转换表
            newTransitionTable[i][inputChar] = stateMap[targetState];
        }
    }

    // 更新DFA
    transitionTable = move(newTransitionTable);
    startState = newStartState;
    endStates = move(newEndStates);
}

/*
    part 6 根据 minDFA 构建词法分析程序
*/
// 程序读入 input.txt，然后将分析结果写入 output.txt 中
// 如果到达 accepting 态，则根据状态输出当前 token 的类型
// 如果到达不存在的状态，则输出 error，并进行错误处理

bool Judge(DFA dfa, string tokens) {
    int nowState = dfa.startState;
    for (int i = 0; i < tokens.size(); i++) {
        if (dfa.transitionTable[nowState].count(tokens[i]) == 0)
            return false;    // 当前状态没有对应的转换
        int toState = dfa.transitionTable[nowState].at(tokens[i]);
        nowState = toState;
    }
    // 检查是否处于接受状态
    return dfa.endStates.count(nowState) != 0;
}
bool CheckLegalChar(string token) {
    // 查看是否有字母表中不支持的字符
    for (int i = 0; i < token.size(); i++) {
        if (!isAlphabet(token[i])) {
            return false;
        }
    }
    return true;
}
// 一个字符串开头是数字，后面是字母或数字，则判定
bool CheckIllegalIDs(string token) {
    std::regex pattern("^[0-9][a-zA-Z0-9_]*$");
    if (regex_match(token, pattern))
        return true;
    else
        return false;
}
bool CheckIllegalNum(string token) {
    std::regex pattern("^[0-9]+\\.$");
    if (regex_match(token, pattern))
        return true;
    else
        return false;
}
void LexicalAnalysis(const string& regexFileName, const string& codeFileName) {
    // get regexFile and form DFAs
    ifstream file(regexFileName);
    string KEYWORD, OPERATOR, CONST, DELIMITER, IDS;
    getline(file, KEYWORD);
    getline(file, OPERATOR);
    getline(file, CONST);
    getline(file, DELIMITER);
    getline(file, IDS);
    DFA KEYWORDdfa = ConvertNFAtoDFA(Postfix2NFA(Regex2Postfix(KEYWORD)));
    DFA OPERATORdfa = ConvertNFAtoDFA(Postfix2NFA(Regex2Postfix(OPERATOR)));
    DFA CONSTdfa = ConvertNFAtoDFA(Postfix2NFA(Regex2Postfix(CONST)));
    DFA DELIMITERdfa = ConvertNFAtoDFA(Postfix2NFA(Regex2Postfix(DELIMITER)));
    DFA IDSdfa = ConvertNFAtoDFA(Postfix2NFA(Regex2Postfix(IDS)));
    file.close();

    ifstream codeFile(codeFileName);
    ofstream out("output.txt");
    char ch;
    // 一次读入一个字符串
    string tokens = "";
    while (codeFile.get(ch)) {
        if (ch == '\n')
            continue;
        if (ch == ' ' || ch == '{' || ch == '}' || ch == '[' || ch == ']' ||
            ch == ',' || ch == ';') {
            // 如果 token 不为空，对该 token 进行处理
            if (!tokens.empty()) {
                bool match = false;
                if (Judge(KEYWORDdfa, tokens)) {
                    // cout << "Keyword:" << tokens << endl;
                    out << "Keyword: " << tokens << endl;
                    match = true;
                } else if (Judge(OPERATORdfa, tokens)) {
                    out << "Operator: " << tokens << endl;
                    match = true;
                } else if (Judge(IDSdfa, tokens)) {
                    out << "IDs: " << tokens << endl;
                    match = true;
                } else if (Judge(CONSTdfa, tokens)) {
                    out << "Const: " << tokens << endl;
                    match = true;
                }
                if (!match) {
                    if (!CheckLegalChar(tokens)) {
                        out << "Illegal character Exits in:" << tokens << endl;
                    } else if (CheckIllegalIDs(tokens)) {
                        out << "Illegal ID Exits in:" << tokens << endl;
                    } else if (CheckIllegalNum(tokens)) {
                        out << "Illegal Number Exits in:" << tokens << endl;
                    } else {
                        out << "Error Exits in:" << tokens << endl;
                    }
                }
                tokens = "";
            }
            if (ch != ' ')
                out << "Delimiter: " << ch << endl;
        } else {
            tokens += ch;
        }
    }
}

int main() {
    // LexicalAnalysis("regex.txt", "input.txt");
//    NFA nfa = Postfix2NFA(Regex2Postfix("(a|b)a*b*"));
//    DFA dfa = ConvertNFAtoDFA(nfa);
//
//    dfa.Minimize();
//    dfa.print();
    string regex = "void|char|int|float|double|struct|const|return|continue|break|if|else|switch|case|default|for|do|while";
    cout << Regex2Postfix(regex) << endl;
    return 0;
}
