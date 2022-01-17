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
        return Command::Name::STORE;
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
