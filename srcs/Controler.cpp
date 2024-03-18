#include "Controler.h"
#include "Robot.h"
#include "Good.h"
#include "marcos.h"
#include "Cell.h"
#include <cassert>
#include <algorithm>
#include <random>
#define ARG1 15
#define ARG2 5
#define MIN_ARG 20
extern int Time;
vector<vector<Controler>> ctrls(CTRL_NUM, vector<Controler>(CTRL_NUM));

void Controler::init(int x, int y)
{
    // TODO
    id = Location(x, y);
    right = &ctrls[x][(y + 1) % CTRL_NUM];
    down = &ctrls[(x + 1) % CTRL_NUM][y];
    up = &ctrls[x][y == 0 ? CTRL_NUM - 1 : y - 1];
    left = &ctrls[x == 0 ? CTRL_NUM - 1 : x - 1][y];
}
void Controler::addGood(Good *good)
{
    LOG("add good at (%d,%d) in ctrls[%d][%d]\n", good->loc.x, good->loc.y, id.x, id.y);
    gds.push_back(good);

    Robot *ptr = nullptr;
    bool succ = findRobot(good->loc, &ptr);
    if (!succ)
    {
        succ = random() % 2 ? left->findRobot(good->loc, &ptr) : up->findRobot(good->loc, &ptr);
        if (!succ)
        {
            return;
        }
    }
    assert(ptr != nullptr);
    succ = ptr->to(good->loc, false);
    assert(succ);
    removeGood(good->loc);
    LOGLOC("void Controler::addGood(Good *good) will ret!\n");
}
void Controler::addRobot(Robot *rbt)
{
    assert(rbt != nullptr);
    for (auto i : rbts)
    {
        if (rbt->id == i->id)
        {
            LOGERR("add rbt[%d] twice at frame in(%d,%d)?\n", rbt->id, id.x, id.y);
            assert(0);
        }
    }
    LOG("add rbt[%d] at (%d,%d) at frame %d\n", rbt->id, id.x, id.y, Time);
    rbts.push_back(rbt);
    if (rbt->getStatus() == FREE)
    {
        free_rbts_nums++;
    }
}
void Controler::action()
{

    // TO ADD MORE ACTIONS
}
bool Controler::findRobot(Location &loc, Robot **rbt)
{
    LOGLOC("try to find rbt to move good(%d,%d) in[%d][%d] at frame %d\n", loc.x, loc.y, id.x, id.y, Time);
    Robot *ret = nullptr;
    int dis = 1145514;
    for (auto i : rbts)
    {
        if (i->getStatus() == FREE)
        {
            LOGLOC("rbt[%d] is free\n", i->id);
            int temp = i->getLocation().disTO(loc);
            LOGLOC("rbt[%d] at(%d,%d)'s dis = %d\n", i->id, i->getLocation().x, i->getLocation().y, temp);
            if (temp < dis)
            {
                LOGLOC("set ret = rbt[%d]\n", i->id);
                dis = temp;
                ret = i;
            }
        }
    }
    if (ret == nullptr)
    {
        LOG("in [%d][%d] can't find rbt to (%d,%d)\n", id.x, id.y, loc.x, loc.y);
        return false;
    }
    (*rbt) = ret;
    LOG("find the rbt[%d] to (%d,%d)!\n", ret->id, loc.x, loc.y);
    return true;
}
bool Controler::findTraget(Robot *rbt, Location &loc)
{
    Good *good = nullptr;
    int dis = 114514;
    for (auto gd : gds)
    {
        int temp = gd->disTo(rbt->getLocation());
        if ((temp * 2 + Time < gd->life && temp < dis) || temp < MIN_ARG)
        {
            dis = temp;
            good = gd;
        }
    }
    if (good == nullptr)
    {
        return false;
    }
    else
    {
        loc = good->loc;
        return true;
    }
}

void Controler::removeRobot(Robot *rbt)
{
    LOG("rm rbt[%d] at (%d,%d)\n", rbt->id, id.x, id.y);

    auto it = find(rbts.begin(), rbts.end(), rbt);

    Location loc = rbt->getLocation();
    Location temp = findCTRL(loc);
    assert(temp == id);
    if (it == rbts.end())
    {

        LOGERR("can't find the rbt[%d] at(%d,%d) in ctrl[%d][%d]\n", rbt->id, loc.x, loc.y, temp.x, temp.y);
        for (int i = 0; i < CTRL_NUM; i++)
        {
            for (int j = 0; j < CTRL_NUM; j++)
            {
                for (auto it : ctrls[i][j].rbts)
                {
                    if (it->id == rbt->id)
                    {
                        LOGERR("rbt[%d] at ctrl[%d][%d]?\n\n", rbt->id, i, j);
                        for (auto itt : ctrls[i][j].rbts)
                        {
                            LOGERR("rbt[%d] at ctrl[%d][%d]?\n", itt->id, i, j);
                        }
                        assert(0);
                    }
                }
            }
        }

        assert(0);
    }
    rbts.erase(it);
}
bool Controler::removeGood(Location &loc)
{
    LOG("remove (%d,%d) at frame %d by loc\n", loc.x, loc.y, Time);
    LOGLOC("start Controler::removeGood(Location &loc)\n");
    for (auto gd = gds.begin(); gd != gds.end(); gd++)
    {
        if ((*gd)->loc == loc)
        {
            Good *del = *gd;
            gds.erase(gd);
            bool succ = frames[(*gd)->life % MAGIC_NUMBER].removeGood(del->loc);
            assert(succ);
            delete del;
            LOGLOC("end Controler::removeGood(Location &loc)\n");
            return true;
        }
    }
    Location temp = findCTRL(loc);
    assert(temp == id);
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            for (auto gd : ctrls[i][j].gds)
            {
                if (gd->loc == loc)
                {
                    LOGERR("good (%d,%d) at ctrl[%d][%d]?\n", loc.x, loc.y, i, j);
                }
            }
        }
    }

    LOGERR("can't find the  at(%d,%d) in ctrl[%d][%d]\n", loc.x, loc.y, temp.x, temp.y);
    // LOGERR("no good at (%d,%d)?\n", loc.x, loc.y);
    return false;
}
bool Controler::removeGood(Good *good)
{
    // LOG("mvGood!\n");
    LOG("remove (%d,%d) at frame %d by good*\n", good->loc.x, good->loc.y, Time);
    auto it = find(gds.begin(), gds.end(), good);
    if (it == gds.end())
    {
        LOG("can't find the good in CTRL[%d][%d]!\n", id.x, id.y);
        return false;
    }
    gds.erase(it);
    return true;
}
void checkCTRL()
{
    vector<bool> flags(ROBOT_NUM, false);

    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            for (auto rbt : ctrls[i][j].rbts)
            {
                LOGLOC("Find rbt[%d] at(%d,%d)\n", rbt->id, i, j);
                if (flags[rbt->id])
                {
                    LOGERR("rbt[%d] find twice\n", rbt->id);
                    assert(0);
                }
                flags[rbt->id] = true;
            }
        }
    }
    for (int i = 0; i < ROBOT_NUM; i++)
    {
        if (!flags[i])
        {
            LOGERR("can't find rbt[%d]\n", i);
            assert(0);
        }
    }
}
