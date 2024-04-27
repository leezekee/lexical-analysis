//
// Created by lizhi on 3/23/2024.
//

#ifndef LEXICAL_ANALYSIS_NFA_H
#define LEXICAL_ANALYSIS_NFA_H
#include "common.h"
#include "Regex.h"
#include "Graph.h"

class NFA {
private:
    ds::Inputs inputs;                                   // 可输入各种的字符
    ds::States states;                                    // 状态的编号
    ds::EndStates end_states;                                // 终止状态列表
    ds::NFATransitions transitions;   // 跳转hash表
    int start_state = -1;
public:
    NFA() = default;
    NFA(const Regex& regex);
    ds::Inputs get_inputs() { return inputs; }
    ds::States get_states() { return states; }
    ds::EndStates get_end_states() { return end_states; }
    ds::NFATransitions get_transitions() { return transitions; }
    int get_start_state() { return start_state; }
    void add_input(char input);
    void add_state(int state);
    void add_end_state(int state);
    void set_start_state(int state);
    void add_transition(int start, char input, int end);
    void from_regex(Regex regex);
    friend std::ostream &operator<< (std::ostream &os, NFA &nfa);
    void to_graph(const std::string&  expr_name);
};


#endif //LEXICAL_ANALYSIS_NFA_H
