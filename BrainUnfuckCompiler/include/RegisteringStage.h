#ifndef REGISTRYSTAGE_H
#define REGISTRYSTAGE_H

#include "Stage.h"

#include <sstream>
#include <algorithm>
#include <iostream>

class RegisteringStage : public Stage
{
public:
    RegisteringStage();
    virtual ~RegisteringStage();

    void process(CompilerContext& context) const override;
private:
};

#endif // REGISTRYSTAGE_H
