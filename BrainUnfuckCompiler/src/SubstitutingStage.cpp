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
        while ((commandsIt = std::find_if(commandsIt, commands.end(), [](const std::pair<int, Command>& p) { return p.second.name == Command::Name::DO; })) != commands.end())
        {
            Command doCommand = commandsIt->second;
            bool replaceCommands = false;
            Procedure proc;
            if (doCommand.arguments.size() < 1 || doCommand.arguments.at(0).type != Argument::Type::VARIABLE)
            {
                std::ostringstream oss;
                oss << "Line " << commandsIt->first << ": DO requires a procedure name as the first argument.";
                context.log(oss.str());
            }
            else
            {
                proc = context.getProcedure(doCommand.arguments.at(0).str);
                if (proc.name.empty())
                {
                    std::ostringstream oss;
                    oss << "Line " << commandsIt->first << ": Argument does not name a valid procedure.";
                    context.log(oss.str());
                }
                else if (proc.parameters.size() != doCommand.arguments.size() - 1)
                {
                    std::ostringstream oss;
                    oss << "Line " << commandsIt->first << ": Incorrect number of arguments provided to procedure '" << proc.name << "'.";
                    context.log(oss.str());
                }
                else
                {
                    replaceCommands = true;
                }
            }

            commandsIt = commands.erase(commandsIt);
            subs++;

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
                            commandArg = doCommand.arguments.at(1 + proc.parameters.at(commandArg.str));
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

    printCommands(commands);

    context.setSubstitutingStageResults(commands);
}
