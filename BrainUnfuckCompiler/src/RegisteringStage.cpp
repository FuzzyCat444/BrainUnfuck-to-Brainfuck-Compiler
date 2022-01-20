#include "RegisteringStage.h"

RegisteringStage::RegisteringStage()
{
}

RegisteringStage::~RegisteringStage()
{
}

void RegisteringStage::process(CompilerContext& context) const
{
    // Extract data
    std::vector<std::pair<int, Command>> commands = context.getTokenizingStageResults();
    auto commandsIt = commands.begin();
    while (commandsIt != commands.end())
    {
        int lineNum = commandsIt->first;
        Command& command = commandsIt->second;
        if (command.name == Command::Name::DATA)
        {
            int dataSize = -1;
            std::vector<Argument>& args = command.arguments;
            int argNum = 1;
            for (auto it = args.begin(); it != args.end(); it++, argNum++)
            {
                Argument& a = *it;
                switch (a.type)
                {
                case Argument::Type::INVALID:
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": Argument #" << argNum << " to be registered as data is invalid.";
                    context.log(oss.str());
                    break;
                }
                case Argument::Type::NUMERIC_LITERAL:
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": Argument #" << argNum << " to be registered as data is a numeric literal instead of a name.";
                    context.log(oss.str());
                    break;
                }
                case Argument::Type::STRING_LITERAL:
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": Argument #" << argNum << " to be registered as data is a string literal instead of a name.";
                    context.log(oss.str());
                    break;
                }
                case Argument::Type::VARIABLE:
                    dataSize = 1;
                    break;
                case Argument::Type::VARIABLE_ARRAY:
                    dataSize = a.num;
                    break;
                }
                if (dataSize != -1)
                {
                    if (dataSize == 0)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": Data name '" << a.str << "' cannot have a size of 0.";
                        context.log(oss.str());
                    }
                    else if (!context.registerData(a.str, dataSize))
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": Data name '" << a.str << "' redefinition.";
                        context.log(oss.str());
                    }
                }
            }
            commandsIt = commands.erase(commandsIt);
        }
        else
            commandsIt++;
    }

    // Extract procedures
    auto procStartIt = commands.begin();
    while ((procStartIt = std::find_if(procStartIt, commands.end(),
            [](const std::pair<int, Command>& p) { return p.second.name == Command::Name::PROC; })) != commands.end())
    {
        int lineNum = procStartIt->first;

        auto procEndIt = std::find_if(procStartIt + 1, commands.end(),
             [](const std::pair<int, Command>& p) { return p.second.name == Command::Name::ENDPROC; });

        bool procRegistered = false;
        if (procEndIt == commands.end())
        {
            std::ostringstream oss;
            oss << "Line " << lineNum << ": PROC is not followed with ENDPROC.";
            context.log(oss.str());
        }
        else
        {
            Command& command = procStartIt->second;
            if (command.arguments.size() == 0)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": Procedure must have a name.";
                context.log(oss.str());
            }
            else if (command.arguments.at(0).type != Argument::Type::VARIABLE)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": Procedure name is invalid.";
                context.log(oss.str());
            }
            else
            {
                std::string procName = command.arguments.at(0).str;
                if (!isdigit(procName.at(0)) && std::all_of(procName.begin(), procName.end(), [](char ch) { return isalnum(ch) || ch == '_'; }) &&
                    Command::commandNameFromStr(procName) == Command::Name::INVALID)
                {
                    if (std::all_of(command.arguments.begin() + 1, command.arguments.end(), [](const Argument& arg) { return arg.type == Argument::Type::VARIABLE; }))
                    {
                        std::vector<std::string> paramNames;
                        for (auto it = command.arguments.begin() + 1; it != command.arguments.end(); it++)
                            paramNames.push_back(it->str);
                        std::vector<std::pair<int, Command>> procCommands(procStartIt + 1, procEndIt);
                        int result = context.registerProcedure(procName, paramNames, procCommands);
                        if (result == -1)
                        {
                            std::ostringstream oss;
                            oss << "Line " << lineNum << ": Procedure '" << procName << "' redefinition.";
                            context.log(oss.str());
                        }
                        else if (result == -2)
                        {
                            std::ostringstream oss;
                            oss << "Line " << lineNum << ": Duplicate procedure arguments for '" << procName << "'.";
                            context.log(oss.str());
                        }
                        else
                        {
                            procStartIt = commands.erase(procStartIt, procEndIt + 1);
                            procRegistered = true;
                        }
                    }
                    else
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": Procedure '" << procName << "' has invalid parameters.";
                        context.log(oss.str());
                    }
                }
                else
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": Procedure name '" << procName << "' is invalid.";
                    context.log(oss.str());
                }
            }
        }
        if (!procRegistered)
            procStartIt++;
    }

    context.setRegisteringStageResults(commands);
}
