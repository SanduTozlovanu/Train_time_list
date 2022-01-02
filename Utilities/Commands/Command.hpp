#pragma once
#include <iostream>
#include "../Instruments.hpp"
class Command
{
    public:
    virtual const char* execute()=0;
};