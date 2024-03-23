#include "Controler.h"
#include "Robot.h"
#include "Good.h"
#include "marcos.h"
#include "Cell.h"
#include "Boat.h"
#include <cassert>
#include <algorithm>
#include <random>
#include <cmath>
#define ARG1 15
#define ARG2 5
#define MIN_ARG 20
extern int Time;
const int para4 = 2;
const int para5 = 1;
vector<vector<Controler>> ctrls(CTRL_NUM, vector<Controler>(CTRL_NUM));

int evalprio(int x, int y)
{
    // LOGERR("x=%d, y=%d\n",x,y);
    // assert(0);
    // return max(x * para4, y * para5);

    return x * para4 + y * para5;
}

void Controler::init(int x, int y) // 1
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
    removeGood(good->loc);
    LOGLOC("void Controler::addGood(Good *good) will ret!\n");
}
void Controler::addRobot(Robot *rbt) // 2
{
    assert(rbt != nullptr);
    for (auto i : rbts)
    {
        if (rbt->id == i->id)
        {
            LOG("add rbt[%d] twice at frame in(%d,%d)?\n", rbt->id, id.x, id.y);
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
void Controler::addBerth(Berth *berth) // 3
{
    brhs.push_back(berth);
    // berth_num++;
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
    LOG("try to find good for rbt[%d] at (%d,%d) in ctrls[%d][%d]\n", rbt->id, rbt->getLocation().x, rbt->getLocation().y, id.x, id.y);
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
        LOG("find fail!\n");
        return false;
    }
    else
    {
        LOG("find succ!\n");
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

        LOG("can't find the rbt[%d] at(%d,%d) in ctrl[%d][%d]\n", rbt->id, loc.x, loc.y, temp.x, temp.y);
        for (int i = 0; i < CTRL_NUM; i++)
        {
            for (int j = 0; j < CTRL_NUM; j++)
            {
                for (auto it : ctrls[i][j].rbts)
                {
                    if (it->id == rbt->id)
                    {
                        LOG("rbt[%d] at ctrl[%d][%d]?\n\n", rbt->id, i, j);
                        for (auto itt : ctrls[i][j].rbts)
                        {
                            LOG("rbt[%d] at ctrl[%d][%d]?\n", itt->id, i, j);
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
                    LOG("good (%d,%d) at ctrl[%d][%d]?\n", loc.x, loc.y, i, j);
                }
            }
        }
    }

    LOG("can't find the  at(%d,%d) in ctrl[%d][%d]\n", loc.x, loc.y, temp.x, temp.y);
    // LOG("no good at (%d,%d)?\n", loc.x, loc.y);
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
                    LOG("rbt[%d] find twice\n", rbt->id);
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
            LOG("can't find rbt[%d]\n", i);
            assert(0);
        }
    }
}

void redistribution1()
{
    int b1 = 0, b2 = 1, b3 = 2, b4 = 6, b5 = 9;
    choose_bid.push_back(b1);
    choose_bid.push_back(b2);
    choose_bid.push_back(b3);
    choose_bid.push_back(b4);
    choose_bid.push_back(b5);

    list<Berth *> temp[4];
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            for (auto it : ctrls[i][j].brhs)
            {
                if (it->ID == b1 || it->ID == b2 || it->ID == b3 || it->ID == b4 || it->ID == b5)
                {
                    temp[i * CTRL_NUM + j].push_back(it);
                }
            }
        }
    }
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            ctrls[i][j].brhs.clear();
            for (auto it : temp[i * CTRL_NUM + j])
            {
                ctrls[i][j].brhs.push_back(it);
                it->ctrl_id=Location(i,j);
            }
        }
    }

    // robot
    list<Robot *> tempr[4];
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            for (auto it : ctrls[i][j].rbts)
            {
                switch (it->id)
                {
                case 0:
                case 4:
                case 7:
                    tempr[0].push_back(it);
                    it->bth_id = 0;
                    break;
                
                case 9:
                    tempr[0].push_back(it);
                    it->bth_id = 2;
                    break;
                case 1:
                case 2:
                    tempr[1].push_back(it);
                    it->bth_id = 1;
                    break;
                case 5:
                case 6:
                    tempr[2].push_back(it);
                    it->bth_id = 6;
                    break;
                case 3:
                case 8:
                    tempr[3].push_back(it);
                    it->bth_id = 9;
                    break;
                default:
                    break;
                }
            }
        }
    }

    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            ctrls[i][j].rbts.clear();
            for (auto it : tempr[i * CTRL_NUM + j])
            {
                ctrls[i][j].rbts.push_back(it);
                it->ctrl_id = Location(i, j);
            }

            LOGERR("ctrls[%d][%d] has robots %d berth %d\n", i, j, ctrls[i][j].rbts.size(), ctrls[i][j].brhs.size());
            for (auto r : ctrls[i][j].rbts)
            {
                LOGERR("robots id=%d to berth %d\n", r->id, r->bth_id);
            }
            for (auto b : ctrls[i][j].brhs)
            {
                LOGERR("berth id=%d\n", b->ID);
                assert(Berths[b->ID].loc == b->loc);
            }
        }
    }
    // assert(0);
}

void redistribution2()
{
    int b1 = 0, b2 = 2, b3 = 6, b4 = 7, b5 = 9;
    choose_bid.push_back(b1);
    choose_bid.push_back(b2);
    choose_bid.push_back(b3);
    choose_bid.push_back(b4);
    choose_bid.push_back(b5);

    list<Berth *> temp[4];
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            for (auto it : ctrls[i][j].brhs)
            {
                if (it->ID == b1 || it->ID == b2 || it->ID == b3 || it->ID == b4 || it->ID == b5)
                {
                    temp[i * CTRL_NUM + j].push_back(it);
                }
            }
            ctrls[i][j].brhs.clear();
            for (auto it : temp[i * CTRL_NUM + j])
            {
                ctrls[i][j].brhs.push_back(it);
                it->ctrl_id=Location(i,j);
            }
        }
    }

    // robot
    list<Robot *> tempr[4];
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            for (auto it : ctrls[i][j].rbts)
            {
                switch (it->id)
                {
                case 0:
                case 1:
                case 2:
                case 3:
                    tempr[1].push_back(it);
                    it->bth_id = 0;
                    break;
                case 6:
                case 5:
                case 4:
                    tempr[1].push_back(it);
                    it->bth_id = 2;
                    break;
                case 7:
                
                    tempr[2].push_back(it);
                    it->bth_id = 7;
                    break;
                case 9:
                    tempr[3].push_back(it);
                    it->bth_id = 9;
                    break;
                
                case 8:
                
                    tempr[3].push_back(it);
                    it->bth_id = 6;
                    break;
                default:
                    break;
                }
            }
        }
    }

    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            ctrls[i][j].rbts.clear();
            for (auto it : tempr[i * CTRL_NUM + j])
            {
                ctrls[i][j].rbts.push_back(it);
                it->ctrl_id = Location(i, j);
            }

            LOGERR("ctrls[%d][%d] has robots %d berth %d\n", i, j, ctrls[i][j].rbts.size(), ctrls[i][j].brhs.size());
            for (auto r : ctrls[i][j].rbts)
            {
                LOGERR("robots id=%d to berth %d\n", r->id, r->bth_id);
            }
            for (auto b : ctrls[i][j].brhs)
            {
                LOGERR("berth id=%d\n", b->ID);
                assert(Berths[b->ID].loc == b->loc);
            }
        }
    }
    // assert(0);
}

void redistribution3()
{
    int noberth = 0, oneberth = 0, manyberth = 0;
    list<Berth *> temp[4];
    Location randcenter[4];
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            int dx = i * (200 / CTRL_NUM) + 200 / CTRL_NUM / 3;
            int dy = j * (200 / CTRL_NUM) + 200 / CTRL_NUM / 3;
            int cx, cy;
            do
            {
                cx = rand() % (200 / CTRL_NUM / 3);
                cy = rand() % (200 / CTRL_NUM / 3);
                // LOG("maps[%d][%d]\n",dx+cx,dy+cy);
                assert(dx + cx >= 1 && dx + cx <= 200 && dy + cy >= 1 && dy + cy <= 200);
            } while (maps[dx + cx][dy + cy].getType() == BARRIER || maps[dx + cx][dy + cy].getType() == SEA);
            assert(maps[dx + cx][dy + cy].getType() != BARRIER && maps[dx + cx][dy + cy].getType() != SEA);
            randcenter[i * CTRL_NUM + j] = Location(dx + cx, dy + cy);
        }
    }

    vector<pair<int, int>> nullctrl;

    int min = 100000000;
    Berth *ptr = nullptr;
    PATH<PATH_TYPE> path;
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            if (ctrls[i][j].brhs.size() == 1)
            {
                oneberth++;
                choose_bid.push_back(ctrls[i][j].brhs.front()->ID);
                LOGERR("choose berth %d\n", ptr->ID);
                temp[i * CTRL_NUM + j].push_back(ctrls[i][j].brhs.front());
                ctrls[i][j].brhs.clear();
                LOG("one\n");
            }
            else if (ctrls[i][j].brhs.size() >= 2)
            {
                manyberth++;
                LOG("two\n\n");
                min = 100000000;
                ptr = nullptr;
                for (auto b : ctrls[i][j].brhs)
                {
                    path.clear();
                    LOG("randcenter[%d]=(%d,%d),type=%d\n", i * CTRL_NUM + j, randcenter[i * CTRL_NUM + j].x, randcenter[i * CTRL_NUM + j].y, maps[randcenter[i * CTRL_NUM + j].x][randcenter[i * CTRL_NUM + j].y].getType());
                    Location temploc = b->loc;
                    assert(b->inArea(b->loc));
                    while (!randcenter[i * CTRL_NUM + j].findPath(randcenter[i * CTRL_NUM + j], temploc, path, b->ID, false))
                    {
                        int dx = i * (200 / CTRL_NUM) + 200 / CTRL_NUM / 4;
                        int dy = j * (200 / CTRL_NUM) + 200 / CTRL_NUM / 4;
                        int cx, cy;
                        do
                        {
                            cx = rand() % (200 / CTRL_NUM / 2);
                            cy = rand() % (200 / CTRL_NUM / 2);
                            // LOG("maps[%d][%d]\n",dx+cx,dy+cy);
                            assert(dx + cx >= 1 && dx + cx <= 200 && dy + cy >= 1 && dy + cy <= 200);
                        } while (maps[dx + cx][dy + cy].getType() == BARRIER || maps[dx + cx][dy + cy].getType() == SEA);
                        assert(maps[dx + cx][dy + cy].getType() != BARRIER && maps[dx + cx][dy + cy].getType() != SEA);
                        randcenter[i * CTRL_NUM + j] = Location(dx + cx, dy + cy);
                        // int prio=b->priority * para4 + path.size() * boat_capacity / 2 * para5;
                    }

                    int prio = evalprio(b->evalWeight(), (int)path.size() * boat_capacity / 2);
                    LOGERR("id:%d,prio=%d,b->evalWeight()=%d * para4=%d, path.size() * boat_capacity / 2=%d *para5= %d\n", b->ID, prio, b->evalWeight(), b->evalWeight() * para4, path.size() * boat_capacity / 2, path.size() * boat_capacity / 2 * para5);
                    // assert(0);
                    if (prio < min)
                    {
                        min = prio;
                        ptr = b;
                    }
                }
                if (ptr != nullptr)
                {
                    LOGERR("choose berth %d\n", ptr->ID);
                    choose_bid.push_back(ptr->ID);
                    LOG("choose berth %d\n", ptr->ID);
                    temp[i * CTRL_NUM + j].push_back(ptr);
                    auto it = find(ctrls[i][j].brhs.begin(), ctrls[i][j].brhs.end(), ptr);
                    ctrls[i][j].brhs.erase(it);
                }
                else
                {
                    assert(0);
                }
            }
            else
            {
                noberth++;
                nullctrl.push_back(pair<int, int>(i, j));
            }
        }
    }
    assert(noberth + oneberth + manyberth == 4);
    // LOG("noberth+oneberth+manyberth=%d\n",noberth+oneberth+manyberth);

    Controler *ctr = nullptr;
    LOG("nullctrl num=%d\n", nullctrl.size());
    for (auto it : nullctrl)
    {
        Controler &null_ctrl = ctrls[it.first][it.second];
        vector<Controler *> nei = {null_ctrl.left, null_ctrl.right};
        ptr = nullptr;
        min = 1000000000;
        path.clear();
        for (auto ne : nei)
        {
            if (!ne->brhs.empty())
            {
                LOG("there is b\n");
                for (auto b : ne->brhs)
                {
                    LOG("randcenter[%d]=(%d,%d)\n", it.first * CTRL_NUM + it.second, randcenter[it.first * CTRL_NUM + it.second].x, randcenter[it.first * CTRL_NUM + it.second].y);
                    Location temploc = b->loc;
                    if (randcenter[it.first * CTRL_NUM + it.second].findPath(randcenter[it.first * CTRL_NUM + it.second], temploc, path, b->ID, false))
                    {
                        // LOG("path.size()=%d\n",path.size());
                        int prio = evalprio(b->evalWeight(), (int)path.size() * boat_capacity / 2);
                        LOGERR("id:%d,prio=%d,b->evalWeight()=%d * para4=%d, path.size() * boat_capacity / 2=%d *para5= %d\n", b->ID, prio, b->evalWeight(), b->evalWeight() * para4, path.size() * boat_capacity / 2, path.size() * boat_capacity / 2 * para5);
                        if (prio < min)
                        {
                            min = prio;
                            ptr = b;
                            ctr = ne;
                        }
                    }
                }
            }
        }
        if (ptr != nullptr)
        {
            LOGERR("choose near berth %d\n", ptr->ID);
            choose_bid.push_back(ptr->ID);
            LOG("choose berth %d\n", ptr->ID);
            temp[it.first * CTRL_NUM + it.second].push_back(ptr);

            auto goal = find(ctr->brhs.begin(), ctr->brhs.end(), ptr);
            if (goal != null_ctrl.left->brhs.end())
            {
                ctr->brhs.erase(goal);
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            assert(0);
        }
    }

    LOG("choose_bid.size=%d\n", choose_bid.size());
    assert(choose_bid.size() == 4);

    ptr = nullptr;
    ctr = nullptr;
    min = 1000000000;
    path.clear();
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            if (!ctrls[i][j].brhs.empty())
            {
                for (auto b : ctrls[i][j].brhs)
                {
                    Location temploc = b->loc;
                    if (randcenter[i * CTRL_NUM + j].findPath(randcenter[i * CTRL_NUM + j], temploc, path, b->ID, false))
                    {
                        int prio = evalprio(b->evalWeight(), (int)path.size() * boat_capacity / 2);
                        if (prio < min)
                        {
                            min = prio;
                            ptr = b;
                            ctr = &ctrls[i][j];
                        }
                    }
                }
            }
        }
    }
    if (ptr != nullptr)
    {
        choose_bid.push_back(ptr->ID);
        LOG("choose berth %d\n", ptr->ID);

        for (int i = 0; i < CTRL_NUM; i++)
        {
            for (int j = 0; j < CTRL_NUM; j++)
            {
                ctrls[i][j].brhs.clear();
                assert(temp[i * CTRL_NUM + j].size() == 1);
                ctrls[i][j].brhs.push_back(temp[i * CTRL_NUM + j].front());
            }
        }
        ctr->brhs.push_back(ptr);
    }
    else
    {
        assert(0);
    }

    // robot
    list<Robot *> tempr[4];
    list<pair<pair<int, int>, int>> need;

    path.clear();
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            LOG("i=%d,j=%d berth num=%d, robot num=%d\n", i, j, ctrls[i][j].brhs.size(), ctrls[i][j].rbts.size());
            if (ctrls[i][j].brhs.size() * 2 <= ctrls[i][j].rbts.size())
            {
                int k = ctrls[i][j].brhs.size() * 2;
                for (auto it = ctrls[i][j].rbts.begin(); it != ctrls[i][j].rbts.end();)
                {
                    assert(!ctrls[i][j].brhs.empty());
                    Location temploc = ctrls[i][j].brhs.front()->loc;
                    if ((*it)->loc.findPath((*it)->loc, temploc, path, ctrls[i][j].brhs.front()->ID, false))
                    {
                        tempr[i * CTRL_NUM + j].push_back(*it);
                        it = ctrls[i][j].rbts.erase(it);
                        LOG("add robot\n");
                        k--;
                    }
                    else
                    {
                        ++it;
                        k--;
                    }
                    if (k == 0)
                    {
                        break;
                    }
                }
                if (k > 0)
                { // there are/is robot can't go to the berth;
                    assert(0);
                }
            }
            else if (ctrls[i][j].brhs.size() * 2 > ctrls[i][j].rbts.size())
            {
                int k = ctrls[i][j].rbts.size();
                int p = ctrls[i][j].brhs.size() * 2 - ctrls[i][j].rbts.size();
                LOG("k=%d,p=%d\n", k, p);

                for (auto it = ctrls[i][j].rbts.begin(); it != ctrls[i][j].rbts.end();)
                {
                    Location temploc = ctrls[i][j].brhs.front()->loc;
                    if ((*it)->loc.findPath((*it)->loc, temploc, path, ctrls[i][j].brhs.front()->ID, false))
                    {
                        tempr[i * CTRL_NUM + j].push_back(*it);
                        LOG("add robot\n");
                        it = ctrls[i][j].rbts.erase(it);
                        k--;
                    }
                    else
                    {
                        ++it;
                        k--;
                    }
                    if (k == 0)
                    {
                        break;
                    }
                }
                if (k > 0)
                {
                    assert(0);
                }
                need.push_back(pair<pair<int, int>, int>(pair<int, int>(i, j), p));
                LOG("need i=%d,j=%d\n", i, j);
            }
        }
    }
    LOG("num of need=%d\n", need.size());

    for (auto l : need)
    {
        Controler &ctrl = ctrls[l.first.first][l.first.second];
        LOG("ctrls[%d][%d]\n", l.first.first, l.first.second);
        vector<Controler *> nei = {ctrl.up, ctrl.left, ctrl.left->up};
        int k = l.second;
        for (auto ne : nei)
        {
            if (!ne->rbts.empty())
            {
                for (auto it = ne->rbts.begin(); it != ne->rbts.end() && k > 0;)
                {
                    Location temploc = ctrl.brhs.front()->loc;
                    if ((*it)->loc.findPath((*it)->loc, temploc, path, ctrl.brhs.front()->ID, false))
                    {
                        tempr[l.first.first * CTRL_NUM + l.first.second].push_back(*it);
                        it = ne->rbts.erase(it);
                        LOG("add robot\n");
                        k--;
                    }
                    else
                    {
                        ++it;
                        k--;
                    }
                }
            }
            if (k == 0)
            {
                break;
            }
        }
        if (k > 0)
        {
            // assert(0);
        }
    }

    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            // assert(ctrls[i][j].rbts.empty());
            // ctrls[i][j].rbts.clear();
            int k = 0;
            for (auto it : tempr[i * CTRL_NUM + j])
            {
                ctrls[i][j].rbts.push_back(it);
                it->ctrl_id = Location(i, j);

                it->bth_id = ctrls[i][j].brhs[k / 2]->getID();
                k++;
            }
            LOGERR("ctrls[%d][%d] has robots %d berth %d\n", i, j, ctrls[i][j].rbts.size(), ctrls[i][j].brhs.size());
            for (auto r : ctrls[i][j].rbts)
            {
                LOGERR("robots id=%d to berth %d\n", r->id, r->bth_id);
            }
            for (auto b : ctrls[i][j].brhs)
            {
                LOGERR("berth id=%d\n", b->ID);
                assert(Berths[b->ID].loc == b->loc);
            }
        }
    }
    // assert(0);
}

void redistribution()
{
    int noberth = 0, oneberth = 0, manyberth = 0;
    list<Berth *> temp[4];
    Location randcenter[4];
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            int dx = i * (200 / CTRL_NUM) + 200 / CTRL_NUM / 3;
            int dy = j * (200 / CTRL_NUM) + 200 / CTRL_NUM / 3;
            int cx, cy;
            do
            {
                cx = rand() % (200 / CTRL_NUM / 3);
                cy = rand() % (200 / CTRL_NUM / 3);
                // LOG("maps[%d][%d]\n",dx+cx,dy+cy);
                assert(dx + cx >= 1 && dx + cx <= 200 && dy + cy >= 1 && dy + cy <= 200);
            } while (maps[dx + cx][dy + cy].getType() == BARRIER || maps[dx + cx][dy + cy].getType() == SEA);
            assert(maps[dx + cx][dy + cy].getType() != BARRIER && maps[dx + cx][dy + cy].getType() != SEA);
            randcenter[i * CTRL_NUM + j] = Location(dx + cx, dy + cy);
        }
    }

    vector<pair<int, int>> nullctrl;

    int min = 100000000;
    Berth *ptr = nullptr;
    PATH<PATH_TYPE> path;
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            if (ctrls[i][j].brhs.size() == 1)
            {
                oneberth++;
                choose_bid.push_back(ctrls[i][j].brhs.front()->ID);
                LOGERR("choose berth %d\n", ptr->ID);
                temp[i * CTRL_NUM + j].push_back(ctrls[i][j].brhs.front());
                ctrls[i][j].brhs.clear();
                LOG("one\n");
            }
            else if (ctrls[i][j].brhs.size() >= 2)
            {
                manyberth++;
                LOG("two\n\n");
                min = 100000000;
                ptr = nullptr;
                for (auto b : ctrls[i][j].brhs)
                {
                    path.clear();
                    LOG("randcenter[%d]=(%d,%d),type=%d\n", i * CTRL_NUM + j, randcenter[i * CTRL_NUM + j].x, randcenter[i * CTRL_NUM + j].y, maps[randcenter[i * CTRL_NUM + j].x][randcenter[i * CTRL_NUM + j].y].getType());
                    Location temploc = b->loc;
                    assert(b->inArea(b->loc));
                    while (!randcenter[i * CTRL_NUM + j].findPath(randcenter[i * CTRL_NUM + j], temploc, path, b->ID, false))
                    {
                        int dx = i * (200 / CTRL_NUM) + 200 / CTRL_NUM / 4;
                        int dy = j * (200 / CTRL_NUM) + 200 / CTRL_NUM / 4;
                        int cx, cy;
                        do
                        {
                            cx = rand() % (200 / CTRL_NUM / 2);
                            cy = rand() % (200 / CTRL_NUM / 2);
                            // LOG("maps[%d][%d]\n",dx+cx,dy+cy);
                            assert(dx + cx >= 1 && dx + cx <= 200 && dy + cy >= 1 && dy + cy <= 200);
                        } while (maps[dx + cx][dy + cy].getType() == BARRIER || maps[dx + cx][dy + cy].getType() == SEA);
                        assert(maps[dx + cx][dy + cy].getType() != BARRIER && maps[dx + cx][dy + cy].getType() != SEA);
                        randcenter[i * CTRL_NUM + j] = Location(dx + cx, dy + cy);
                        // int prio=b->priority * para4 + path.size() * boat_capacity / 2 * para5;
                    }

                    int prio = evalprio(b->evalWeight(), (int)path.size() * boat_capacity / 2);
                    LOGERR("id:%d,prio=%d,b->evalWeight()=%d * para4=%d, path.size() * boat_capacity / 2=%d *para5= %d\n", b->ID, prio, b->evalWeight(), b->evalWeight() * para4, path.size() * boat_capacity / 2, path.size() * boat_capacity / 2 * para5);
                    // assert(0);
                    if (prio < min)
                    {
                        min = prio;
                        ptr = b;
                    }
                }
                if (ptr != nullptr)
                {
                    LOGERR("choose berth %d\n", ptr->ID);
                    choose_bid.push_back(ptr->ID);
                    LOG("choose berth %d\n", ptr->ID);
                    temp[i * CTRL_NUM + j].push_back(ptr);
                    auto it = find(ctrls[i][j].brhs.begin(), ctrls[i][j].brhs.end(), ptr);
                    ctrls[i][j].brhs.erase(it);
                }
                else
                {
                    assert(0);
                }
            }
            else
            {
                noberth++;
                nullctrl.push_back(pair<int, int>(i, j));
            }
        }
    }
    assert(noberth + oneberth + manyberth == 4);
    // LOG("noberth+oneberth+manyberth=%d\n",noberth+oneberth+manyberth);

    Controler *ctr = nullptr;
    // LOG("nullctrl num=%d\n", nullctrl.size());
    // for (auto it : nullctrl)
    // {
    //     Controler &null_ctrl = ctrls[it.first][it.second];
    //     vector<Controler *> nei = {null_ctrl.left, null_ctrl.right};
    //     ptr = nullptr;
    //     min = 1000000000;
    //     path.clear();
    //     for (auto ne : nei)
    //     {
    //         if (!ne->brhs.empty())
    //         {
    //             LOG("there is b\n");
    //             for (auto b : ne->brhs)
    //             {
    //                 LOG("randcenter[%d]=(%d,%d)\n", it.first * CTRL_NUM + it.second, randcenter[it.first * CTRL_NUM + it.second].x, randcenter[it.first * CTRL_NUM + it.second].y);
    //                 Location temploc=b->loc;
    //                 if (randcenter[it.first * CTRL_NUM + it.second].findPath(randcenter[it.first * CTRL_NUM + it.second], temploc, path, b->ID, false))
    //                 {
    //                     // LOG("path.size()=%d\n",path.size());
    //                     int prio = evalprio(b->evalWeight(), (int)path.size() * boat_capacity / 2);
    //                     LOGERR("id:%d,prio=%d,b->evalWeight()=%d * para4=%d, path.size() * boat_capacity / 2=%d *para5= %d\n", b->ID, prio ,b->evalWeight(), b->evalWeight()*para4, path.size() * boat_capacity / 2, path.size() * boat_capacity / 2 * para5);
    //                     if (prio < min)
    //                     {
    //                         min = prio;
    //                         ptr = b;
    //                         ctr = ne;

    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     if (ptr != nullptr)
    //     {
    //         LOGERR("choose near berth %d\n",ptr->ID);
    //         choose_bid.push_back(ptr->ID);
    //         LOG("choose berth %d\n", ptr->ID);
    //         temp[it.first * CTRL_NUM + it.second].push_back(ptr);

    //         auto goal = find(ctr->brhs.begin(), ctr->brhs.end(), ptr);
    //         if (goal != null_ctrl.left->brhs.end())
    //         {
    //             ctr->brhs.erase(goal);
    //         }
    //         else
    //         {
    //             assert(0);
    //         }
    //     }
    //     else
    //     {
    //         assert(0);
    //     }
    // }

    LOG("choose_bid.size=%d\n", choose_bid.size());
    assert(5 - choose_bid.size() == noberth + 1);

    for (int k = 0; k < noberth + 1; k++)
    {
        ptr = nullptr;
        ctr = nullptr;
        min = 1000000000;
        path.clear();
        pair<int, int> cloc;

        for (int i = 0; i < CTRL_NUM; i++)
        {
            for (int j = 0; j < CTRL_NUM; j++)
            {
                if (!ctrls[i][j].brhs.empty())
                {
                    for (auto b : ctrls[i][j].brhs)
                    {
                        Location temploc = b->loc;
                        if (randcenter[i * CTRL_NUM + j].findPath(randcenter[i * CTRL_NUM + j], temploc, path, b->ID, false))
                        {
                            int prio = evalprio(b->evalWeight(), (int)path.size() * boat_capacity / 2);
                            LOGERR("id:%d,prio=%d,b->evalWeight()=%d * para4=%d, path.size() * boat_capacity / 2=%d *para5= %d\n", b->ID, prio, b->evalWeight(), b->evalWeight() * para4, path.size() * boat_capacity / 2, path.size() * boat_capacity / 2 * para5);
                            if (prio < min)
                            {
                                min = prio;
                                ptr = b;
                                ctr = &ctrls[i][j];
                                cloc = pair<int, int>(i, j);
                            }
                        }
                    }
                }
            }
        }

        if (ptr != nullptr)
        {
            choose_bid.push_back(ptr->ID);
            LOGERR("choose best berth %d\n", ptr->ID);

            temp[cloc.first * CTRL_NUM + cloc.second].push_back(ptr);
            auto it = find(ctr->brhs.begin(), ctr->brhs.end(), ptr);
            ctr->brhs.erase(it);
        }
        else
        {
            assert(0);
        }
    }
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            ctrls[i][j].brhs.clear();
            for (auto it : temp[i * CTRL_NUM + j])
            {
                ctrls[i][j].brhs.push_back(it);
            }
        }
    }

    // if (ptr != nullptr)
    // {
    //     choose_bid.push_back(ptr->ID);
    //     LOG("choose berth %d\n", ptr->ID);

    //     for (int i = 0; i < CTRL_NUM; i++)
    //     {
    //         for (int j = 0; j < CTRL_NUM; j++)
    //         {
    //             ctrls[i][j].brhs.clear();
    //             assert(temp[i * CTRL_NUM + j].size() == 1);
    //             ctrls[i][j].brhs.push_back(temp[i * CTRL_NUM + j].front());
    //         }
    //     }
    //     ctr->brhs.push_back(ptr);
    // }
    // else
    // {
    //     assert(0);
    // }

    // robot
    list<Robot *> tempr[4];
    list<pair<pair<int, int>, int>> need;

    path.clear();
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            LOG("i=%d,j=%d berth num=%d, robot num=%d\n", i, j, ctrls[i][j].brhs.size(), ctrls[i][j].rbts.size());
            if (!ctrls[i][j].brhs.empty() && ctrls[i][j].brhs.size() * 2 <= ctrls[i][j].rbts.size())
            {
                int k = ctrls[i][j].brhs.size() * 2;
                for (auto it = ctrls[i][j].rbts.begin(); it != ctrls[i][j].rbts.end();)
                {
                    assert(!ctrls[i][j].brhs.empty());
                    Location temploc = ctrls[i][j].brhs.front()->loc;
                    if ((*it)->loc.findPath((*it)->loc, temploc, path, ctrls[i][j].brhs.front()->ID, false))
                    {
                        tempr[i * CTRL_NUM + j].push_back(*it);
                        it = ctrls[i][j].rbts.erase(it);
                        LOG("add robot\n");
                        k--;
                    }
                    else
                    {
                        ++it;
                        k--;
                    }
                    if (k == 0)
                    {
                        break;
                    }
                }
                if (k > 0)
                { // there are/is robot can't go to the berth;
                    assert(0);
                }
            }
            else if (ctrls[i][j].brhs.size() * 2 > ctrls[i][j].rbts.size())
            {
                int k = ctrls[i][j].rbts.size();
                int p = ctrls[i][j].brhs.size() * 2 - ctrls[i][j].rbts.size();
                LOG("k=%d,p=%d\n", k, p);

                for (auto it = ctrls[i][j].rbts.begin(); it != ctrls[i][j].rbts.end();)
                {
                    Location temploc = ctrls[i][j].brhs.front()->loc;
                    if ((*it)->loc.findPath((*it)->loc, temploc, path, ctrls[i][j].brhs.front()->ID, false))
                    {
                        tempr[i * CTRL_NUM + j].push_back(*it);
                        LOG("add robot\n");
                        it = ctrls[i][j].rbts.erase(it);
                        k--;
                    }
                    else
                    {
                        ++it;
                        k--;
                    }
                    if (k == 0)
                    {
                        break;
                    }
                }
                if (k > 0)
                {
                    assert(0);
                }
                need.push_back(pair<pair<int, int>, int>(pair<int, int>(i, j), p));
                LOG("need i=%d,j=%d\n", i, j);
            }
        }
    }
    LOG("num of need=%d\n", need.size());

    for (auto l : need)
    {
        Controler &ctrl = ctrls[l.first.first][l.first.second];
        LOG("ctrls[%d][%d]\n", l.first.first, l.first.second);
        vector<Controler *> nei = {ctrl.up, ctrl.left, ctrl.left->up};
        int k = l.second;
        for (auto ne : nei)
        {
            if (!ne->rbts.empty())
            {
                for (auto it = ne->rbts.begin(); it != ne->rbts.end() && k > 0;)
                {
                    Location temploc = ctrl.brhs.front()->loc;
                    if ((*it)->loc.findPath((*it)->loc, temploc, path, ctrl.brhs.front()->ID, false))
                    {
                        tempr[l.first.first * CTRL_NUM + l.first.second].push_back(*it);
                        it = ne->rbts.erase(it);
                        LOG("add robot\n");
                        k--;
                    }
                    else
                    {
                        ++it;
                        k--;
                    }
                }
            }
            if (k == 0)
            {
                break;
            }
        }
        if (k > 0)
        {
            // assert(0);
        }
    }

    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            // assert(ctrls[i][j].rbts.empty());
            // ctrls[i][j].rbts.clear();
            int k = 0;
            for (auto it : tempr[i * CTRL_NUM + j])
            {
                ctrls[i][j].rbts.push_back(it);
                it->ctrl_id = Location(i, j);

                it->bth_id = ctrls[i][j].brhs[k / 2]->getID();
                k++;
            }
            LOGERR("ctrls[%d][%d] has robots %d berth %d\n", i, j, ctrls[i][j].rbts.size(), ctrls[i][j].brhs.size());
            for (auto r : ctrls[i][j].rbts)
            {
                LOGERR("robots id=%d to berth %d\n", r->id, r->bth_id);
            }
            for (auto b : ctrls[i][j].brhs)
            {
                LOGERR("berth id=%d\n", b->ID);
                assert(Berths[b->ID].loc == b->loc);
            }
        }
    }
    // assert(0);
}

void robot_redistribute(int berth_ID)
{
    
    Location loc = Berths[berth_ID].ctrl_id;
    Controler &ctrl = ctrls[loc.x][loc.y];
    //LOGERR("the berth %d in ctrl[%d][%d]\n",berth_ID,loc.x,loc.y);

    int min = 100000;
    Controler *ctr = nullptr;
    int bid = 0;
    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            for (auto it : ctrls[i][j].brhs)
            {
                int prio=it->evalWeight() + Berths[berth_ID].loc.disTO(it->loc) * 2;
                if (prio < min)
                {
                    min = prio;
                    ctr = &ctrls[i][j];
                    bid = it->ID;
                }
            }
        }
    }
    //LOGERR("want to the berth %d\n",bid);
    if (bid == berth_ID)
    {
        return;
    }
    for (auto it = ctrl.rbts.begin(); it != ctrl.rbts.end();)
    {
        //LOGERR("(*it)->bth_id=%d, compare to%d\n",(*it)->bth_id,berth_ID);
        if ((*it)->bth_id == berth_ID)
        {
            //LOGERR("robot%d change berth goal from %d to %d\n", (*it)->id, (*it)->bth_id, bid);
            (*it)->bth_id = bid;
            (*it)->ctrl_id = ctr->id;
            ctr->rbts.push_back((*it));
            it = ctrl.rbts.erase(it);
        }
        else
        {
            ++it;
        }
        //LOGERR("1\n");
    }
    //LOGERR("conu\n");
    LOGERR("now the berth %d are quit to berth %d at %d\n",berth_ID,bid,Time);
}