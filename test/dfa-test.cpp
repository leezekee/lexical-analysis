//
// Created by lizhi on 4/22/2024.
//
//
// Created by lizhi on 4/10/2024.
//

#include "DFA.h"

int main() {
    Regex regular = Regex::Regex("a((a|b)*ab|a*)b");
    std::cout << regular.get_suffix_expr() << std::endl;
    NFA nfa = NFA();
    nfa.from_regex(regular);
    DFA dfa = DFA();
    dfa.from_nfa(nfa);
    std::cout << dfa << std::endl;
    return 0;
}