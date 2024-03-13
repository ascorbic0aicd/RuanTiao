#pragma once
#define CTRL_NUM 2
#define DIV (200 / CTRL_NUM)
#include <list>
#include "Good.h"
#include "Berth.h"
class Robot;
using namespace std;

class Controler
{
private:
    list<Good *> gds;
    list<Robot *> rbts;
    //list<Berth *> brhs;
    int boat_num = 0;
    int free_rbts_nums = 0;

public:
    void init();
    void addRobot(Robot *rbt);
    void addGood(Good *Good);
};
extern Controler ctrls[CTRL_NUM + 2][CTRL_NUM + 2];
