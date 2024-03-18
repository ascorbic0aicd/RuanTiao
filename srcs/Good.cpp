#include "Good.h"
#include "Controler.h"
#include "marcos.h"
#include "Berth.h"
#include "Cell.h"
extern int Time;
void addGood(int val, int life, int x, int y)
{
    LOGLOC("start new\n");
    Good *gd = new Good(x, y, val, life);
    Location loc = Location(x,y);    
    Location ctrl_id = findCTRL(loc);
    LOGLOC("new (%d,%d) in [%d][%d]\n",x,y,ctrl_id.x,ctrl_id.y);  
    ctrls[ctrl_id.x][ctrl_id.y].addGood(gd);
    LOG("Add good(%d,%d)  at  ctrl[%d][%d] in frame %d\n", x, y, ctrl_id.x,ctrl_id.y, Time);
}
int Good::disTo(const Location &target)
{
    int ret = loc.disTO(target);
    int dis = 114514;
    for (auto bth : Berths)
    {
        if (bth.haveBoat())
        {
            int temp = maps[loc.x][loc.y].disToBerth(bth);
            if (temp < dis)
            {
                dis = temp;
            }
        }
    }
    ret += dis;
    return ret;
}