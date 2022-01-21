#include "Structure.h"

Command::Name Command::commandNameFromStr(std::string str)
{
    if (str == "DATA")
        return Command::Name::DATA;
    else if (str == "PROC")
        return Command::Name::PROC;
    else if (str == "ENDPROC")
        return Command::Name::ENDPROC;
    else if (str == "DO")
        return Command::Name::DO;
    else if (str == "DO_")
        return Command::Name::DO_;
    else if (str == "WHILE")
        return Command::Name::WHILE;
    else if (str == "ENDWHILE")
        return Command::Name::ENDWHILE;
    else if (str == "STORE")
        return Command::Name::STORE;
    else if (str == "IN")
        return Command::Name::IN;
    else if (str == "OUT")
        return Command::Name::OUT;
    else if (str == "ADDRESS")
        return Command::Name::ADDRESS;
    else if (str == "COPYVV")
        return Command::Name::COPYVV;
    else if (str == "COPYMV")
        return Command::Name::COPYMV;
    else if (str == "COPYVM")
        return Command::Name::COPYVM;
    else if (str == "COPYMM")
        return Command::Name::COPYMM;
    else if (str == "ADD")
        return Command::Name::ADD;
    else if (str == "SUB")
        return Command::Name::SUB;
    else if (str == "MUL")
        return Command::Name::MUL;
    else if (str == "DIV")
        return Command::Name::DIV;
    else if (str == "LT")
        return Command::Name::LT;
    else if (str == "GT")
        return Command::Name::GT;
    else if (str == "EQ")
        return Command::Name::EQ;
    else if (str == "NEQ")
        return Command::Name::NEQ;
    else if (str == "LTE")
        return Command::Name::LTE;
    else if (str == "GTE")
        return Command::Name::GTE;
    else if (str == "NOT")
        return Command::Name::NOT;
    else if (str == "OR")
        return Command::Name::OR;
    else if (str == "AND")
        return Command::Name::AND;

    return Command::Name::INVALID;
}

std::string Command::commandStrFromName(Name name)
{
    if (name == Name::DATA)
        return "DATA";
    else if (name == Name::PROC)
        return "PROC";
    else if (name == Name::ENDPROC)
        return "ENDPROC";
    else if (name == Name::DO)
        return "DO";
    else if (name == Name::DO_)
        return "DO_";
    else if (name == Name::WHILE)
        return "WHILE";
    else if (name == Name::ENDWHILE)
        return "ENDWHILE";
    else if (name == Name::STORE)
        return "STORE";
    else if (name == Name::IN)
        return "IN";
    else if (name == Name::OUT)
        return "OUT";
    else if (name == Name::ADDRESS)
        return "ADDRESS";
    else if (name == Name::COPYVV)
        return "COPYVV";
    else if (name == Name::COPYMV)
        return "COPYMV";
    else if (name == Name::COPYVM)
        return "COPYVM";
    else if (name == Name::COPYMM)
        return "COPYMM";
    else if (name == Name::ADD)
        return "ADD";
    else if (name == Name::SUB)
        return "SUB";
    else if (name == Name::MUL)
        return "MUL";
    else if (name == Name::DIV)
        return "DIV";
    else if (name == Name::LT)
        return "LT";
    else if (name == Name::GT)
        return "GT";
    else if (name == Name::EQ)
        return "EQ";
    else if (name == Name::NEQ)
        return "NEQ";
    else if (name == Name::LTE)
        return "LTE";
    else if (name == Name::GTE)
        return "GTE";
    else if (name == Name::NOT)
        return "NOT";
    else if (name == Name::OR)
        return "OR";
    else if (name == Name::AND)
        return "AND";

    return "INVALID";
}

void printCommands(const std::vector<std::pair<int, Command>>& commands)
{
    for (auto it = commands.begin(); it != commands.end(); it++)
    {
        const Command& command = it->second;
        std::cout << Command::commandStrFromName(command.name) << " ";
        for (auto argIt = command.arguments.begin(); argIt != command.arguments.end(); argIt++)
        {
            if (argIt->type == Argument::Type::INVALID)
                std::cout << "INVALID";
            else if (argIt->type == Argument::Type::NUMERIC_LITERAL)
                std::cout << argIt->num;
            else if (argIt->type == Argument::Type::STRING_LITERAL)
                std::cout << '"' << argIt->str << '"';
            else if (argIt->type == Argument::Type::VARIABLE)
                std::cout << argIt->str;
            else if (argIt->type == Argument::Type::VARIABLE_ARRAY)
                std::cout << argIt->str << "[" << argIt->num << "]";
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
