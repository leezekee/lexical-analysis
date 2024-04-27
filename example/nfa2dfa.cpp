//
// Created by lizhi on 4/22/2024.
//
#include "DFA.h"

int main() {
    Regex regular = Regex::Regex("a((a|b)*ab|a*)b");
    std::cout << regular.get_suffix_expr() << std::endl;
    NFA nfa = NFA();
    nfa.from_regex(regular);
    nfa.to_graph();
    DFA dfa = DFA();
    dfa.from_nfa(nfa);
    std::cout << dfa << std::endl;
    dfa.to_graph();
    return 0;
}