#ifndef SUBSTITUTIONSTAGE_H
#define SUBSTITUTIONSTAGE_H

#include "Stage.h"

#include <sstream>
#include <algorithm>
#include <iostream>

class SubstitutingStage : public Stage
{
public:
    SubstitutingStage();
    virtual ~SubstitutingStage();

    void process(CompilerContext& context) const override;
private:
};

#endif // SUBSTITUTIONSTAGE_H
