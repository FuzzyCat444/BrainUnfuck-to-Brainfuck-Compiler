#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>

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
        DATA, PROC, ENDPROC, DO, DO_, WHILE, ENDWHILE,
        STORE, IN, OUT,
        ADDRESS, COPYVV, COPYMV, COPYVM,
        ADD, SUB, MUL, DIV,
        LT, GT, EQ, NEQ, LTE, GTE, NOT, OR, AND,
        INVALID
    };

    static Name commandNameFromStr(std::string str);
    static std::string commandStrFromName(Name name);

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
    std::unordered_map<std::string, int> parameters;
    std::vector<std::pair<int, Command>> commands;
};

#endif // STRUCTURE_H
