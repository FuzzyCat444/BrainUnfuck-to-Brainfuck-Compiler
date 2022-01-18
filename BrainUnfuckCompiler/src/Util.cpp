#include "Util.h"

namespace util
{
    std::string repeat(std::string str, int times)
    {
        std::string ret;
        for (int i = 0; i < times; i++)
            ret += str;
        return ret;
    }
}

