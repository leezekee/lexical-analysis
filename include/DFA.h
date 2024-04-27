//
// Created by lizhi on 3/23/2024.
//

#ifndef LEXICAL_ANALYSIS_DFA_H
#define LEXICAL_ANALYSIS_DFA_H

#include <set>
#include "NFA.h"

class DFA {
protected:
    static int state_id;
    ds::Inputs inputs;                                   // 可输入各种的字符
    ds::States states;                                    // 状态的编号
    ds::EndStates end_states;                                // 终止状态列表
    ds::DFATransitions transitions;                             // 跳转hash表
    std::string name;
    int start_state = -1;
public:
    DFA() = default;
    DFA(NFA& nfa);
    void add_input(char input);
    void add_state(int state);
    void set_start_state(int state);
    void add_end_state(int state);
    void add_transition(int start, char input, int end);
    static ds::Closure get_closure(ds::NFATransitions nfa_transitions, const ds::NFAStates& NFA_states);
    static ds::ClosureMap get_closure_map(ds::NFATransitions nfa_transitions, const ds::NFAStates& NFA_states);
    void from_nfa(NFA &nfa);
    friend std::ostream &operator<< (std::ostream &os, DFA &dfa);
    virtual void to_graph(const std::string& expr_name);
    ds::Inputs get_inputs() { return inputs; }
    ds::States get_states() { return states; }
    ds::EndStates get_end_states() { return end_states; }
    ds::DFATransitions get_transitions() { return transitions; }
    int get_start_state() { return start_state; }

    virtual void set_name(const std::string& _name) { this->name = _name; }

    virtual std::string get_name() { return name; }
};


#endif //LEXICAL_ANALYSIS_DFA_H
