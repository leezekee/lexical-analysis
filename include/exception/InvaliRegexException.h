//
// Created by lizhi on 4/22/2024.
//

#ifndef LEXICAL_ANALYSIS_INVALIREGEXEXCEPTION_H
#define LEXICAL_ANALYSIS_INVALIREGEXEXCEPTION_H

#include "CompileException.h"
#include "string"

class InvalidRegexException : public CompileException {
public:
    explicit InvalidRegexException() : CompileException("Invalid regex") {}
};


#endif //LEXICAL_ANALYSIS_INVALIREGEXEXCEPTION_H
