//
// Created by lizhi on 3/23/2024.
//

#include <queue>
#include <fstream>
#include "DFA.h"
#include "Exceptions.h"

int DFA::state_id = 0;

void DFA::add_input(char input) {
    if (std::find(inputs.begin(), inputs.end(), input) == inputs.end()) {
        inputs.push_back(input);
    }
}

void DFA::add_state(int state) {
    if (std::find(states.begin(), states.end(), state) == states.end()) {
        states.push_back(state);
    }
}

void DFA::add_end_state(int state) {
    if (std::find(end_states.begin(), end_states.end(), state) == end_states.end()) {
        end_states.push_back(state);
    }
}


void DFA::set_start_state(int state) {
    start_state = state;
}

void DFA::add_transition(int start, char input, int end) {
    add_state(start);
    add_state(end);
    add_input(input);
    if (transitions.find(start) == transitions.end()) {
        ds::DFATransEdge trans;
        trans[input] = end;
        transitions[start] = trans;
    } else {
        if (transitions[start].find(input) == transitions[start].end()) {
            transitions[start][input] = end;
        }
    }
}

ds::Closure DFA::get_closure(ds::NFATransitions nfa_transitions,
                               const ds::NFAStates& NFA_states) {
    // NFA 定义：vector<map<char, vector<int>>> G;
    ds::StateQueue state_queue;    // 用于存储要计算ε闭包的 NFA 状态
    ds::Closure closure;          // 用于存储计算出的ε闭包
    // 当前元素全部进入closure
    for (int NFA_state: NFA_states) {
        closure.insert(NFA_state);
        state_queue.push(NFA_state);
    }
    // 查找当前元素的ε闭包
    while (!state_queue.empty()) {
        int currentState = state_queue.front();
        state_queue.pop();
        if (nfa_transitions[currentState].count(EPSILON) != 0) {
            for (int nextState: nfa_transitions[currentState].at(EPSILON)) {
                if (closure.insert(nextState).second) {
                    state_queue.push(nextState);
                }
            }
        }
    }
    return closure;
}

void DFA::from_nfa(NFA &nfa) {

    ds::NFA2DFAReflections reflections;    // 用于保存NFA状态集合到DFA状态的映射
    ds::NFAStateQueue state_queue;    // 用于保存待处理的NFA状态集合

    auto dfa_transitions = nfa.get_transitions();

    ds::DFAState start = get_closure(dfa_transitions, {nfa.get_start_state()});
    reflections[start] = state_id++;
    state_queue.push(start);
    set_start_state(reflections[start]);

    while (!state_queue.empty()) {
        ds::DFAState currentState = state_queue.front();
        state_queue.pop();
        if (currentState.count(nfa.get_end_states()[0]) != 0) {
            add_end_state(reflections[currentState]);
        }
        ds::Inputs dfa_inputs = nfa.get_inputs();
        std::remove(dfa_inputs.begin(), dfa_inputs.end(), EPSILON);
        for (char input : dfa_inputs) {
            ds::DFAState nextState = {};
            for (int NFA_state: currentState) {
                if (nfa.get_transitions()[NFA_state].count(input) != 0) {
                    for (int nextNFAState: dfa_transitions[NFA_state].at(input)) {
                        nextState.insert(nextNFAState);
                    }
                }
            }
            if (nextState.empty()) {
                continue;
            }
            ds::DFAState closure = get_closure(dfa_transitions, nextState);
            if (reflections.count(closure) == 0) {
                reflections[closure] = state_id++;
                state_queue.push(closure);
            }
            add_transition(reflections[currentState], input, reflections[closure]);
        }
    }
}

std::ostream &operator<<(std::ostream &os, DFA &dfa) {
    os << "DFA {" << std::endl;
    os << "    " << "name: " << dfa.name << std::endl;
    os << "    " << "inputs: ";
    for (char input: dfa.inputs) {
        os << input << " ";
    }
    os << std::endl;
    os << "    " << "states: ";
    for (int state: dfa.states) {
        os << state << " ";
    }
    os << std::endl;
    os << "    " << "start state_id: " << dfa.start_state << std::endl;
    os << "    " << "end states: ";
    for (int state: dfa.end_states) {
        os << state << " ";
    }
    os << std::endl;
    os << "    " << "transitions: [" << std::endl;
    for (auto &trans: dfa.transitions) {
        int start = trans.first;
        auto edges = trans.second;
        for (auto &edge: edges) {
            char input = edge.first;
            auto end = edge.second;
            os << "        " << start << " -> " << input << " -> " << end << std::endl;

        }
    }
    os << "    ]\n}" << std::endl;
    return os;
}

void DFA::to_graph(const std::string& expr_name) {
    std::string dot_file_name;
    std::string graph_file_name;
    if (expr_name.empty()) {
        dot_file_name = R"(..\example\outputs\dot\DFA.dot)";
        graph_file_name = R"(..\example\outputs\graph\DFA.png)";
    } else {
        dot_file_name = R"(..\example\outputs\dot\)" + expr_name + "_DFA.dot";
        graph_file_name = R"(..\example\outputs\graph\)" + expr_name + "_DFA.png";
    }

    std::fstream fs = std::fstream(dot_file_name, std::ios::out);
    fs << "digraph " << expr_name << "_DFA {" << std::endl;
    fs << "rankdir=LR;" << std::endl;
    fs << "node [shape = circle];";
    bool start_flag = true;
    for (auto &trans: transitions) {
        int start = trans.first;
        auto edges = trans.second;
        for (auto &edge: edges) {
            char input = edge.first;
            auto end = edge.second;
            if (start_flag) {
                fs << "S ->" << start_state << ";" << std::endl;
                start_flag = false;
            }
            fs << start << " -> " << end << " [label=\"" << input << "\"];" << std::endl;
            if (std::find(end_states.begin(), end_states.end(), end) != end_states.end()) {
                fs << end << " [shape = doublecircle];" << std::endl;
            }
        }
    }
    fs << "}" << std::endl;
    fs.close();
    std::string command = R"(.\Graphviz\bin\dot.exe -Tpng )" + dot_file_name + " -o " + graph_file_name;
    system(command.c_str());
}

// todo 优化closure闭包的计算
ds::ClosureMap DFA::get_closure_map(ds::NFATransitions nfa_transitions, const ds::NFAStates &NFA_states) {
    return ds::ClosureMap();
}

DFA::DFA(NFA &nfa) {
    from_nfa(nfa);
}



