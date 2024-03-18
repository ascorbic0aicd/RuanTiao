#include "Robot.h"
#include <queue>
#include <map>
#include <cassert>
#include "Berth.h"
#include "Cell.h"
#include "Controler.h"
extern int Time;
vector<Robot *> robots(ROBOT_NUM);
// vector<bool> finish_check(ROBOT_NUM, false);
void Robot::checkStatus(int x, int y, bool have_good, bool can_move)
{
    if (loc != Location(x, y))
    {
        LOGERR("rbt[%d] need at (%d,%d) but in fact is at (%d,%d) at frame %d\n", id, loc.x, loc.y, x, y, Time);
        assert(loc == Location(x, y));
    }
    if (!can_move)
    {
        LOGERR("why rbt[%d] is broken in frame %d?\n", id, Time);
    }
}

bool Robot::to(const Location &target, bool to_berth)
{
    assert(paths.empty());
    LOGLOC("Begin find the path from (%d,%d) to (%d,%d)\n",loc.x,loc.y,target.x,target.y);
    bool succ = loc.findPath(loc, target, paths);
    assert(succ);
    status = to_berth ? HAVE_GOOD : MOVING;
    if (to_berth)
    {
        for (auto bth : Berths)
        {
            if (bth.getLoc() == target)
            {
                target_id = bth.getID();
                assert(id >= 0 && id < BERTH_NUM);
                break;
            }
        }
    }

    this->target = target;
    return succ;
}
bool Robot::action()
{

    if (status == MOVING || status == HAVE_GOOD)
    {
        LOGLOC("rbt[%d] is moving sts = %d loc = (%d,%d) target =(%d,%d)\n", id, status, loc.x, loc.y, target.x, target.y);
        if (loc == target)
        {
            assert(paths.empty());
            // LOGLOC("x/DIV = %d, y/DIV=%d\n", loc.x / DIV, loc.y / DIV);
            if (status == MOVING)
            {
                printf("get %d\n", id);
                LOGLOC("get %d\n", id);
            }
            else
            {
                assert(status == HAVE_GOOD);
                printf("pull %d\n", id);
                LOGLOC("pull %d\n", id);
                Berths[target_id].pullGood();
            }
            LOGLOC("rbt[%d] act done\n", id);
            return true;
        }
        else
        {
            assert(!paths.empty());
            assert(id >= 0 && id < BERTH_NUM);
            printf("move %d %d\n", id, loc.directonTo(paths.front()));
            Location ctrl_id = findCTRL(loc);
            if (ctrl_id != findCTRL(paths.front()))
            {
                Location new_ctrl = findCTRL(paths.front());
                LOG("move from (%d,%d) to (%d,%d)\n", ctrl_id.x, ctrl_id.y, new_ctrl.x, new_ctrl.y);
                ctrls[ctrl_id.x][ctrl_id.y].removeRobot(this);
                ctrls[new_ctrl.x][new_ctrl.y].addRobot(this);
            }
            // assert(maps[loc.x][loc.y].arrive_times.size() != 0);
            // int sz = maps[loc.x][loc.y].arrive_times.erase(Time);
            // if (sz == 0)
            // {
            //     for (auto i : maps[loc.x][loc.y].arrive_times)
            //     {
            //         LOGERR("{%d,%d}\n", i.first, i.second);
            //     }
            //     LOGERR("rbt[%d] arrive (%d,%d) at %d but can't remove arrive_time :(\n", id, loc.x, loc.y, Time);
            //     assert(sz != 0);
            // }
            LOG("rbt[%d] arrive (%d,%d) from (%d,%d) at frame %d\n", id, paths.front().x, paths.front().y, loc.x, loc.y, Time);
            loc = paths.front();
            LOG("set rbt[%d].loc = (%d,%d)\n", id, loc.x, loc.y);
            paths.pop_front();
        }
    }
    else
    {
        LOG("rbt[%d] is free at Frame %d\n", id, Time);
        assert(status == FREE);
    }
    LOGLOC("rbt[%d] act done\n", id);
    return false;
}
int rbt_idx = 0;

/*void Robot::changeWay(Location &new_target)
{
    assert(new_target == Location(-1, -1) || new_target != next_step);
    if (next_step != Location(-1, -1))
    {
        maps[next_step.x][next_step.y].will_arrive.remove(id);
        paths.push_front(next_step);
        paths.push_front(loc);
    }
    next_step = new_target;
    if (new_target != Location(-1, -1))
    {

        maps[new_target.x][new_target.y].will_arrive.push_back(id);
    }
}
bool check(Robot rbt, Location &next)
{
    if (maps[next.x][next.y].now_id != -1)
    {
        Robot *other = robots[maps[next.x][next.y].now_id];
        if (finish_check[other->id])
        {
            bool succ = other->next_step == Location(-1, -1) || other->next_step == rbt.loc;
            return succ;
        }
        else
        {
            // assert(other->next_step != Location(-1,-1));
            if (other->next_step == rbt.loc)
            {
                Location neighbours[4] = {next.getDown(), next.getLeft(), next.getRight(), next.getUp()};
                for (int i = 0; i < 4; i++)
                {
                    bool succ = check(*other, neighbours[i]);
                    if (succ)
                    {
                        other->changeWay(neighbours[i]);
                        return true;
                    }
                }
                return false;
            }
        }
    }
    else if (maps[next.x][next.y].will_arrive.size() != 1)
    {
        // assert(!(maps[next.x][next.y].will_arrive.empty()));
        int sz = maps[next.x][next.y].will_arrive.size();
        assert(sz != 0);
        for (auto it = maps[next.x][next.y].will_arrive.begin(); it != maps[next.x][next.y].will_arrive.end())
        {
            assert(!finish_check[(*it)]);
                    if ((*it)
                    {

                    }
        }
    }
}
void Robot::checkPath()
{
    assert(finish_check[id] == false);
    bool succ = check(next_step);
    if (!succ)
    {
        Location neighbours[4] = {next_step.getDown(), next_step.getLeft(), next_step.getRight(), next_step.getUp()};

        for (int i = 0; i < 4; i++)
        {
            succ = check(neighbours[i]);
            if (succ)
            {
                changeWay(neighbours[i]);
                return;
            }
        }
        if (!succ)
        {
            paths.push_front(next_step);
            next_step = Location(-1, -1);
        }
    }
}*/
