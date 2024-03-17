#include "Controler.h"
#include "Robot.h"
#include "Good.h"
#include "marcos.h"
#include <cassert>
#include <algorithm>
#include <random>
#define ARG1 15
#define ARG2 5

vector<vector<Controler>> ctrls(CTRL_NUM, vector<Controler>(CTRL_NUM));

void Controler::init(int x, int y)
{
    // TODO
    right = &ctrls[x][(y + 1) % CTRL_NUM];
    down = &ctrls[(x + 1) % CTRL_NUM][y];
    up = &ctrls[x][y == 0 ? CTRL_NUM - 1 : y - 1];
    left = &ctrls[x == 0 ? CTRL_NUM - 1 : x - 1][y];
}
void Controler::addGood(Good *good)
{
    gds.push_back(good);
    Robot *ptr = nullptr;
    int near = 114514;
    for (auto rbt : rbts)
    {;
        assert(rbt!=nullptr);
        if (rbt->getStatus() == FREE)
        {
            int dis = rbt->getLocation().disTO(good->loc);
            if (dis < near)
            {
                LOG("change to robot(%d,%d) dis =%d\n",rbt->getLocation().x,rbt->getLocation().y,dis);
                ptr = rbt;
                near = dis;
            }
        }
    }
    LOGLOC("succ = false\n");
    bool succ = false;
    if (ptr != nullptr)
    {
        succ = ptr->to(good->loc);
    }
    if (succ)
    {
        ptr->setTarget(good->loc);
    }
    
    LOGLOC("void Controler::addGood(Good *good) will ret!\n");
}
void Controler::addRobot(Robot *rbt)
{
    assert(rbt!=nullptr);
    rbts.push_back(rbt);
    if (rbt->getStatus() == FREE)
    {
        free_rbts_nums++;
    }
}

bool Controler::removeGood(Good *good)
{
    //LOG("mvGood!\n");
    auto it = find(gds.begin(), gds.end(), good);
    if (it == gds.end())
    {
        LOG("can't find the good in CTRL!");
        return false;
    }
    gds.erase(it);
    return true;
}