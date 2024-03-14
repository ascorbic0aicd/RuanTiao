#pragma once

#include <list>
using namespace std;
class Good;
class Frame
{
private:
    list<Good*> gds;
public:
    void addGood(Good *good);
    void work();
};