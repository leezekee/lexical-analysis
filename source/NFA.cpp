//
// Created by lizhi on 3/23/2024.
//

#include <fstream>
#include <utility>
#include "NFA.h"
#include "Exceptions.h"


void NFA::add_input(char input){
    if (std::find(inputs.begin(), inputs.end(), input) == inputs.end()) {
        inputs.push_back(input);
    }
}

void NFA::add_state(int state) {
    if (std::find(states.begin(), states.end(), state) == states.end()) {
        states.push_back(state);
    }
}

void NFA::add_end_state(int state) {
    if (std::find(end_states.begin(), end_states.end(), state) == end_states.end()) {
        end_states.push_back(state);
    }
}


void NFA::set_start_state(int state) {
    start_state = state;
}


void NFA::add_transition(int start, char input, int end) {
    add_state(start);
    add_state(end);
    add_input(input);
    if (transitions.find(start) == transitions.end()) {
        ds::NFATransEdge trans;
        trans[input] = {end};
        transitions[start] = trans;
    } else {
        if (transitions[start].find(input) == transitions[start].end()) {
            transitions[start][input] = {end};
        } else {
            transitions[start][input].push_back(end);
        }
    }
}

void NFA::from_regex(Regex regex) {
    ds::OpStack op_stack;
    ds::GraphStack graph_stack;
    for (char c : regex.get_suffix_expr()) {
        if (!Regex::_is_operator(c)) {
            Graph g = Graph(c);
            graph_stack.push(g);
        } else {
            if (c == '|') {
                if (graph_stack.size() < 2) {
                    throw InvalidRegexException();
                }
                Graph g2 = graph_stack.top();
                graph_stack.pop();
                Graph g1 = graph_stack.top();
                graph_stack.pop();
                graph_stack.push(Graph::union_(g1, g2));
            } else if (c == '&') {
                if (graph_stack.size() < 2) {
                    throw InvalidRegexException();
                }
                Graph g2 = graph_stack.top();
                graph_stack.pop();
                Graph g1 = graph_stack.top();
                graph_stack.pop();
                graph_stack.push(Graph::concat(g1, g2));
            } else if (c == '*') {
                if (graph_stack.empty()) {
                    throw InvalidRegexException();
                }
                Graph g = graph_stack.top();
                graph_stack.pop();
                graph_stack.push(Graph::closure(g));
            } else {
                throw InvalidRegexException();
            }
        }
    }
    if (graph_stack.size() != 1) {
        throw InvalidRegexException();
    }
    Graph g = graph_stack.top();
    graph_stack.pop();
    for (Edge edge : g.get_edges()) {
        add_transition(edge.from, edge.cond, edge.to);
    }
    add_end_state(g.get_end());
    set_start_state(g.get_start());
}

std::ostream &operator<<(std::ostream &os, NFA &nfa) {
    os << "NFA {" << std::endl;
    os << "    "  << "inputs: ";
    for (char input : nfa.inputs) {
        os << input << " ";
    }
    os << std::endl;
    os << "    "  << "states: ";
    for (int state : nfa.states) {
        os << state << " ";
    }
    os << std::endl;
    os << "    "  << "start state_id: " << nfa.start_state << std::endl;
    os << "    "  << "end states: ";
    for (int state : nfa.end_states) {
        os << state << " ";
    }
    os << std::endl;
    os << "    "  << "transitions: " << std::endl;
    for (auto &trans : nfa.transitions) {
        int start = trans.first;
        std::map<char, std::vector<int>> edges = trans.second;
        for (auto &edge : edges) {
            char input = edge.first;
            std::vector<int> ends = edge.second;
            for (int end : ends) {
                os << "        "  << start << " -> " << input << " -> " << end << std::endl;
            }
        }
    }
    std::cout << "}" << std::endl;
    return os;
}

void NFA::to_graph(const std::string&  expr_name) {
    std::string dot_file_name;
    std::string graph_file_name;
    if (expr_name.empty()) {
        dot_file_name = R"(..\example\outputs\dot\NFA.dot)";
        graph_file_name = R"(..\example\outputs\graph\NFA.png)";
    } else {
        dot_file_name = R"(..\example\outputs\dot\)" + expr_name + "_NFA.dot";
        graph_file_name = R"(..\example\outputs\graph\)" + expr_name + "_NFA.png";
    }

    std::fstream fs = std::fstream(dot_file_name, std::ios::out);
    fs << "digraph " << expr_name << "_NFA {" << std::endl;
    fs << "rankdir=LR;" << std::endl;
    fs << "node [shape = circle];";
    bool start_flag = true;
    for (auto &trans: transitions) {
        int start = trans.first;
        auto edges = trans.second;
        for (auto &edge: edges) {
            char input = edge.first;
            auto ends = edge.second;
            for (int end: ends) {
                if (start_flag) {
                    fs << "S ->" << start << std::endl;
                    start_flag = false;
                }
                if (input != EPSILON)
                    fs << start << " -> " << end << " [label = \"" << input << "\"];" << std::endl;
                else
                    fs << start << " -> " << end << " [label = \"ε\"];" << std::endl;

                if (std::find(end_states.begin(), end_states.end(), end) != end_states.end()) {
                    fs << end << " [shape = doublecircle];" << std::endl;
                }
            }
        }
    }
    fs << "}" << std::endl;
    fs.close();
    std::string command = R"(.\Graphviz\bin\dot.exe -Tpng )" + dot_file_name + " -o " + graph_file_name;
    system(command.c_str());
}

NFA::NFA(const Regex& regex) {
    from_regex(regex);
}


