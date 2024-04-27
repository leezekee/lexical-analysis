//
// Created by lizhi on 4/25/2024.
//

#include <fstream>
#include <regex>
#include "utils.h"


/**
 * Convert regular expression to (minimized) DFA
 * @param expr regular expression
 * @param output whether to output the graphs of NFA and DFA
 * @param detail whether to output information of NFA and DFA in the console
 * @return (minimized) DFA
 */
MinimizedDFA la::utils::reg2dfa(const std::string &expr_name, std::string expr, bool output, bool detail) {
    std::cout << "Converting regex '" << expr_name << "' to (minimized) DFA..." << std::endl;
    Regex regex = Regex::from_expr(std::move(expr));
    NFA nfa = NFA(regex);
    DFA dfa = DFA(nfa);
    MinimizedDFA minimized_dfa = MinimizedDFA(dfa);
    minimized_dfa.set_name(expr_name);

    if (output) {
        // 画图
        nfa.to_graph(expr_name);
        dfa.to_graph(expr_name);
        minimized_dfa.to_graph(expr_name);
        if (detail) {
            std::cout << regex.get_raw_expr() << std::endl;
            std::cout << regex.get_expr() << std::endl;
            std::cout << regex.get_suffix_expr() << std::endl;
            std::cout << nfa << std::endl;
            std::cout << dfa << std::endl;
            std::cout << minimized_dfa << std::endl;
        }
    }
    std::cout << "Conversion " << expr_name << " completed." << std::endl;
    return minimized_dfa;
}

void la::io::export_dfa(const std::string &expr_name, MinimizedDFA &dfa) {
    std::cout << "Exporting DFA to " << dfa.get_name() << ".dfa" << std::endl;
    std::string base_path = R"(..\example\outputs)";
    std::string dfa_path = base_path + "\\" + expr_name + ".dfa";
    std::ofstream out(dfa_path, std::ios::binary);
    out << dfa;
    out.close();
}

MinimizedDFA la::io::load_dfa(const std::string &expr_name) {
    std::string base_path = R"(..\example\outputs)";
    std::string dfa_path = base_path + "\\" + expr_name + ".dfa";
    std::ifstream in(dfa_path, std::ios::binary);
    std::string _name;
    ds::Inputs _inputs;
    ds::States _states;
    ds::States _end_states;
    int _start_state;

    // 验证存储的DFA文件是否有效
    std::string line;
    auto read_line = [&in, &line]() {
        std::getline(in, line);
        line = utils::trim(line);
//        std::cout << line << std::endl;
    };

    read_line();

    if (line.find("MinimizedDFA") == std::string::npos) {
        std::cerr << expr_name << ": Invalid DFA file." << std::endl;
        return {};
    }

    read_line();
    if (line.find("name:") == std::string::npos) {
        std::cerr << expr_name << ": Invalid DFA file." << std::endl;
        return {};
    }
    _name = utils::split(line, ":")[1];


    read_line();
    if (line.find("inputs:") == std::string::npos) {
        std::cerr << expr_name << ": Invalid DFA file." << std::endl;
        return {};
    }
    auto inputs = utils::split(utils::split(line, ":")[1]);
    for (const auto &input: inputs) {
        _inputs.push_back(input[0]);
    }


    read_line();
    if (line.find("states:") == std::string::npos) {
        std::cerr << expr_name << ": Invalid DFA file." << std::endl;
        return {};
    }
    auto states = utils::split(utils::split(line, ":")[1]);
    for (const auto &s: states) {
        _states.push_back(std::stoi(s));
    }

    read_line();
    if (line.find("start_state_id:") == std::string::npos) {
        std::cerr << expr_name << ": Invalid DFA file." << std::endl;
        return {};
    }
    _start_state = std::stoi(utils::split(line, ":")[1]);


    read_line();
    if (line.find("end_states") == std::string::npos) {
        std::cerr << expr_name << ": Invalid DFA file." << std::endl;
        return {};
    }
    auto end_states = utils::split(utils::split(line, ":")[1]);
    for (const auto &state: end_states) {
        _end_states.push_back(std::stoi(state));
    }

    MinimizedDFA minimized_dfa = MinimizedDFA(_name, _inputs, _states, _end_states, _start_state);

    read_line();
    if (line.find("transitions") == std::string::npos) {
        std::cerr << expr_name << ": Invalid DFA file." << std::endl;
        return {};
    }
    while (true) {
        read_line();
        if (line == "]") {
            break;
        }
        auto elems = utils::split(line);
        if (elems.size() != 5) {
            std::cerr << expr_name << ": Invalid DFA file." << std::endl;
            break;
        }
        auto els = std::vector<std::string>();
        els.push_back(utils::trim(elems[0]));
        els.push_back(utils::trim(elems[2]));
        els.push_back(utils::trim(elems[4]));

        int start = std::stoi(utils::trim(els[0]));
        char input = els[1][0];
        int end = std::stoi(utils::trim(els[2]));

        minimized_dfa.add_transition(start, input, end);
    }

    in.close();
    std::cout << "DFA '" << expr_name << "' loaded." << std::endl;
//    std::cout << minimized_dfa << std::endl;

    return minimized_dfa;
}

std::string la::utils::trim(const std::string &str, const std::string &whitespace) {
    // trim from start
    auto start = str.find_first_not_of(whitespace);
    // trim from end
    auto end = str.find_last_not_of(whitespace);
    if (start == std::string::npos || end == std::string::npos) {
        return {};
    }
    return str.substr(start, end - start + 1);
}

std::vector<std::string> la::utils::split(const std::string &s, const std::string &delim) {
    std::vector<std::string> tokens;
    std::string::size_type lastPos = s.find_first_not_of(delim, 0);
    std::string::size_type pos = s.find_first_of(delim, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos) {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delim, pos);
        pos = s.find_first_of(delim, lastPos);
    }

    return tokens;
}

bool la::utils::judge(MinimizedDFA &dfa, const std::string tokens) {
    int current_state = dfa.get_start_state();
    for (char token: tokens) {
        if (dfa.get_transitions()[current_state].count(token) == 0) {
            return false;
        }
        current_state = dfa.get_transitions()[current_state][token];
    }
    return std::find(dfa.get_end_states().begin(), dfa.get_end_states().end(), current_state) !=
           dfa.get_end_states().end();
}

bool la::utils::is_legal(const std::string &token) {
    return std::all_of(token.begin(), token.end(), [](char c) {
        return Regex::_in_alphabet(c);
    });
}

bool la::utils::is_illegal_ids(const std::string &token) {
    std::regex pattern("^[0-9][a-zA-Z0-9_]*$");
    return std::regex_match(token, pattern);
}

bool la::utils::is_illegal_number(const std::string &token) {
    std::regex pattern("^[0-9]+\\$");
    return std::regex_match(token, pattern);
}

bool la::utils::is_delimiter(const char c) {
    return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ',' || c == ';' || c == ':' || c == ' ';
}

void la::utils::scan(const std::string &in_file_path, const std::string &out_file_path) {
    std::string input_base_path = R"(..\example\inputs)";
    std::string output_base_path = R"(..\example\outputs)";
    std::string input_file_path = input_base_path + "\\" + in_file_path;
    std::string output_file_path = output_base_path + "\\" + out_file_path;

    MinimizedDFA keyword_dfa = io::load_dfa("keyword");
    MinimizedDFA ids_dfa = io::load_dfa("ids");
    MinimizedDFA const_dfa = io::load_dfa("const");
    MinimizedDFA operator_dfa = io::load_dfa("operator");
    MinimizedDFA delimiter_dfa = io::load_dfa("delimiter");

    std::ifstream in(input_file_path);
    std::ofstream out(output_file_path);

    std::string line;
    int cnt = 1;
    auto read_line = [&in, &line]() {
        std::getline(in, line);
        line = utils::trim(line);
    };
    while(!in.eof()) {
        read_line();
        std::string token;
        out << cnt << ": " << line << std::endl;
        for (char c : line) {
            if (is_delimiter(c)) {
                if (!token.empty()) {
                    if (judge(keyword_dfa, token)) {
                        out << "    " << cnt << ": keyword: " << token << std::endl;
                    } else if (judge(ids_dfa, token)) {
                        out << "    "  << cnt << ": ids: " << token << std::endl;
                    } else if (judge(const_dfa, token)) {
                        out << "    "  << cnt << ": const: " << token << std::endl;
                    } else if (judge(operator_dfa, token)) {
                        out << "    "  << cnt << ": operator: " << token << std::endl;
                    } else {
                        out << "    "  << cnt << ": illegal: " << token << std::endl;
                    }
                    token.clear();
                }
                if (c != ' ') {
                    out << "    "  << cnt << ": delimiter: " << c << std::endl;
                }
            } else {
                token += c;
            }
        }
        cnt++;
    }

    in.close();
    out.close();
}


