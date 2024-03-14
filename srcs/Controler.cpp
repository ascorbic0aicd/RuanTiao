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

void Controler::init()
{
    //TODO
}
void Controler:: addGood(Good *good)
{
    gds.push_back(good);
    Robot *ptr = nullptr;
    for(auto r: rbts)
    {
        if (r->getStatus()==FREE)
        {
            if (ptr == nullptr)
            {
                ptr = r;
            }
            else
            {
                if (r->disTO(good->x,good->y) - r->disTO(good->x, good->y) > ARG1)
                {
                    ptr = r;
                }
                else if (abs(r->disTO(good->x,good->y) - r->disTO(good->x, good->y)) < ARG2)
                {
                    if (random()%2)
                    {
                        ptr = r;
                    }
                    
                }
            }
        }
    }
    
    
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