#include <bits/stdc++.h>
#include "Robot.h"
#include "Boat.h"
#include "Cell.h"
#include "marcos.h"
#include "Berth.h"
#include "Frame.h"
using namespace std;

#define N 230
#define n 200
extern int boat_capacity;
int money, Time;
char ch[230][230];
void Init()
{
    // init the map
    srand(time(0));
    // init the ctrl

    for (int i = 0; i < CTRL_NUM; i++)
    {
        for (int j = 0; j < CTRL_NUM; j++)
        {
            ctrls[i][j].init(i, j);
        }
    }
    for (int i = 1; i <= n; i++)
    {
        scanf("%s", ch[i] + 1);
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            maps[i][j].init(i, j, (CellType)ch[i][j]);
            if ((CellType)ch[i][j] == ROBOT)
            {
                robots[rbt_idx] = new Robot(rbt_idx, i, j);
                LOGLOC("robots[%d] init done \n", rbt_idx);
                rbt_idx++;
            }
        }
    }
    for (int i = 0; i < ROBOT_NUM; ++i)
    {
        if (robots[i] == nullptr)
        {
            LOGERR("WHY robots[%d] is nullptr\n", i);
            assert(0);
        }
    }
    assert(rbt_idx == ROBOT_NUM);
    // init the berth
    for (int i = 0; i < BERTH_NUM; i++)
    {
        int id;
        scanf("%d", &id);
        int x, y, transport_time, loading_speed;
        scanf("%d%d%d%d", &x, &y, &transport_time, &loading_speed);

        Berths[id].init(x + 1, y + 1, transport_time, loading_speed, id);
        LOG("transport_time = %d, loading_speed = %d\n", transport_time, loading_speed);
    }
    scanf("%d", &boat_capacity);
    LOG("capacity = %d\n", boat_capacity);

    char okk[100];
    scanf("%s", okk);
    initBoat();
    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    scanf("%d%d", &Time, &money);
    int num;
    scanf("%d", &num);

    for (int i = 1; i <= num; i++)
    {
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        if (val > 150)
        {
            //assert(0);
            LOGLOC("add good(%d,%d)val = %d start!\n", x, y, val);
            addGood(val, Time, x + 1, y + 1);
            LOGLOC("add good(%d,%d)val = %d end!\n", x, y, val);
        }
    }
    LOGLOC("input the good done\n");
    bool have_good, sts;
    int x, y;

    for (int i = 0; i < ROBOT_NUM; i++)
    {
        scanf("%d%d%d%d", &have_good, &x, &y, &sts);
        // LOG("checkStatus\n");
        robots[i]->checkStatus(x + 1, y + 1, have_good, sts);
    }
    int status, pos;
    for (int i = 0; i < BOAT_NUM; ++i)
    {
        scanf("%d%d", &status, &pos);
        boats[i].check(pos, (BoatState)status);
    }
    char okk[100];
    scanf("%s", okk);
    return Time;
}

int main()
{
    Init();
    for (int zhen = 1; zhen <= 15000; zhen++)
    {
        // list<Location>test;
        LOG("\nFrame =%d\n", zhen);
        //  Location start(66, 127);
        //  Location target(5, 135);
        //  start.findPath(start, target, test);
        //  assert(0);
        Input();
        // printf("move %d %d\n",4,3);
        for (int i = 0; i < ROBOT_NUM; i++)
        {

            // printf("move %d %d\n", i, rand() % 4);
            printf("get %d\n", i);
            printf("pull %d\n", i);
        }
        for (int i = 0; i < BOAT_NUM; i++)
        {
            boats[i].action();
        }
        LOGLOC("robots action start\n");
        for (auto rbt : robots)
        {
            bool arrive = rbt->action();
            if (arrive)
            {
                if (rbt->getStatus() == HAVE_GOOD)
                {
                    LOGLOC("rbt->getStatus() == HAVE_GOOD");
                    Location target(-1, -1);
                    Location ctrl_id = findCTRL(rbt->getLocation());
                    Controler &old_ctrl = ctrls[ctrl_id.x][ctrl_id.y];
                    bool succ = old_ctrl.findTraget(rbt, target);
                    if (!succ)
                    {
                        Controler *neig[3] = {old_ctrl.left,old_ctrl.up,old_ctrl.left->up};
                        for (int i = 0; i < 3; i++)
                        {
                            succ = neig[i]->findTraget(rbt,target);
                            if(succ)
                            {
                                break;
                            }
                        }
                        
                    }
                    assert(succ);
                    LOG("start rm (%d,%d)\n",target.x,target.y);
                    Location good_owner= findCTRL(target);
                    succ = ctrls[good_owner.x][good_owner.y].removeGood(target);
                    LOG("end rm (%d,%d)\n",target.x,target.y);
                    assert(succ);
                    succ = rbt->to(target, false);

                    assert(succ);
                }
                else
                {
                    assert(rbt->getStatus() == MOVING);
                    int bth_id = maps[rbt->getLocation().x][rbt->getLocation().y].nearest_Berth();
                    bool succ = rbt->to(Berths[bth_id].getLoc(), true);
                    assert(succ);
                }

                arrive = rbt->action();
                assert(!arrive);
            }
        }

        LOGLOC("robots action end\n");

        frames[Time % MAGIC_NUMBER].work();
        puts("OK");
        LOG("frame %d\n", Time);
        checkCTRL();
        fflush(stdout);
    }

    return 0;
}
