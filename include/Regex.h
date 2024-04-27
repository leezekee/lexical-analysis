//
// Created by lizhi on 3/23/2024.
//

#ifndef LEXICAL_ANALYSIS_REGEX_H
#define LEXICAL_ANALYSIS_REGEX_H
#include "common.h"

class Regex {
private:
    std::string _raw_expr;
    std::string _expr;
    std::string _suffix_expr;
private:
    void _convert();
    static bool _add_concat(std::string &expression, int i);
    static int _priority(char c);
public:
    static bool _is_operator(char c);
    static bool _is_letter(char c);
    static bool _in_alphabet(char c);

    explicit Regex(std::string expression = "");

public:
    static Regex from_expr(std::string expr);
    std::string get_suffix_expr() { return _suffix_expr; }
    std::string get_expr() { return _expr; }
    std::string get_raw_expr() { return _raw_expr; }
};


#endif //LEXICAL_ANALYSIS_REGEX_H
