#include "Compiler.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char* argv[])
{
    Compiler cmp;

    std::string fileName(argv[1]);
    std::ifstream ifs(fileName);
    std::ostringstream iss;
    iss << ifs.rdbuf();
    ifs.close();
    std::string program(iss.str());

    int pos = 0;
    while ((pos = program.find("\r\n", pos)) != -1)
    {
        program.replace(pos, 2, "\n");
        pos++;
    }
    while ((pos = program.find("\r", pos)) != -1)
    {
        program.replace(pos, 1, "\n");
        pos++;
    }

    CompilerContext context(program);
    cmp.compile(context);
    std::string compiled = context.getCompiledBrainfuck();

    std::cout << std::endl;
    std::cout << "Compiler Messages:\n===================================\n";
    for (auto it = context.messagesBegin(); it != context.messagesEnd(); it++)
        std::cout << *it << std::endl;

    std::ofstream ofs(argv[2]);
    ofs << compiled;
    ofs.close();

    return 0;
}
