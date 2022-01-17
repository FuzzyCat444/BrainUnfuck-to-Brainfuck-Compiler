#ifndef TOKENIZINGSTAGE_H
#define TOKENIZINGSTAGE_H

#include "Structure.h"
#include "Stage.h"

#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

class TokenizingStage : public Stage
{
public:
    TokenizingStage();
    virtual ~TokenizingStage();

    void process(CompilerContext& context) const override;
private:
    std::string resolveEscapeSequences(std::string) const;
};

#endif // TOKENIZINGSTAGE_H
