#include "Compiler.h"

Compiler::Compiler()
{
}

Compiler::~Compiler()
{
}

void Compiler::compile(CompilerContext& context) const
{
    s1.process(context);
    s2.process(context);
    s3.process(context);
    s4.process(context);
}
