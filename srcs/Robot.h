#pragma once
#include "Location.h"
#include <vector>
using namespace std;

#define ROBOT_NUM 10
extern int rbt_idx;
#include "Controler.h"
#include <cmath>
#include <queue>
#include "marcos.h"
class Robot
{

    Location loc;
    ROBOT_STATUS status;
    direction step;
    Location target;
    PATH<PATH_TYPE> paths;

public:
    const int id;
    Robot(int id, int x, int y) : id(id), loc(x, y), status(FREE), step(UP), target(-1, -1)
    {
        ctrls[(x - 1) / DIV][(y - 1) / DIV].addRobot(this);
    }
    inline Location getLocation() { return loc; }
    inline ROBOT_STATUS getStatus() { return status; };
    void checkStatus(int x, int y, bool have_good, bool can_move);
    void setTarget(Location &target);
    void action();
    bool to(Location &target);
};

extern vector<Robot *> robots;