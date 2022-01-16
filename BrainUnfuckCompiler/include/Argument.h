#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>

struct Argument
{
public:
    enum class Type
    {
        VARIABLE, VARIABLE_ARRAY, STRING_LITERAL, NUMERIC_LITERAL, INVALID
    };

    Argument();
    virtual ~Argument();

    Type type;
    std::string str;
    int num;
};

#endif // ARGUMENT_H
