//
// Created by lizhi on 3/23/2024.
//

#include "Regex.h"
#include "Exceptions.h"

Regex::Regex(std::string expression) {
    _raw_expr = std::move(expression);
    // add '&' when necessary
    std::string new_expr;
    for (int i = 0; i < _raw_expr.size() - 1; i++) {
        if (_raw_expr[i] == '\\') {
            new_expr += new_expr[i++];
            new_expr += new_expr[i];
        } else if ((_in_alphabet(_raw_expr[i]) && _in_alphabet(_raw_expr[i + 1])) ||
                   (_raw_expr[i] == '*' && _raw_expr[i + 1] == '(') ||
                   (_raw_expr[i] == '*' && _in_alphabet(_raw_expr[i + 1])) ||
                   (_raw_expr[i] == ')' && _in_alphabet(_raw_expr[i + 1])) ||
                   (_in_alphabet(_raw_expr[i]) && _raw_expr[i + 1] == '(') ||
                   (_raw_expr[i] == ')' && _raw_expr[i + 1] == '(')) {
            new_expr += _raw_expr[i];
            new_expr += '&';
        } else {
            new_expr += _raw_expr[i];
        }
    }
    new_expr += _raw_expr.back();

    _expr = std::move(new_expr);
}

void Regex::_convert() {
    // change expression into suffix expression
    std::stack<char> op_stack;
    std::string suffix_expr;
    bool last_was_escape = false;
    for (char _c : _expr) {
        if (last_was_escape) {
            suffix_expr += _c;
            last_was_escape = false;
            continue;
        }
        if (_c == '\\') {
            last_was_escape = true;
            continue;
        }
        if (_is_operator(_c)) {
            if (op_stack.empty() || _c == '(') {
                op_stack.push(_c);
            } else if (_c == ')') {
                if (op_stack.empty()) {
                    throw InvalidRegexException();
                }
                while (op_stack.top() != '(') {
                    suffix_expr += op_stack.top();
                    op_stack.pop();
                }
                op_stack.pop();
            } else {
                while (!op_stack.empty() && op_stack.top() != '('
                        && _priority(op_stack.top()) >= _priority(_c)) {
                    suffix_expr += op_stack.top();
                    op_stack.pop();
                }
                op_stack.push(_c);
            }
        } else if (_in_alphabet(_c)) {
            suffix_expr += _c;
        } else {
            throw InvalidRegexException();
        }
    }
    while (!op_stack.empty()) {
        suffix_expr += op_stack.top();
        op_stack.pop();
    }
    _suffix_expr = std::move(suffix_expr);
}

Regex Regex::from_expr(std::string expr) {
    Regex regular(std::move(expr));
    regular._convert();
    return regular;
}

int Regex::_priority(char c) {
    // get the priority of operator c
    if (c == '|') {
        return 1;
    } else if (c == '*') {
        return 3;
    } else if (c == '(' || c == ')') {
        return 4;
    } else if (c == '&') {
        return 2;
    }
    return 0;
}

bool Regex::_is_operator(char c) {
    // check if c is an operator
    if (regular_operator.end() != std::find(regular_operator.begin(), regular_operator.end(), c)) {
        return true;
    }
    return false;
}

bool Regex::_add_concat(std::string &expression, int index) {
    if (index == expression.size() - 1) {
        return false;
    }
    if (_is_letter(expression[index]) && _is_letter(expression[index + 1])) {
        return true;
    }
    if (_is_letter(expression[index]) && expression[index + 1] == '(') {
        return true;
    }
    if (expression[index] == ')' && _is_letter(expression[index + 1])) {
        return true;
    }
    if (expression[index] == '*' && _is_letter(expression[index + 1])) {
        return true;
    }
    if (expression[index] == ')' && expression[index + 1] == '(') {
        return true;
    }
    if (expression[index] == '*' && expression[index + 1] == '(') {
        return true;
    }
    return false;
}

bool Regex::_is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Regex::_in_alphabet(char c) {
    return alphabet.find(c) != std::string::npos;
}
