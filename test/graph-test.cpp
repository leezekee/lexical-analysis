//
// Created by lizhi on 4/10/2024.
//

#include "NFA.h"

int main() {
    Regex regular = Regex::Regex("a((a|b)*ab|a*)b");
    std::cout << regular.get_suffix_expr() << std::endl;
    NFA nfa = NFA();
    nfa.from_regex(regular);
    std::cout << nfa << std::endl;
    return 0;
}