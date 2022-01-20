#include "SubstitutingStage.h"

SubstitutingStage::SubstitutingStage()
{
}

SubstitutingStage::~SubstitutingStage()
{
}

void SubstitutingStage::process(CompilerContext& context) const
{
    std::vector<std::pair<int, Command>> commands = context.getRegisteringStageResults();

    // Substitute all DO commands with procedure commands
    int iterations = 0;
    while (true)
    {
        int subs = 0;
        auto commandsIt = commands.begin();
        while ((commandsIt = std::find_if(commandsIt, commands.end(), [](const std::pair<int, Command>& p) { return p.second.name == Command::Name::DO || p.second.name == Command::Name::DO_; })) != commands.end())
        {
            Command doCommand = commandsIt->second;
            commandsIt = commands.erase(commandsIt);
            subs++;

            if (doCommand.name == Command::Name::DO_)
            {
                Command newDo;
                if (doCommand.arguments.size() != 2)
                {
                    std::ostringstream oss;
                    oss << "Line " << commandsIt->first << ": DO_ - Requires 2 arguments procedure name and variable name.";
                    context.log(oss.str());
                }
                else if (doCommand.arguments.at(0).type != Argument::Type::VARIABLE)
                {
                    std::ostringstream oss;
                    oss << "Line " << commandsIt->first << ": DO_ - Invalid procedure name (argument #1).";
                    context.log(oss.str());
                }
                else if (doCommand.arguments.at(1).type != Argument::Type::VARIABLE &&
                         doCommand.arguments.at(1).type != Argument::Type::VARIABLE_ARRAY)
                {
                    std::ostringstream oss;
                    oss << "Line " << commandsIt->first << ": DO_ - Invalid variable name (argument #2).";
                    context.log(oss.str());
                }
                else
                {
                    newDo.name = Command::Name::DO;
                    Procedure proc = context.getProcedure(doCommand.arguments.at(0).str);
                    if (proc.name.empty())
                    {
                        std::ostringstream oss;
                        oss << "Line " << commandsIt->first << ": DO_ - Procedure '" << doCommand.arguments.at(0).str << "' is undefined.";
                        context.log(oss.str());
                    }
                    else
                    {
                        Argument procNameArg;
                        procNameArg.type = Argument::Type::VARIABLE;
                        procNameArg.str = doCommand.arguments.at(0).str;
                        newDo.arguments.push_back(procNameArg);
                        for (int i = 0; i < proc.parameters.size(); i++)
                        {
                            Argument arg;
                            arg.type = Argument::Type::VARIABLE_ARRAY;
                            arg.str = doCommand.arguments.at(1).str;
                            arg.num = doCommand.arguments.at(1).num + i;
                            newDo.arguments.push_back(arg);
                        }
                        commandsIt = commands.insert(commandsIt, std::make_pair(commandsIt->first, newDo));
                    }
                }
                continue;
            }

            bool replaceCommands = false;
            Procedure proc;
            if (doCommand.arguments.size() < 1 || doCommand.arguments.at(0).type != Argument::Type::VARIABLE)
            {
                std::ostringstream oss;
                oss << "Line " << commandsIt->first << ": DO - Argument #1 must be a procedure name.";
                context.log(oss.str());
            }
            else
            {
                proc = context.getProcedure(doCommand.arguments.at(0).str);
                if (proc.name.empty())
                {
                    std::ostringstream oss;
                    oss << "Line " << commandsIt->first << ": DO - Procedure '" << doCommand.arguments.at(0).str << "' is undefined.";
                    context.log(oss.str());
                }
                else if (proc.parameters.size() != doCommand.arguments.size() - 1)
                {
                    std::ostringstream oss;
                    oss << "Line " << commandsIt->first << ": DO - Incorrect number of arguments provided to procedure '" << proc.name << "'.";
                    context.log(oss.str());
                }
                else
                {
                    replaceCommands = true;
                }
            }

            if (replaceCommands)
            {
                for (auto it1 = proc.commands.begin(); it1 != proc.commands.end(); it1++)
                {
                    Command& procCommand = it1->second;
                    for (auto it2 = procCommand.arguments.begin(); it2 != procCommand.arguments.end(); it2++)
                    {
                        Argument& commandArg = *it2;
                        if (proc.parameters.find(commandArg.str) != proc.parameters.end())
                        {
                            Argument passedArg = doCommand.arguments.at(1 + proc.parameters.at(commandArg.str));
                            if (commandArg.type == Argument::Type::VARIABLE)
                                commandArg = passedArg;
                            else if (commandArg.type == Argument::Type::VARIABLE_ARRAY)
                            {
                                commandArg.str = passedArg.str;
                                commandArg.num += passedArg.num;
                            }
                        }
                    }
                }
                commandsIt = commands.insert(commandsIt, proc.commands.begin(), proc.commands.end());
                commandsIt += proc.commands.size();
            }
        }

        if (subs == 0)
            break;
        else if (iterations >= 256)
        {
            std::ostringstream oss;
            oss << "Procedure nesting too deep, recursion is not allowed.";
            context.log(oss.str());
            break;
        }
        iterations++;
    }

    context.setSubstitutingStageResults(commands);
}
