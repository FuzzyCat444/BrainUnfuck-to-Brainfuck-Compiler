#ifndef COMPILERCONTEXT_H
#define COMPILERCONTEXT_H

#include "Structure.h"

#include <vector>
#include <string>
#include <unordered_map>

struct CompilerContext
{
public:
    CompilerContext(std::string program);
    virtual ~CompilerContext();

    std::string getProgram() const;

    void setTokenizingStageResults(std::vector<std::pair<int, Command>> results);
    std::vector<std::pair<int, Command>> getTokenizingStageResults() const;

    void setRegisteringStageResults(std::vector<std::pair<int, Command>> results);
    std::vector<std::pair<int, Command>> getRegisteringStageResults() const;

    void log(std::string msg);

    bool registerData(std::string dataName, int dataSize);
    bool registerProcedure(std::string procName, std::vector<std::string> paramNames, std::vector<std::pair<int, Command>> commands);

    std::vector<std::string>::const_iterator messagesBegin() const;
    std::vector<std::string>::const_iterator messagesEnd() const;

    std::string getCompiledBrainfuck() const;
private:
    std::string program;

    std::vector<std::pair<int, Command>> tokenizingStageResults;
    std::vector<std::pair<int, Command>> registeringStageResults;

    int dataPos;
    std::unordered_map<std::string, Data> dataReg;
    std::unordered_map<std::string, Procedure> procReg;

    std::vector<std::string> msgs;

    std::string compiledBrainfuck;
};

#endif // COMPILERCONTEXT_H
