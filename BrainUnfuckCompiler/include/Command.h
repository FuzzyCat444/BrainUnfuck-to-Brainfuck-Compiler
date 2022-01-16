#ifndef COMMAND_H
#define COMMAND_H

#include "Argument.h"

#include <vector>
#include <string>

struct Command
{
public:
    enum class Name
    {
        DATA, PROC, ENDPROC, DO, WHILE, ENDWHILE,
        STORE, IN, OUT,
        ADDRESS, COPYVV, COPYMV, COPYVM, COPYMM,
        ADD, SUB, MUL, DIV,
        LT, GT, EQ, LTE, GTE, NOT, OR, AND,
        INVALID
    };

    static Name commandNameFromStr(std::string str);

    Command();
    virtual ~Command();

    Name name;
    std::vector<Argument> arguments;
};

#endif // COMMAND_H
