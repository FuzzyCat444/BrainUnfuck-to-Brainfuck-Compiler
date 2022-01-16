#ifndef DATA_H
#define DATA_H

#include <string>

struct Data
{
public:
    Data();
    virtual ~Data();

    std::string name;
    int position;
};

#endif // DATA_H
