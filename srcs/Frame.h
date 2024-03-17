#pragma once
#include<vector>
#include <list>
#define MAGIC_NUMBER 2024
using namespace std;
class Good;
class Frame
{
private:
    list<Good*> gds;
public:
    void addGood(Good *good);
    void work();
    bool remove(Good* good);
};
extern vector<Frame> frames;