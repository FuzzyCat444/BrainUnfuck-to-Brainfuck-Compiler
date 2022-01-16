#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "Command.h"

#include <string>
#include <vector>

struct Procedure
{
public:
    Procedure();
    virtual ~Procedure();

    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::pair<int, Command>> commands;
};

#endif // PROCEDURE_H
