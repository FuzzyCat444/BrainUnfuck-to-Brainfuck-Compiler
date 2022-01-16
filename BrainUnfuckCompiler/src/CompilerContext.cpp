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

bool CompilerContext::registerProcedure(std::string procName, std::vector<std::string> paramNames, std::vector<std::pair<int, Command>> commands)
{
    if (procReg.find(procName) != procReg.end())
        return false;
    Procedure proc;
    proc.name = procName;
    proc.parameters = paramNames;
    proc.commands = commands;
    procReg.insert(std::make_pair(procName, proc));
    return true;
}

std::vector<std::string>::const_iterator CompilerContext::messagesBegin() const
{
    return msgs.begin();
}

std::vector<std::string>::const_iterator CompilerContext::messagesEnd() const
{
    return msgs.end();
}


std::string CompilerContext::getCompiledBrainfuck() const
{
    return compiledBrainfuck;
}
