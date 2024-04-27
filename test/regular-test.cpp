//
// Created by lizhi on 3/23/2024.
//
#include "Regex.h"
#include "iostream"

int main() {
    Regex regular = Regex::Regex("a((a|b)*ab|a*)b");
    std::cout << regular.get_raw_expr() << std::endl;
    std::cout << regular.get_expr() << std::endl;
    std::cout << regular.get_suffix_expr() << std::endl;
    return 0;
}