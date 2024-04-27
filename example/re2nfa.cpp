//
// Created by lizhi on 4/22/2024.
//

#include "NFA.h"

int main() {
    Regex regular = Regex::from_expr("a((a|b)*ab|a*)b");
    std::cout << "Regex Expression: " << regular.get_raw_expr() << std::endl;
    NFA nfa = NFA();
    nfa.from_regex(regular);
    std::cout << nfa << std::endl;
    return 0;
}