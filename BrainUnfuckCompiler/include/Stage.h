#ifndef STAGE_H
#define STAGE_H

#include "CompilerContext.h"

class Stage
{
public:
    Stage();
    virtual ~Stage();

    virtual void process(CompilerContext& context) const = 0;
};

#endif // STAGE_H
