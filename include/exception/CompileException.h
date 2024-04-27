//
// Created by lizhi on 3/23/2024.
//

#ifndef LEXICAL_ANALYSIS_COMPILEERROR_H
#define LEXICAL_ANALYSIS_COMPILEERROR_H

#include "exception"
#include "string"
#include <iostream>
#include "windows.h"

class CompileException : std::exception{
private:
    std::string _message;
public:
    CompileException(std::string message) : _message(std::move(message)) {
        HANDLE std_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(std_handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
        std::cout << "Compile Error: " << _message << std::endl;
    }
    const char *what() const noexcept override {
        return _message.c_str();
    }
};

#endif //LEXICAL_ANALYSIS_COMPILEERROR_H
