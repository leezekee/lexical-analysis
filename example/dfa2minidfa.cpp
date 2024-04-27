//
// Created by lizhi on 4/22/2024.
//

#include <MinimizedDFA.h>
#include "utils.h"

int main() {
    Regex regular = Regex::from_expr("(a|b)a*b*");
    NFA nfa = NFA();
    nfa.from_regex(regular);
    nfa.to_graph("TEST");
    DFA dfa = DFA();
    dfa.from_nfa(nfa);
    std::cout << dfa << std::endl;
    dfa.to_graph("TEST");
    MinimizedDFA miniDFA = MinimizedDFA();
    miniDFA.from_dfa(dfa);
    std::cout << miniDFA << std::endl;
    miniDFA.to_graph("TEST");
    return 0;
}