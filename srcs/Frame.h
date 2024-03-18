#pragma once

#include"Location.h"
#include <list>
#include <vector>
#define MAGIC_NUMBER 2024
using namespace std;
class Good;
class Frame
{
private:
    list<Good*> gds;
public:
    void addGood(Good *good);
    bool removeGood(Location &loc);
    void work();
};
extern vector<Frame> frames;