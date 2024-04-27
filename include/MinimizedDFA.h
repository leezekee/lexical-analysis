//
// Created by lizhi on 4/22/2024.
//

#ifndef LEXICAL_ANALYSIS_MINDFA_H
#define LEXICAL_ANALYSIS_MINDFA_H

#include "DFA.h"

#include <utility>

class MinimizedDFA : public DFA {
private:
    static int state_id;
    std::string name;
public:
    MinimizedDFA() : DFA() {}

    MinimizedDFA(DFA dfa);

    MinimizedDFA(std::string _name, ds::Inputs _inputs, ds::States _states,
                 ds::States _end_states, int _start_state) {
        name = std::move(_name);
        inputs = std::move(_inputs);
        states = std::move(_states);
        end_states = std::move(_end_states);
        start_state = _start_state;
    }

    static ds::Partitions get_partitions(DFA &dfa);

    void from_dfa(DFA &dfa);

    friend std::ostream &operator<<(std::ostream &os, MinimizedDFA &mini_dfa);

    void to_graph(const std::string &expr_name);

    void set_name(const std::string &name) override { this->name = name; }

    std::string get_name() override { return name; }
};


#endif //LEXICAL_ANALYSIS_MINDFA_H
