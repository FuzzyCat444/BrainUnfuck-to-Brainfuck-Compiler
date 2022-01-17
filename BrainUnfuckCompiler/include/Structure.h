#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

struct Argument
{
    enum class Type
    {
        VARIABLE, VARIABLE_ARRAY, STRING_LITERAL, NUMERIC_LITERAL, INVALID
    };

    Type type;
    std::string str;
    int num;
};

struct Command
{
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

    Name name;
    std::vector<Argument> arguments;
};

struct Data
{
    std::string name;
    int position;
};

struct Procedure
{
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::pair<int, Command>> commands;
};

#endif // STRUCTURE_H
