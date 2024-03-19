#include "Controler.h"
#include "Robot.h"
#include "Good.h"
#include "marcos.h"
#include <cassert>
#include <algorithm>
#include <random>
#include "Boat.h"
#include "Frame.h"
#define ARG1 15
#define ARG2 5
extern int Time;
extern vector<vector<Cell>> maps;

Controler ctrls[CTRL_NUM + 2][CTRL_NUM + 2];
int Manhadun(int mx, int my, int gx, int gy)
{
    return abs(mx - gx) + abs(my - gy);
}
void Controler::init(int i)
{
    id = i;
    vector<Berth> bth(BERTH_NUM);
    for (int i = 0; i < BERTH_NUM; i++)
    {
        bth[i] = Berths[i];
    }
    sort(bth.begin(), bth.end());
    // sort(brhl.begin(), brhl.end());
    for (int i = 0; i < BOAT_NUM; i++)
    {
        brhs.push_back(bth[i]);
        berth_num++;
    }
    // TODO
}
void Controler::addGood(Good *good)
{
    LOG("addGood Point(%d,%d)\n", good->getX(), good->getY());
    gds.push_back(good);
}
list<Point> Path()
{
    list<Point> p;
    return p;
}
void Controler::Manager()
{
    // LOG("manager\n");
    for (auto r : rbts)
    {
        r->getpull();
        LOG("manager robot id=%d status=%d\n", r->ID(), r->getStatus());
        if (r->getStatus() == FREE)
        {
            Good *gtr = nullptr;
            if (r->StoStatus() == BROKEN)
            {
                Good *gtr = gds.back();
                r->changestoStatus(FREE);
            }
            else
            {
                int mindis = 999999;
                for (auto g : gds)
                {
                    int dis = r->disTO(g->getX(), g->getY());
                    if (dis < mindis && dis < (g->life - Time))
                    {
                        mindis = dis;
                        gtr = g;
                    }
                }
            }

            if (gtr != nullptr)
            {
                // LOG("Robot from Point(%d,%d)\n", r->getX(), r->getY());
                r->path = r->AStarPath(r->loc(), gtr->loc());
                // LOG("Robot from Point(%d,%d)\n", r->getX(), r->getY());
                if (!r->path.empty())
                {
                    r->changeStatus(MOVING);
                }
                removeGood(gtr);
                frames[gtr->Life() % MAGIC_NUMBER].remove(gtr);
            }
        }
        else if (r->getStatus() == HAVE_GOOD && r->path.empty())
        {
            int mindis = 999999;
            Berth btr = brhs[0];
            for (int j = 0; j < berth_num; j++)
            {
                Berth b = brhs[j];
                int dis = r->disTO(b.getX(), b.getY());
                if (dis < mindis)
                {
                    mindis = dis;
                    btr = b;
                }
            }
            LOG("Berth Point(%d,%d)\n", btr.getX(), btr.getY());
            LOG("Robot from Point(%d,%d) %d\n", r->getX(), r->getY(), r->getStatus());
            int bx = btr.getX() + rand() % 4;
            int by = btr.getY() + rand() % 4;
            int berth_id=btr.getID();

            // if (maps[r->getX()][r->getY()].toBerth[berth_id].empty())
            // {
            //     maps[r->getX()][r->getY()].toBerth[berth_id] = r->AStarPath(r->loc(), Point(bx, by));
            //     r->path = maps[r->getX()][r->getY()].toBerth[berth_id];
            // }
            // else
            // {
            //     r->path = maps[r->getX()][r->getY()].toBerth[berth_id];
            // }

            r->path=r->AStarPath(r->loc(), Point(bx, by));

            if (!r->path.empty())
            {
                r->changeStatus(HAVE_GOOD, berth_id);
            }
        }
        r->move();
    }
}

void Controler::addRobot(Robot *rbt)
{
    // LOG("Controler::addRobot\n");
    rbts.push_back(rbt);
    if (rbt->getStatus() == FREE)
    {
        free_rbts_nums++;
    }
}

bool Controler::removeGood(Good *good)
{
    auto it = find(gds.begin(), gds.end(), good);
    if (it == gds.end())
    {
        LOG("can't find the good in CTRL!");
        return false;
    }
    gds.erase(it);
    LOG("good die time = %d, now =%d",good->life, Time);

    return true;
}

void Controler::addBerth(Berth *berth)
{
    brhl.push_back(berth);
}