#pragma once
#define CTRL_NUM 2
#include <list>
#include <vector>
#include "Good.h"
#include "Berth.h"
#include "Location.h"
class Robot;
using namespace std;

class Controler
{
private:
    Location id;
    list<Good *> gds;
    list<Robot *> rbts;
    list<Berth *> brhs;
    int boat_num = 0;
    int free_rbts_nums = 0;
    friend void checkCTRL();

public:
    Controler() : id(-1, -1){};
    Controler *left, *right, *up, *down;
    bool findTraget(Robot *rbt, Location &loc);
    void init(int x, int y);
    void addRobot(Robot *rbt);
    void removeRobot(Robot *rbt);
    void action();
    void addGood(Good *Good);
    bool removeGood(Good *good);
    bool removeGood(Location &loc);
    bool findRobot(Location &loc, Robot **rbt);
};
extern vector<vector<Controler>> ctrls;
void checkCTRL();
inline Location findCTRL(const Location &loc)
{
    return Location((loc.x - 1) / (200 / CTRL_NUM), (loc.y - 1) / (200 / CTRL_NUM));
}
