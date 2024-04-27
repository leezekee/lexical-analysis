//
// Created by lizhi on 3/23/2024.
//

#ifndef LEXICAL_ANALYSIS_COMMON_H
#define LEXICAL_ANALYSIS_COMMON_H

#include <utility>
#include <algorithm>
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <map>
#include <queue>
#include "set"
#include "Graph.h"

#define EPSILON '#' // 空字符


const std::vector<char> regular_operator = {'(', ')', '*', '|', '&'};

const std::string alphabet =
        "!%+,-./"
        "0123456789;<=>@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]_"
        "abcdefghijklmnopqrstuvwxyz{}";

namespace ds {
    using Inputs = std::vector<char>;
    using States = std::vector<int>;
    using EndStates = std::vector<int>;
    using NFATransitions = std::map<int, std::map<char, std::vector<int>>>;
    using DFATransitions = std::map<int, std::map<char, int>>;
    using OpStack = std::stack<char>;
    using GraphStack = std::stack<Graph>;
    using Closure = std::set<int>;
    using NFAStates = std::set<int>;
    using StateQueue = std::queue<int>;
    using DFAState = std::set<int>;
    using NFAStateQueue = std::queue<ds::DFAState>;
    using NFATransEdge = std::map<char, std::vector<int>>;
    using DFATransEdge = std::map<char, int>;
    using Partitions = std::vector<std::set<int>>;
    using Partition = std::set<int>;
    using NFA2DFAReflections = std::map<ds::DFAState, int, std::less<>>;
    using PartitionMap = std::map<ds::DFATransEdge, std::set<int>>;
    using DFA2MiniDFAReflections = std::map<int, int>;
    using ClosureMap = std::map<char, ds::Closure>;
}


#endif //LEXICAL_ANALYSIS_COMMON_H
