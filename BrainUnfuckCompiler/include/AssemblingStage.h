#ifndef ASSEMBLINGSTAGE_H
#define ASSEMBLINGSTAGE_H

#include "Stage.h"
#include "Util.h"

#include <sstream>
#include <algorithm>
#include <cmath>
#include <stack>

class AssemblingStage : public Stage
{
public:
    AssemblingStage();
    virtual ~AssemblingStage();

    void process(CompilerContext& context) const override;
private:
    std::string variableToPosition(int variable, int position) const;
    std::string positionToVariable(int position, int variable) const;
    std::string placeByteInPosition(int byte, int position) const;
    std::string zeroWorkingMem(int amt) const;

    std::string dataPointerMovementAnnihilation(std::string input) const;
};

#endif // ASSEMBLINGSTAGE_H
