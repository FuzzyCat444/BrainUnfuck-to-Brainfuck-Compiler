#include "CompilerContext.h"

CompilerContext::CompilerContext(std::string program)
: program(program), dataPos(0)
{
}

CompilerContext::~CompilerContext()
{
}

std::string CompilerContext::getProgram() const
{
    return program;
}

void CompilerContext::setTokenizingStageResults(std::vector<std::pair<int, Command>> results)
{
    tokenizingStageResults = results;
}

std::vector<std::pair<int, Command>> CompilerContext::getTokenizingStageResults() const
{
    return tokenizingStageResults;
}

void CompilerContext::setRegisteringStageResults(std::vector<std::pair<int, Command>> results)
{
    registeringStageResults = results;
}

std::vector<std::pair<int, Command>> CompilerContext::getRegisteringStageResults() const
{
    return registeringStageResults;
}

void CompilerContext::setSubstitutingStageResults(std::vector<std::pair<int, Command>> results)
{
    substitutingStageResults = results;
}

std::vector<std::pair<int, Command>> CompilerContext::getSubstitutingStageResults() const
{
    return substitutingStageResults;
}

void CompilerContext::log(std::string msg)
{
    msgs.push_back(msg);
}

bool CompilerContext::registerData(std::string dataName, int dataSize)
{
    if (dataReg.find(dataName) != dataReg.end())
        return false;
    Data data;
    data.name = dataName;
    data.position = dataPos;
    dataReg.insert(std::make_pair(dataName, data));
    dataPos += dataSize;
    return true;
}

int CompilerContext::registerProcedure(std::string procName, std::vector<std::string> paramNames, std::vector<std::pair<int, Command>> commands)
{
    if (procReg.find(procName) != procReg.end())
        return -1;
    Procedure proc;
    proc.name = procName;
    for (int i = 0; i < paramNames.size(); i++)
    {
        if (proc.parameters.find(paramNames.at(i)) == proc.parameters.end())
            proc.parameters.insert(std::make_pair(paramNames.at(i), i));
        else
            return -2;
    }
    proc.commands = commands;
    procReg.insert(std::make_pair(procName, proc));
    return 0;
}

int CompilerContext::getDataPos(Argument argument) const
{
    if (dataReg.find(argument.str) != dataReg.end() && argument.num >= 0)
        return dataReg.at(argument.str).position + argument.num;
    return -1;
}

Procedure CompilerContext::getProcedure(std::string procName) const
{
    if (procReg.find(procName) != procReg.end())
        return procReg.at(procName);

    Procedure proc;
    proc.name = "";
    return proc;
}

std::vector<std::string>::const_iterator CompilerContext::messagesBegin() const
{
    return msgs.begin();
}

std::vector<std::string>::const_iterator CompilerContext::messagesEnd() const
{
    return msgs.end();
}

void CompilerContext::setCompiledBrainfuck(std::string compiled)
{
    compiledBrainfuck = compiled;
}

std::string CompilerContext::getCompiledBrainfuck() const
{
    return compiledBrainfuck;
}
