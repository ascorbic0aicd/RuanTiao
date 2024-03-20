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
    friend bool check(Robot rbt, Location &next);
    
    Location loc;
    ROBOT_STATUS status;
    Location target;
    Location next_step;
    vector<int>missing_num;
    list<int> black_list;
    int cd = 0;
    PATH<PATH_TYPE> paths;
    PATH<PATH_TYPE> old_ways;
    int target_id;
public:
    const int id;
    Robot(int id, int x, int y) : id(id), loc(x, y), status(FREE), target(-1, -1),next_step(-1,-1),target_id(-1),missing_num(10,0)
    {
        Location ctrl_id = findCTRL(loc);
        ctrls[ctrl_id.x][ctrl_id.y].addRobot(this);
    }
    inline const Location& getLocation() { return loc; }
    inline ROBOT_STATUS getStatus() { return status; };
    void checkStatus(int x, int y, bool have_good, bool can_move);
    void arrive();
    bool action();
    inline void setStatus(ROBOT_STATUS sts){status = sts;}
    bool to(const Location &target,bool to_berth);
    void comeBack();
    bool findTarget();
    void move();
};

extern vector<Robot *> robots;