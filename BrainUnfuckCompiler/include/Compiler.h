#ifndef COMPILER_H
#define COMPILER_H

#include "TokenizingStage.h"
#include "RegisteringStage.h"
#include "SubstitutingStage.h"
#include "AssemblingStage.h"
#include "Stage.h"

#include <string>
#include <iostream>

class Compiler
{
public:
    Compiler();
    virtual ~Compiler();

    void compile(CompilerContext& context) const;
private:
    TokenizingStage s1;
    RegisteringStage s2;
    SubstitutingStage s3;
    AssemblingStage s4;
};

#endif // COMPILER_H
