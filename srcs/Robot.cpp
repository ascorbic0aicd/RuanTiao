#include "Robot.h"
#include <queue>
#include <map>
#include <cassert>
#include "Berth.h"
#include "Cell.h"
extern int Time;
vector<Robot *> robots(ROBOT_NUM);

void Robot::checkStatus(int x, int y, bool have_good, bool can_move)
{
    // TODO
    // LOGERR("in checkstatus!\n");
    if (!can_move)
    {
        // LOG("%d can't move in %d?\n",id,Time);
        status = BROKEN;
        return;
    }
    else
    {
        if ((status == MOVING || status == HAVE_GOOD) && !paths.empty())
        {
            loc = Location(x, y);
            if (paths.front() != loc)
            {
                LOGERR("loc is (%d,%d) but it need be (%d,%d)\n", loc.x, loc.y, paths.front().x, paths.front().y);
                assert(paths.front() == loc);
            }
            if (loc == target)
            {
                paths.clear();
                if (status == MOVING)
                {
                    Berth &bth = Berths[0];
                    int dis = 114514;
                    for (auto i : Berths)
                    {
                        int temp = i.getLoc().disTO(loc);
                        if (temp < dis)
                        {
                            dis = temp;
                            bth = i;
                        }
                    }
                    assert(status!=HAVE_GOOD);
                    LOG("find the path to berth\n");
                    bool f = loc.findPath(loc, bth.getLoc(), paths);
                    if (!f)
                    {
                        LOGERR("id=%d. ",id);
                    }
                    
                    assert(f);
                    target = bth.getLoc();
                    LOG("first step:(%d,%d)",paths.front().x,paths.front().y);
                    status = HAVE_GOOD;
                    //ctrls[loc.x/DIV][loc.y/DIV].removeGood()
                }
            }
            else
            {
                paths.pop_front();
            }
        }
        else if (status == TO_MOVE)
        {
            status = MOVING;
        }
        else
        {
            if (status == BROKEN)
            {
                status = FREE;
                paths.clear();
            }
        }
        // TODO
        char s[][100] = {"BROKEN","HAVE_GOOD","FREE","MOVING","TO_MOVE"};
        if (id==3)
        {
            
            LOG("FRAME %d STATUS = %s, target = (%d,%d)\n",Time,s[status],target.x,target.y);
            LOG("path:\n");
            for(auto i:paths)
            {
                LOG("(%d,%d)\n",i.x,i.y);
            }
        }
        
    }
    
}

bool Robot::to(Location &target)
{

    LOG("find the path to good\n");
    bool succ = loc.findPath(loc, target, paths);
    LOGLOC("bool Robot::to(Location &target) will ret!\n");
    return succ;
}

void Robot::setTarget(Location &target)
{
    status = TO_MOVE;
    this->target = target;
}
void Robot::action()
{
    if (status == BROKEN || paths.empty())
    {
        return;
    }
    
    printf("move %d %d\n", id, loc.directonTo(paths.front()));
    //LOGERR("move %d %d\n", id, loc.directonTo(paths.front()));

    if (loc == target)
    {
        status = FREE;
        target = Location(-1, -1);
    }
}
int rbt_idx = 0;
