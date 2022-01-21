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

    std::string right19 = util::repeat(">", 19);
    std::string right20 = right19 + ">";
    std::string left19 = util::repeat("<", 19);
    std::string left20 = left19 + "<";

    auto argN = [](const Command& command, int n) { return command.arguments.at(n); };
    auto varArg = [argN](const Command& command, int n) { return argN(command, n).type == Argument::Type::VARIABLE; };
    auto varArrArg = [argN](const Command& command, int n) { return argN(command, n).type == Argument::Type::VARIABLE_ARRAY; };
    auto namedArg = [argN, varArg, varArrArg](const Command& command, int n) { return varArg(command, n) || varArrArg(command, n); };
    auto numArg = [argN](const Command& command, int n) { return argN(command, n).type == Argument::Type::NUMERIC_LITERAL; };
    auto strArg = [argN](const Command& command, int n) { return argN(command, n).type == Argument::Type::STRING_LITERAL; };
    auto literalArg = [argN, numArg, strArg](const Command& command, int n) { return numArg(command, n) || strArg(command, n); };
    auto namedNumArg = [argN, namedArg, numArg](const Command& command, int n) { return namedArg(command, n) || numArg(command, n); };
    auto namedStrArg = [argN, namedArg, strArg](const Command& command, int n) { return namedArg(command, n) || strArg(command, n); };
    auto byteSize = [argN](const Command& command, int n) { return argN(command, n).num <= 255; };

    std::stack<int> whileStack;

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
                oss << "Line " << lineNum << ": IN/OUT - Takes 2 arguments.";
                context.log(oss.str());
            }
            else if (!numArg(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": IN/OUT - Argument #1 must be a numeric literal.";
                context.log(oss.str());
            }
            else if (argN(command, 0).num <= 0)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": IN/OUT - Cannot do less than 1 character of input/output.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": IN/OUT - Argument #2 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                int pos = context.getDataPos(argN(command, 1));
                if (pos == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": IN/OUT - Argument #2 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    int argA = argN(command, 0).num;
                    int argB = pos;

                    compiled << right19;
                    compiled << util::repeat(right20, argB);
                    compiled << util::repeat(chr + right20, argA);
                    compiled << util::repeat(left20, argA + argB);
                    compiled << left19;
                }
            }
        }
        else if (command.name == Command::Name::STORE)
        {
            if (command.arguments.size() != 2)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": STORE - Takes 2 arguments.";
                context.log(oss.str());
            }
            else if (!literalArg(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": STORE - Argument #1 must be a numeric or string literal.";
                context.log(oss.str());
            }
            else if (!byteSize(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": STORE - Numeric argument #1 must be in range (0-255).";
                context.log(oss.str());
            }
            else if (!namedArg(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": STORE - Argument #2 must be a variable name.";
                context.log(oss.str());
            }
            else
            {
                int pos = context.getDataPos(argN(command, 1));
                if (pos == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": STORE - Argument #2 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    int argB = pos;
                    if (numArg(command, 0))
                    {
                        int argA = argN(command, 0).num;

                        compiled << right19;
                        compiled << util::repeat(right20, argB);
                        compiled << "[-]";
                        compiled << util::repeat("+", argA);
                        compiled << util::repeat(left20, argB);
                        compiled << left19;
                    }
                    else // STRING_LITERAL
                    {
                        std::string argA = argN(command, 0).str;

                        compiled << right19;
                        compiled << util::repeat(right20, argB);

                        for (int i = 0; i < argA.size(); i++)
                        {
                            compiled << "[-]";
                            compiled << util::repeat("+", (int) argA.at(i));
                            compiled << right20;
                        }

                        compiled << util::repeat(left20, argA.size());

                        compiled << util::repeat(left20, argB);
                        compiled << left19;
                    }
                }
            }
        }
        else if (command.name == Command::Name::ADD || command.name == Command::Name::SUB)
        {
            std::string chr = "+";
            if (command.name == Command::Name::SUB)
                chr = "-";
            if (command.arguments.size() != 3)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": ADD/SUB - Takes 3 arguments.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 0) || !byteSize(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": ADD/SUB - Argument #1 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 1) || !byteSize(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": ADD/SUB - Argument #2 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 2))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": ADD/SUB - Argument #3 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                compiled << zeroWorkingMem(3);
                if (namedArg(command, 0)) // {a}
                {
                    int posA = context.getDataPos(argN(command, 0));
                    if (posA == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": ADD/SUB - Argument #1 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posA, 2);
                }
                else
                    compiled << placeByteInPosition(argN(command, 0).num, 2);
                if (namedArg(command, 1)) // {b}
                {
                    int posB = context.getDataPos(argN(command, 1));
                    if (posB == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": ADD/SUB - Argument #2 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posB, 1);
                }
                else
                    compiled << placeByteInPosition(argN(command, 1).num, 1);
                int posC = context.getDataPos(argN(command, 2));
                if (posC == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": ADD/SUB - Argument #3 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    compiled << "<<<";
                    compiled << "[->>+<<]>[->" + chr + "<]<"; // Add or subtract
                    compiled << ">>>";
                    compiled << positionToVariable(0, posC);
                }
            }
        }
        else if (command.name == Command::Name::MUL)
        {
            if (command.arguments.size() != 3)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": MUL - Takes 3 arguments.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 0) || !byteSize(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": MUL - Argument #1 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 1) || !byteSize(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": MUL - Argument #2 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 2))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": MUL - Argument #3 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                compiled << zeroWorkingMem(4);
                if (namedArg(command, 0)) // {a}
                {
                    int posA = context.getDataPos(argN(command, 0));
                    if (posA == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": MUL - Argument #1 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posA, 3);
                }
                else
                    compiled << placeByteInPosition(argN(command, 0).num, 3);
                if (namedArg(command, 1)) // {b}
                {
                    int posB = context.getDataPos(argN(command, 1));
                    if (posB == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": MUL - Argument #2 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posB, 2);
                }
                else
                    compiled << placeByteInPosition(argN(command, 1).num, 2);
                int posC = context.getDataPos(argN(command, 2));
                if (posC == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": MUL - Argument #3 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    compiled << "<<<<";
                    compiled << "[->[->+>+<<]>>[-<<+>>]<<<]"; // Multiply
                    compiled << ">>>>";
                    compiled << positionToVariable(1, posC);
                }
            }
        }
        else if (command.name == Command::Name::DIV)
        {
            if (command.arguments.size() != 4)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": DIV - Takes 4 arguments.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 0) || !byteSize(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": DIV - Argument #1 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 1) || !byteSize(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": DIV - Argument #2 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 2))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": DIV - Argument #3 must be a variable.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 3))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": DIV - Argument #4 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                compiled << zeroWorkingMem(10);
                if (namedArg(command, 0)) // {a}
                {
                    int posA = context.getDataPos(argN(command, 0));
                    if (posA == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": DIV - Argument #1 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posA, 7);
                }
                else
                    compiled << placeByteInPosition(argN(command, 0).num, 7);
                if (namedArg(command, 1)) // {b}
                {
                    int posB = context.getDataPos(argN(command, 1));
                    if (posB == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": DIV - Argument #2 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posB, 8);
                }
                else
                    compiled << placeByteInPosition(argN(command, 1).num, 8);
                int posC = context.getDataPos(argN(command, 2));
                int posD = context.getDataPos(argN(command, 3));
                if (posC == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": DIV - Argument #3 is an undefined data name.";
                    context.log(oss.str());
                }
                else if (posD == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": DIV - Argument #4 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    compiled << "<<<<<<<<";
                    compiled << "<[->>>>>+>+>>+<<<<<<<<]>>>>>[-<<<<<+>>>>>]<<<<<>[>>+<<[-<-[<]>>]>[" <<
                                "->>>[-<+<<<<<+>>>>>>]>+<<[->+<]<<]>[->]<<<]<[->>>>>>>>-<<<<<<<<]>"; // Divide
                    compiled << ">>>>>>>>";
                    compiled << positionToVariable(1, posC);
                    compiled << positionToVariable(0, posD);
                }
            }
        }
        else if (command.name == Command::Name::WHILE)
        {
            if (command.arguments.size() != 1)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": WHILE - Takes 1 condition argument.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": WHILE - Argument #1 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                int pos = context.getDataPos(argN(command, 0));
                if (pos == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": WHILE - Argument #1 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    whileStack.push(pos);
                    compiled << right19 << util::repeat(right20, pos);
                    compiled << "[";
                    compiled << util::repeat(left20, pos) << left19;
                }
            }
        }
        else if (command.name == Command::Name::ENDWHILE)
        {
            if (command.arguments.size() != 0)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": ENDWHILE - Takes 0 arguments.";
                context.log(oss.str());
            }
            else if (whileStack.size() == 0)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": ENDWHILE - No matching WHILE command.";
                context.log(oss.str());
            }
            else
            {
                int pos = whileStack.top();
                whileStack.pop();
                compiled << right19 << util::repeat(right20, pos);
                compiled << "]";
                compiled << util::repeat(left20, pos) << left19;
            }
        }
        else if (command.name == Command::Name::LTE ||
                 command.name == Command::Name::LT ||
                 command.name == Command::Name::GT ||
                 command.name == Command::Name::GTE ||
                 command.name == Command::Name::EQ ||
                 command.name == Command::Name::NEQ ||
                 command.name == Command::Name::AND ||
                 command.name == Command::Name::OR)
        {
            if (command.arguments.size() != 3)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": LTE/LT/GT/GTE/EQ/NEQ/AND/OR - Takes 3 arguments.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 0) || !byteSize(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": LTE/LT/GT/GTE/EQ/NEQ/AND/OR - Argument #1 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 1) || !byteSize(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": LTE/LT/GT/GTE/EQ/NEQ/AND/OR - Argument #2 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 2))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": LTE/LT/GT/GTE/EQ/NEQ/AND/OR - Argument #3 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                int workingPosA = command.name == Command::Name::LTE ? 5 :
                                  command.name == Command::Name::LT ? 4 :
                                  command.name == Command::Name::GTE ? 4 :
                                  command.name == Command::Name::GT ? 5 :
                                  command.name == Command::Name::EQ ? 5 :
                                  command.name == Command::Name::NEQ ? 5 :
                                  command.name == Command::Name::AND ? 7 :
                                  command.name == Command::Name::OR ? 2 : 0;
                int workingPosB = command.name == Command::Name::LTE ? 4 :
                                  command.name == Command::Name::LT ? 5 :
                                  command.name == Command::Name::GTE ? 5 :
                                  command.name == Command::Name::GT ? 4 :
                                  command.name == Command::Name::EQ ? 4 :
                                  command.name == Command::Name::NEQ ? 4 :
                                  command.name == Command::Name::AND ? 6 :
                                  command.name == Command::Name::OR ? 1 : 0;
                int workingMem = command.name == Command::Name::LTE ? 7 :
                                 command.name == Command::Name::LT ? 7 :
                                 command.name == Command::Name::GTE ? 7 :
                                 command.name == Command::Name::GT ? 7 :
                                 command.name == Command::Name::EQ ? 6 :
                                 command.name == Command::Name::NEQ ? 6 :
                                 command.name == Command::Name::AND ? 8 :
                                 command.name == Command::Name::OR ? 3 : 0;
                int onePos = command.name == Command::Name::LTE ? 2 :
                             command.name == Command::Name::LT ? 2 :
                             command.name == Command::Name::GTE ? 2 :
                             command.name == Command::Name::GT ? 2 :
                             command.name == Command::Name::EQ ? 3 :
                             command.name == Command::Name::NEQ ? 3 :
                             command.name == Command::Name::AND ? 3 :
                             command.name == Command::Name::OR ? 0 : 0;

                compiled << zeroWorkingMem(workingMem);
                if (namedArg(command, 0))
                {
                    int posA = context.getDataPos(argN(command, 0));
                    if (posA == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": LTE/LT/GT/GTE/EQ/NEQ/AND/OR - Argument #1 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posA, workingPosA);
                }
                else
                    compiled << placeByteInPosition(argN(command, 0).num, workingPosA);
                if (namedArg(command, 1))
                {
                    int posB = context.getDataPos(argN(command, 1));
                    if (posB == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": LTE/LT/GT/GTE/EQ/NEQ/AND/OR - Argument #2 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posB, workingPosB);
                }
                else
                    compiled << placeByteInPosition(argN(command, 1).num, workingPosB);
                int posC = context.getDataPos(argN(command, 2));
                if (posC == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": LTE/LT/GT/GTE/EQ/NEQ/AND/OR - Argument #3 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    if (command.name != Command::Name::OR)
                    {
                        compiled << util::repeat("<", onePos + 1);
                        compiled << "+";
                        compiled << util::repeat(">", onePos + 1);
                        if (command.name == Command::Name::EQ)
                            compiled << "<+>";
                    }
                    compiled << util::repeat("<", workingPosA + 1);
                    compiled << (command.name == Command::Name::LTE ? ">[-<-[<]>>]>[->>+<<]>[->]<<<<" :
                                 command.name == Command::Name::LT ? "[-<-[<]>>]>[-]>[->>+<]<<<" :
                                 command.name == Command::Name::GTE ? "[-<-[<]>>]>[->>+<<]>[->]<<<" :
                                 command.name == Command::Name::GT ? ">[-<-[<]>>]>[-]>[->>+<]<<<<" :
                                 command.name == Command::Name::EQ ? "[->-<]>[>>>>-<<]>[->>]<<<<" :
                                 command.name == Command::Name::NEQ ? "[->-<]>[>>>>+<<]>[->>]<<<<" :
                                 command.name == Command::Name::AND ? "[>]>>[->>>+<<<]>[->]>[->>]<<<<<<" :
                                 command.name == Command::Name::OR ? "[->>+<<]>[->+<]<" : "");
                    compiled << util::repeat(">", workingPosA + 1);
                    compiled << positionToVariable(0, posC);
                }
            }
        }
        else if (command.name == Command::Name::NOT)
        {
            if (command.arguments.size() != 2)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": NOT - Takes 2 arguments.";
                context.log(oss.str());
            }
            else if (!namedNumArg(command, 0) || !byteSize(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": NOT - Argument #1 must be a variable or numeric (0-255) or string literal.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": NOT - Argument #2 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                zeroWorkingMem(5);
                if (namedArg(command, 0))
                {
                    int posA = context.getDataPos(argN(command, 0));
                    if (posA == -1)
                    {
                        std::ostringstream oss;
                        oss << "Line " << lineNum << ": NOT - Argument #1 is an undefined data name.";
                        context.log(oss.str());
                    }
                    else
                        compiled << variableToPosition(posA, 4);
                }
                else
                    compiled << placeByteInPosition(argN(command, 0).num, 4);
                int posB = context.getDataPos(argN(command, 1));
                if (posB == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": NOT - Argument #2 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    compiled << "<<<+>>>";
                    compiled << "<<<<<";
                    compiled << "[>]>[-]>[->>+<]<<<";
                    compiled << ">>>>>";
                    compiled << positionToVariable(0, posB);
                }
            }
        }
        else if (command.name == Command::Name::COPYVV)
        {
            if (command.arguments.size() != 2)
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": COPYVV - Takes 2 arguments.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 0))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": COPYVV - Argument #1 must be a variable.";
                context.log(oss.str());
            }
            else if (!namedArg(command, 1))
            {
                std::ostringstream oss;
                oss << "Line " << lineNum << ": COPYVV - Argument #2 must be a variable.";
                context.log(oss.str());
            }
            else
            {
                int posA = context.getDataPos(argN(command, 0));
                int posB = context.getDataPos(argN(command, 1));
                if (posA == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": COPYVV - Argument #1 is an undefined data name.";
                    context.log(oss.str());
                }
                else if (posB == -1)
                {
                    std::ostringstream oss;
                    oss << "Line " << lineNum << ": COPYVV - Argument #2 is an undefined data name.";
                    context.log(oss.str());
                }
                else
                {
                    if (posB < posA)
                    {
                        compiled << right19 << util::repeat(right20, posB) << "[-]";
                        compiled << util::repeat(right20, posA - posB);
                        compiled << "[-<+" << util::repeat(left20, posA - posB).substr(1) << "+";
                        compiled << util::repeat(right20, posA - posB) << "]";
                        compiled << "<[->+<]>" << util::repeat(left20, posA) << left19;
                    }
                    else if (posA < posB)
                    {
                        compiled << right19 << util::repeat(right20, posB) << "[-]";
                        compiled << util::repeat(left20, posB - posA);
                        compiled << "[-<+>" << util::repeat(right20, posB - posA) << "+";
                        compiled << util::repeat(left20, posB - posA) << "]";
                        compiled << "<[->+<]>" << util::repeat(left20, posA) << left19;
                    }
                }
            }
        }
    }

    std::string compiledStr = compiled.str();
    std::string revised = dataPointerMovementAnnihilation(compiledStr);

    context.setCompiledBrainfuck(revised);
}

// Places a variable in the working memory, position >= 0
std::string AssemblingStage::variableToPosition(int variable, int position) const
{
    std::string right19 = util::repeat(">", 19);
    std::string right20 = right19 + ">";
    std::string left19 = util::repeat("<", 19);
    std::string left20 = left19 + "<";
    std::string right20X = util::repeat(right20, variable);
    std::string left20X = util::repeat(left20, variable);

    std::string pass1 = right19 + right20X;
    std::string pass2 = "[-<+" + (left20X + left19).substr(1) + util::repeat("<", position + 1) + "+";
    std::string pass3 = util::repeat(">", position + 1) + right19 + right20X + "]";
    std::string pass4 = "<[->+<]>" + left20X + left19;

    return pass1 + pass2 + pass3 + pass4;
}

std::string AssemblingStage::positionToVariable(int position, int variable) const
{
    std::string right19 = util::repeat(">", 19);
    std::string right20 = right19 + ">";
    std::string left19 = util::repeat("<", 19);
    std::string left20 = left19 + "<";
    std::string right20X = util::repeat(right20, variable);
    std::string left20X = util::repeat(left20, variable);

    std::string pass1 = right19 + right20X + "[-]";
    std::string pass2 = left20X + left19 + util::repeat("<", position + 1);
    std::string pass3 = "[-" + util::repeat(">", position + 1) + right19 + right20X + "+";
    std::string pass4 = left20X + left19 + util::repeat("<", position + 1) + "]" + util::repeat(">", position + 1);

    return pass1 + pass2 + pass3 + pass4;
}

// Places a byte into working memory
std::string AssemblingStage::placeByteInPosition(int byte, int position) const
{
    return util::repeat("<", position + 1) + util::repeat("+", byte) + util::repeat(">", position + 1);
}

// Finds all pairs of "<" and ">" and combines them into nothing
std::string AssemblingStage::dataPointerMovementAnnihilation(std::string input) const
{
    int first = 0;
    while ((first = input.find_first_of("<>", first)) != -1)
    {
        int second = input.find_first_not_of("<>", first + 1);

        if (second == -1)
            second = input.size();

        std::string ptrMoves = input.substr(first, second - first);
        int rights = std::count(ptrMoves.begin(), ptrMoves.end(), '>');
        int lefts = ptrMoves.size() - rights;
        int totalRights = std::max(0, rights - lefts);
        int totalLefts = std::max(0, lefts - rights);
        std::string newPtrMoves = util::repeat(">", totalRights) + util::repeat("<", totalLefts);
        input.replace(first, second - first, newPtrMoves);
        first += newPtrMoves.size();
        second = first + 1;
    }
    return input;
}

std::string AssemblingStage::zeroWorkingMem(int amt) const
{
    std::string pass1 = util::repeat("<[-]", amt);
    std::string pass2 = util::repeat(">", amt);
    return pass1 + pass2;
}
