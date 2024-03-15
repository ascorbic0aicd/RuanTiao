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
}
list<Point> Path(){

}
void Controler::Manager()
{
    //LOG("manager\n");
    for(auto r:rbts)
    {
        LOG("manager robot id=%d\n", r->ID());
        if (r->getStatus()==FREE)
        {
            int mindis = 999999;
            Good* gtr = nullptr;
            for(auto g:gds)
            {
                int dis =r->disTO(g->getX(),g->getY());
                if(dis<mindis)
                {
                    mindis=dis;
                    gtr=g;
                }
            }

            //r->path = r->AStarPath(r->loc(),gtr->loc());
            r->path=Path();
            r->changeStatus(MOVING);
        }
        else if (r->getStatus()==HAVE_GOOD && r->path.empty())
        {
            int mindis = 999999;
            Berth btr = Berths[0];
            for(int j=0;j<BERTH_NUM;j++)
            {
                Berth b=Berths[j];
                int dis =r->disTO(b.getX(),b.getY());
                if(dis<mindis)
                {
                    mindis=dis;
                    btr=b;
                }
            }
            //r->path = r->AStarPath(r->loc(),btr.loc());
            r->path=Path();
            r->changeStatus(HAVE_GOOD);
        }
        r->action();
    }
}

void Controler::addRobot(Robot *rbt)
{
    //LOG("Controler::addRobot\n");
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