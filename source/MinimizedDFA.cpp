//
// Created by lizhi on 4/22/2024.
//

#include "MinimizedDFA.h"
#include <queue>
#include <fstream>
#include "DFA.h"

int MinimizedDFA::state_id = 0;

ds::Partitions MinimizedDFA::get_partitions(DFA &dfa) {
    ds::EndStates end_states = dfa.get_end_states();
    ds::Inputs inputs = dfa.get_inputs();
    ds::DFATransitions transitions = dfa.get_transitions();

    ds::DFAState end_state_set(end_states.begin(), end_states.end());
    ds::Partitions partitions = {end_state_set};

    ds::Partition accept, non_accept;

    for (int state: dfa.get_states()) {
        if (end_state_set.find(state) == end_state_set.end()) {
            non_accept.insert(state);
        }
    }
    if (!non_accept.empty()) {
        partitions.push_back(non_accept);
    }

    bool updated = true;
    while(updated) {
        updated = false;
        ds::Partitions new_partitions;

        for (const auto &partition : partitions) {
            ds::PartitionMap partition_map;
            for (int state: partition) {
                ds::DFATransEdge trans_edge;
                for (const auto & trans : transitions[state]) {
                    int target = trans.second;
                    for (int p = 0; p < partitions.size(); p++) {
                        if (partitions[p].find(target) != partitions[p].end()) {
                            trans_edge[trans.first] = p;
                            break;
                        }
                    }
                }
                partition_map[trans_edge].insert(state);
            }

            if (partition_map.size() > 1) {
                updated = true;
            }

            for (const auto &part : partition_map) {
                new_partitions.push_back(part.second);
            }
        }

        partitions = std::move(new_partitions);
    }

    return partitions;
}


void MinimizedDFA::from_dfa(DFA &dfa) {
    ds::Partitions partitions = get_partitions(dfa);
    ds::EndStates dfa_end_states = dfa.get_end_states();
    int dfa_start_state = dfa.get_start_state();
    ds::DFA2MiniDFAReflections reflections;
    ds::DFATransitions dfa_transitions = dfa.get_transitions();

    for (int i = 0; i < partitions.size(); i++) {
        for (int old : partitions[i]) {
            reflections[old] = i;

            if (old == dfa_start_state) {
                set_start_state(i);
            }

            if (std::find(dfa_end_states.begin(), dfa_end_states.end(), old) != dfa_end_states.end()) {
                add_end_state(i);
            }
        }
    }

    for (int i = 0; i < partitions.size(); i++) {
        int representative = *partitions[i].begin();

        // 遍历该状态的所有转换
        for (const auto& trans : dfa_transitions[representative]) {
            char input = trans.first;
            int target = trans.second;

            // 使用旧状态到新状态的映射来建立转换表
            add_transition(i, input, reflections[target]);
        }
    }
}


std::ostream &operator<<(std::ostream &os, MinimizedDFA &mini_dfa) {
    os << "MinimizedDFA {" << std::endl;
    os << "    " << "name: " << mini_dfa.name << std::endl;
    os << "    " << "inputs: ";
    for (char input: mini_dfa.inputs) {
        os << input << " ";
    }
    os << std::endl;
    os << "    " << "states: ";
    for (int state: mini_dfa.states) {
        os << state << " ";
    }
    os << std::endl;
    os << "    " << "start_state_id: " << mini_dfa.start_state << std::endl;
    os << "    " << "end_states: ";
    for (int state: mini_dfa.end_states) {
        os << state << " ";
    }
    os << std::endl;
    os << "    " << "transitions: [" << std::endl;
    for (auto &trans: mini_dfa.transitions) {
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

// 生成图
void MinimizedDFA::to_graph(const std::string& expr_name) {
    std::string dot_file_name;
    std::string graph_file_name;
    if (expr_name.empty()) {
        dot_file_name = R"(..\example\outputs\dot\MinimizedDFA.dot)";
        graph_file_name = R"(..\example\outputs\graph\MinimizedDFA.png)";
    } else {
        dot_file_name = R"(..\example\outputs\dot\)" + expr_name + "_MinimizedDFA.dot";
        graph_file_name = R"(..\example\outputs\graph\)" + expr_name + "_MinimizedDFA.png";
    }

    std::fstream fs = std::fstream(dot_file_name, std::ios::out);
    fs << "digraph " << expr_name << "_MinimizedDFA {" << std::endl;
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

MinimizedDFA::MinimizedDFA(DFA dfa) {
    from_dfa(dfa);
}





