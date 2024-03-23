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
    if (!can_move)
    {
        // rbt[5] need at (104,48) but in fact is at (104,49) at frame 190
        LOGERR("why rbt[%d] is broken in frame %d?\n", id, Time);
    }
    if (loc != Location(x, y))
    {
        LOGERR("rbt[%d] need at (%d,%d) but in fact is at (%d,%d) at frame %d\n", id, loc.x, loc.y, x, y, Time);
        assert(loc == Location(x, y));
    }
}

bool Robot::to(const Location &tg, bool to_berth)
{
    LOG("*log find the way to %d\n",tg.x,tg.y);
    Location target = tg;
    LOGLOC("rbt[%d] begin find the path from (%d,%d) to (%d,%d) \n", id, loc.x, loc.y, target.x, target.y);
    LOGLOC("to berth is %d\n", to_berth);
    if (to_berth)
    {

        for (auto bth : ctrls[ctrl_id.x][ctrl_id.y].brhs)
        {
            if (bth->getLoc() == target)
            {
                target_id = bth->getID();
                assert(id >= 0 && id < BERTH_NUM);
                break;
            }
        }
    }
    LOGLOC("beigin clear path\n");

    LOGLOC("end clear path\n");

    bool succ = loc.findPath(loc, target, paths, to_berth ? target_id : -1);
    if (succ)
    {
        old_ways.clear();
        status = to_berth ? HAVE_GOOD : MOVING;
        this->target = target;
    }
    return succ;
}
bool Robot::findTarget() // will set_target
{
    // Location ctrl_id = findCTRL(loc);
    // Controler &old_ctrl = ctrls[ctrl_id.x][ctrl_id.y];
    // bool succ = old_ctrl.findTraget(this, target);
    Controler &old_ctrl = ctrls[ctrl_id.x][ctrl_id.y];
    bool succ = old_ctrl.findTraget(this, target);
    if (!succ)
    {
        Controler *neig[3] = {old_ctrl.left, old_ctrl.up, old_ctrl.left->up};
        for (int i = 0; i < 3; i++)
        {
            succ = neig[i]->findTraget(this, target);
            if (succ)
            {
                LOG("succ to find target at(%d,%d)", target.x, target.y);
                break;
            }
        }
    }
    if (succ)
    {
        succ = to(target, false);
        LOG("start rm (%d,%d)\n", target.x, target.y);
        Location good_owner = findCTRL(target);
        bool test = ctrls[good_owner.x][good_owner.y].removeGood(target);
        LOG("end rm (%d,%d)\n", target.x, target.y);
        assert(test);
    }
    else
    {
        comeBack();
    }
    return succ;
}

void Robot::arrive()
{
    if (status == HAVE_GOOD)
    {
        LOGLOC("rbt->getStatus() == HAVE_GOOD\n");
        bool succ = findTarget();
        if (!succ)
        {
            comeBack();
            status = JOGGING;
        }
    }
    else
    {
        // int bth_id = maps[loc.x][loc.y].nearest_Berth(black_list);

        //int bth_id = ctrls[ctrl_id.x][ctrl_id.y].brhs.front()->getID();
        
        bool succ = to(Berths[bth_id].getLoc(), true);

        // DEBUG
        if (id == 2)
        {
            LOG("rbt[2] succ = %d at frame %d\n", succ, Time);
        }

        if (!succ)
        {
            missing_num[bth_id]++;
            if (missing_num[bth_id] == 3)
            {
                black_list.push_back(bth_id);
            }
            cd = 3;
            status = FINDING;
        }
    }
    action();
}
void Robot::move()
{
    if (paths.empty())
    {
        LOGERR("rbt[%d].paths is empty! loc =(%d,%d) target = (%d,%d),status = %d\n", id, loc.x, loc.y, target.x, target.y, status);
    }
    // if (paths.empty())
    // {
    //     status==FREE;
    //     return;
    // }

    assert(!paths.empty());

    assert(id >= 0 && id < BERTH_NUM);
    direction dir = loc.directonTo(paths.front());
    if (dir != STAY)
    {

        printf("move %d %d\n", id, dir);
    }

    // Location ctrl_id = findCTRL(loc);
    // if (ctrl_id != findCTRL(paths.front()))
    // {
    //     Location new_ctrl = findCTRL(paths.front());
    //     LOG("move from (%d,%d) to (%d,%d)\n", ctrl_id.x, ctrl_id.y, new_ctrl.x, new_ctrl.y);
    //     ctrls[ctrl_id.x][ctrl_id.y].removeRobot(this);
    //     ctrls[new_ctrl.x][new_ctrl.y].addRobot(this);
    // }

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
    old_ways.push_front(loc);
    loc = paths.front();
    // LOG("set rbt[%d].loc = (%d,%d)\n", id, loc.x, loc.y);
    paths.pop_front();
}
bool Robot::action()
{
    //LOG("rbt[%d] status = %d,target =(%d,%d)\n", id, status, target.x, target.y);
    char s[][100] = {"BROKEN", "HAVE_GOOD", "FREE", "MOVING", "JOGGING", "FINDING"};
    {
        LOG("*rbt[%d].status = %s at frame %d target =(%d,%d)\n",id, s[status], Time,target.x,target.y);
    }

    if (status == MOVING || status == HAVE_GOOD)
    {
        LOGLOC("rbt[%d] is moving sts = %d loc = (%d,%d) target =(%d,%d)\n", id, status, loc.x, loc.y, target.x, target.y);
        if (loc == target)
        {
            // assert(paths.empty());
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
            arrive();
            return true;
        }
        else
        {
            move();
        }
    }
    else if (status == JOGGING)
    {
        bool succ = findTarget();
        if (succ)
        {
            status = MOVING;
        }
        if (target == loc)
        {
            comeBack();
        }

        move();
    }
    else if (status == FINDING)
    {
        if (cd == 0)
        {
            // int bth_id = maps[loc.x][loc.y].nearest_Berth(black_list);
            //int bth_id = ctrls[ctrl_id.x][ctrl_id.y].brhs.front()->getID();
            bool succ = to(Berths[bth_id].getLoc(), true);
            if (!succ)
            {
                missing_num[bth_id]++;
                if (missing_num[bth_id] == 3)
                {
                    black_list.push_back(bth_id);
                }
                cd = 3;
                status = FINDING;
            }
            else
            {
                LOG("rbt[%d] find berth at frame %d to (%d,%d)", id, Time, target.x, target.y);
                action();
            }
        }
        else
        {
            move();
            cd--;
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
void Robot::comeBack()
{
    Location tg = old_ways.back();
    bool succ = to(tg, false);

    // assert(succ);
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
