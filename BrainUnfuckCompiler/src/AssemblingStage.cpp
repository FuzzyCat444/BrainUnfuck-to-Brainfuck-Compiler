#include "AssemblingStage.h"

AssemblingStage::AssemblingStage()
{
}

AssemblingStage::~AssemblingStage()
{
}

void AssemblingStage::process(CompilerContext& context) const
{
    std::vector<std::pair<int, Command>> commands = context.getSubstitutingStageResults();

    std::ostringstream compiled;
    for (auto it = commands.begin(); it != commands.end(); it++)
    {
        int lineNum = it->first;
        const Command& command = it->second;

        if (command.name == Command::Name::INVALID)
        {
            std::ostringstream oss;
            oss << "Line " << lineNum << ": Command name is invalid.";
            context.log(oss.str());
        }
        else if (command.name == Command::Name::IN || command.name == Command::Name::OUT)
        {
            std::string chr = ",";
            if (command.name == Command::Name::OUT)
                chr = ".";

            if (command.arguments.size() != 2)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": IN/OUT takes 2 arguments.";
                context.log(oss.str());
            }
            else if (command.arguments.at(0).type != Argument::Type::NUMERIC_LITERAL)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": First argument of IN/OUT must be a numeric literal.";
                context.log(oss.str());
            }
            else if (command.arguments.at(0).num <= 0)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": Cannot take less than 1 character of input/output.";
                context.log(oss.str());
            }
            else if (command.arguments.at(1).type != Argument::Type::VARIABLE && command.arguments.at(1).type != Argument::Type::VARIABLE_ARRAY)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": Second argument of IN/OUT must be a variable.";
                context.log(oss.str());
            }
            else
            {
                int offset = command.arguments.at(1).type == Argument::Type::VARIABLE_ARRAY ? command.arguments.at(1).num : 0;
                int pos = context.getDataPos(command.arguments.at(1).str, offset);
                if (pos == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": Invalid data referenced or invalid variable position to input/output chars.";
                    context.log(oss.str());
                }
                else
                {
                    int argA = command.arguments.at(0).num;
                    int argB = pos;

                    std::string right19 = util::repeat(">", 19);
                    std::string right20 = right19 + ">";
                    std::string left19 = util::repeat("<", 19);
                    std::string left20 = left19 + "<";

                    compiled << right19;
                    compiled << util::repeat(right20, argB);
                    compiled << util::repeat(chr + right20, argA);
                    compiled << util::repeat(left20, argA + argB);
                    compiled << left19;
                }
            }
        }
    }

    context.setCompiledBrainfuck(compiled.str());
}
