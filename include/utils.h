//
// Created by lizhi on 4/25/2024.
//

#ifndef LEXICAL_ANALYSIS_LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_LEXICAL_ANALYSIS_H

#include "MinimizedDFA.h"


namespace la {
    namespace utils {
        MinimizedDFA reg2dfa(const std::string &expr_name, std::string expr, bool output = false, bool detail = false);
        std::string trim(const std::string& str, const std::string& whitespace = " \t");
        std::vector<std::string> split(const std::string &str, const std::string &delim = " ");
        bool judge(MinimizedDFA &dfa, const std::string tokens);
        bool is_legal(const std::string &token);
        bool is_illegal_ids(const std::string &token);
        bool is_illegal_number(const std::string &token);
        bool is_delimiter(const char c);
        void scan(const std::string &in_file_path, const std::string &out_file_path);
    }
    namespace io {
        void export_dfa(const std::string &expr_name, MinimizedDFA &dfa);
        MinimizedDFA load_dfa(const std::string &expr_name);
    }
}


void scan();

#endif //LEXICAL_ANALYSIS_LEXICAL_ANALYSIS_H
