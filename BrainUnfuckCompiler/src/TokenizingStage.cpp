#include "TokenizingStage.h"

TokenizingStage::TokenizingStage()
{
}

TokenizingStage::~TokenizingStage()
{
}

void TokenizingStage::process(CompilerContext& context) const
{
    // Convert lines to vector of strings, stripping whitespace
    std::vector<std::string> lines;
    std::istringstream inputSS(context.getProgram());
    std::string ssLine;
    while (std::getline(inputSS, ssLine))
    {
        auto notSpace = [](char ch)
        {
            return !std::isspace(ch);
        };
        ssLine.erase(ssLine.begin(), std::find_if(ssLine.begin(), ssLine.end(), notSpace));
        ssLine.erase(std::find_if(ssLine.rbegin(), ssLine.rend(), notSpace).base(), ssLine.end());
        lines.push_back(ssLine);
    }

    // Tokenize
    std::vector<std::string> tokens;
    std::vector<std::pair<int, Command>> commands;
    for (int m = 0; m < lines.size(); m++)
    {
        std::string& line = lines.at(m);
        if (line.empty())
            continue;

        // Check correct number of parentheses
        if (std::count(line.begin(), line.end(), '"') % 2 != 0)
        {
            std::ostringstream oss;
            oss << "Line " << m + 1 << ": Incomplete parentheses.";
            context.log(oss.str());
        }

        // Break into string tokens
        int i = 0;
        int j = 0;
        bool inQuotes = false;
        while (j <= line.size())
        {
            if (j == line.size() || (line.at(j) == ' ' && !inQuotes))
            {
                tokens.push_back(line.substr(i, j - i));
                while (j < line.size() && line.at(j) == ' ')
                    j++;
                i = j;
            }

            if (j < line.size() && line.at(j) == '"')
                inQuotes = !inQuotes;
            j++;
        }

        // Build command object and arguments from tokens
        Command::Name commandName = Command::commandNameFromStr(tokens.at(0));
        if (commandName == Command::Name::INVALID)
        {
            std::ostringstream oss;
            oss << "Line " << m + 1 << ": Command name '" << tokens.at(0) << "' is invalid.";
            context.log(oss.str());
        }
        std::vector<Argument> commandArguments;
        for (int n = 1; n < tokens.size(); n++)
        {
            std::string tokenN = tokens.at(n);

            Argument arg;
            arg.type = Argument::Type::INVALID;
            if (std::all_of(tokenN.begin(), tokenN.end(), isdigit)) // Numeric
            {
                arg.type = Argument::Type::NUMERIC_LITERAL;
                arg.num = std::stoi(tokenN);
            }
            else if (!isdigit(tokenN.at(0)) &&
                     std::all_of(tokenN.begin(), tokenN.end(), [](char ch) { return isalnum(ch) || ch == '_'; }) &&
                     Command::commandNameFromStr(tokenN) == Command::Name::INVALID) // Variable
            {
                arg.type = Argument::Type::VARIABLE;
                arg.num = 0;
                arg.str = tokenN;
            }
            else if (tokenN.size() >= 2 &&
                     tokenN.front() == '"' && tokenN.back() == '"' &&
                     std::count(tokenN.begin(), tokenN.end(), '"') == 2) // Str literal
            {
                arg.type = Argument::Type::STRING_LITERAL;
                arg.num = 0;
                arg.str = resolveEscapeSequences(tokenN.substr(1, tokenN.size() - 2));
            }
            else
            {
                int b1 = tokenN.find("[");
                int b2 = tokenN.find("]");
                if (b1 != -1 && b2 != -1 && b1 < b2)
                {
                    std::string namePart = tokenN.substr(0, b1);
                    std::string numberPart = tokenN.substr(b1 + 1, b2 - b1 - 1);
                    std::string endPart = tokenN.substr(b2 + 1);
                    if (namePart.size() > 0 && !isdigit(namePart.at(0)) &&
                        std::all_of(namePart.begin(), namePart.end(), [](char ch) { return isalnum(ch) || ch == '_'; }) &&
                        Command::commandNameFromStr(namePart) == Command::Name::INVALID &&
                        numberPart.size() > 0 && std::all_of(numberPart.begin(), numberPart.end(), isdigit) &&
                        endPart.size() == 0) // Array variable
                    {
                        arg.type = Argument::Type::VARIABLE_ARRAY;
                        arg.str = namePart;
                        arg.num = std::stoi(numberPart);
                    }
                }
            }

            commandArguments.push_back(arg);
            if (arg.type == Argument::Type::INVALID)
            {
                std::ostringstream oss;
                oss << "Line " << m + 1 << ": Argument '" << tokenN << "' is invalid.";
                context.log(oss.str());
            }
        }
        Command command;
        command.name = commandName;
        command.arguments = commandArguments;
        commands.push_back(std::make_pair(m + 1, command));

        tokens.clear();
    }

    context.setTokenizingStageResults(commands);
}

std::string TokenizingStage::resolveEscapeSequences(std::string input) const
{
    int pos = 0;
    while ((pos = input.find("\\", pos)) != -1)
    {
        if (pos + 3 < input.size())
        {
            std::string codeStr = input.substr(pos + 1, 3);
            int code = -1;
            if (std::all_of(codeStr.begin(), codeStr.end(), isdigit))
                code = std::stoi(codeStr);
            if (code != -1)
            {
                std::string specialChar = "";
                specialChar.push_back((char) code);
                input.replace(pos, 4, specialChar);
            }
        }
        pos++;
    }
    return input;
}
