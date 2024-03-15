#include <bits/stdc++.h>
#include "Robot.h"
#include "Boat.h"
#include "Cell.h"
#include "marcos.h"
#include "Berth.h"
#include "Controler.h"
using namespace std;

const int n=200;
const int N=230;


extern int boat_capacity;
int money, Time;
char ch[N][N];
vector<vector<Cell>> maps(N, vector<Cell>(N));


int sum = 0;
int cnt = 0;
int myMax = -1;
int min_val = 114514;
int max_val = -1;
int myignore = 0;
void Init()
{
    // init the map
    srand(time(0));
    for (int i = 1; i <= n; i++)
    {
        scanf("%s", ch[i] + 1);
    }
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            maps[i][j].init((CellType)ch[i][j]);
            if ((CellType)ch[i][j] == ROBOT)
            {
                robots[rbt_idx] = new Robot(rbt_idx, i, j);
                rbt_idx++;
            }
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
        Berths[id].init(x, y, transport_time, loading_speed, id);
        LOG("transport_time = %d, loading_speed = %d\n", transport_time, loading_speed);
    }
    scanf("%d", &boat_capacity);
    LOG("capacity = %d\n", boat_capacity);

    char okk[100];
    scanf("%s", okk);
    LOG("OKK=%s", okk);
    initBoat();
    printf("OK\n");
    fflush(stdout);
}

int Input()
{
    scanf("%d%d", &Time, &money);
    int num;
    scanf("%d", &num);
    if (num != 0)
    {
        cnt += num;
        if (num > myMax)
            myMax = num;
    }

    for (int i = 1; i <= num; i++)
    {
        
        int x, y, val;
        scanf("%d%d%d", &x, &y, &val);
        //Good* good=new Good(x,y, val, Time);
        //ctrls[0]->addGood(good);

        min_val = min(min_val, val);
        max_val = max(max_val, val);
        if (val < 180)
        {
            myignore++;
        }
        else
        {
            sum += val;
        }
    }
    bool have_good, sts;
    int x, y;

    for (int i = 0; i < ROBOT_NUM; i++)
    {
        scanf("%d%d%d%d", &have_good, &x, &y, &sts);
        robots[i]->checkStatus(x, y, have_good, sts);
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
        int id = Input();
        ctrls[1][1].Manager();
        // for (int i = 0; i < ROBOT_NUM; i++)
        // {
        //     printf("move %d %d\n", i, rand() % 4);
        //     printf("get %d\n", i);
        //     printf("pull %d\n", i);
        // }
        for (int i = 0; i < BOAT_NUM; i++)
        {
            boats[i].action();
        }

        puts("OK");
        fflush(stdout);
    }
    LOG("max = %d\n", myMax);
    LOG("sum = %d\n", sum);
    LOG("num = %d\n", cnt);
    LOG("max_val = %d\n", max_val);
    LOG("minval = %d\n", min_val);
    LOG("E(val) = %d\n", sum / (cnt - myignore));
    LOG("ignore = %d\n", myignore);
    LOG("rate =%lf\n", (double)myignore / cnt);
    return 0;
}
