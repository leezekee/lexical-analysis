/**
 * Created by lizhi on 4/25/2024.
 * Program 01: Regx to Minimized DFA
 * This is a program that converts a regular expression to a minimized DFA.
 * The regular expressions are read from the input file.
 * The minimized DFA graphs are output to folder `bin/graph`.
 */

#include "utils.h"
#include "string"
#include "fstream"

int main(int argc, char *argv[]) {
    std::string base_path = "../example/inputs";
    std::ifstream fs = std::ifstream();
    fs.open(base_path + "/grammer.txt");
    std::string keyword_, operator_, const_, delimiter_, ids_;
    const std::string   KEYWORD = "keyword",
                        OPERATOR = "operator",
                        CONST = "const",
                        DELIMITER = "delimiter",
                        IDS = "ids";

    // get keyword regex
    std::getline(fs, keyword_);
    // get operator regex
    std::getline(fs, operator_);
    // get const regex
    std::getline(fs, const_);
    // get delimiter regex
    std::getline(fs, delimiter_);
    // get ids regex
    std::getline(fs, ids_);


    // convert regex to dfa
    MinimizedDFA keyword_dfa = la::utils::reg2dfa(KEYWORD, keyword_);
    MinimizedDFA operator_dfa = la::utils::reg2dfa(OPERATOR, operator_);
    MinimizedDFA const_dfa = la::utils::reg2dfa(CONST, const_);
    MinimizedDFA delimiter_dfa = la::utils::reg2dfa(DELIMITER, delimiter_);
    MinimizedDFA ids_dfa = la::utils::reg2dfa(IDS, ids_);

    // export dfa
    la::io::export_dfa(KEYWORD, keyword_dfa);
    la::io::export_dfa(OPERATOR, operator_dfa);
    la::io::export_dfa(CONST, const_dfa);
    la::io::export_dfa(DELIMITER, delimiter_dfa);
    la::io::export_dfa(IDS, ids_dfa);
    return 0;
}