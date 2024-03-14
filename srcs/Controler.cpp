#include "Controler.h"
#include"Robot.h"
#include"Good.h"
#include "marcos.h"
#include<cassert>
#include<algorithm>
#include <random>
#define ARG1 15
#define ARG2 5

Controler ctrls[CTRL_NUM+2][CTRL_NUM + 2];
int Manhadun(int mx, int my, int gx,int gy)
{
    return abs(mx - gx) + abs(my - gy);
}
void Controler::init()
{
    //TODO
}
void Controler:: addGood(Good *good)
{
    gds.push_back(good);
    Robot *ptr = nullptr;
    int mindis = 999999;
    for(auto r: rbts)
    {
        if (r->getStatus()==FREE)
        {
            int dis =r->disTO(good->x,good->y);
            if(dis<mindis)
            {
                mindis=dis;
                ptr=r;
            }
        }
    }
    ptr->path = ptr->AStarPath(ptr->loc(),good->loc());
    ptr->changeStatus(MOVING);
    ptr->action();
}

void Controler::addRobot(Robot *rbt)
{
    rbts.push_back(rbt);
    if (rbt->getStatus() == FREE )
    {
        free_rbts_nums++;
    }
    
}


bool Controler:: removeGood(Good *good)
{
    auto it = find(gds.begin(),gds.end(),good);
    if (it==gds.end())
    {
        LOG("can't find the good in CTRL!");
        return false;
    }
    gds.erase(it);
    return true;
}