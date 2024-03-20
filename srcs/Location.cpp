#include "Location.h"
#include "Robot.h"
#include "Cell.h"
#include "marcos.h"
#include <queue>
#include <cassert>
#include <map>
#include <list>
extern int Time;
extern char ch[230][230];
#define TOO_MANY 500
bool can_goback = true;
// 10847
struct Waypoint
{
    Location loc;
    int cost;
    int guess;
    static int cnt;
    Waypoint *parent;
    Waypoint(Location loc, int cost, int guess, Waypoint *parent = nullptr) : loc(loc), cost(cost), guess(guess), parent(parent)
    {
        cnt++;
        ch[loc.x][loc.y] = false;
    }
    Waypoint(int x, int y, int cost, int guess, Waypoint *parent = nullptr) : loc(x, y), cost(cost), guess(guess), parent(parent)
    {
        cnt++;
        ch[x][y] = true;
    }
    ~Waypoint()
    {
        cnt--;
        ch[loc.x][loc.y] = true;
    }
    void remake()
    {
        ch[loc.x][loc.y] = true;

        Location neighbours[4] = {
            Location(this->loc.x - 1, this->loc.y),
            Location(this->loc.x + 1, this->loc.y),
            Location(this->loc.x, this->loc.y + 1),
            Location(this->loc.x, this->loc.y - 1),
        };

        for (int i = 0; i < 4; i++)
        {
            ch[neighbours[i].x][neighbours[i].y] = true;
        }
    }
    bool operator<(const Waypoint &other) const
    {
        // LOG("cmp cost(%d) + guess(%d) <cmp cost(%d) + guess(%d) = ",cost,guess,other.cost,other.guess);
        if (cost + guess == other.cost + other.guess)
        {
            return cost > other.cost;
        }

        return cost + guess < other.cost + other.guess;
    }
    bool operator==(const Waypoint &other) const
    {
        return cost + guess == other.cost + other.guess;
    }
    bool operator>(const Waypoint &other) const
    {
        return cost + guess > other.cost + other.guess;
    }
};
struct cmp
{
    bool operator()(Waypoint *&a, Waypoint *&b) const
    {
        return *a > *b;
    }
};
int Waypoint::cnt = 0;
bool Location::operator==(const Location &other) const
{
    return x == other.x && y == other.y;
}
bool Location::operator!=(const Location &other) const
{
    return !((*this) == other);
}
void clearWaypoint(priority_queue<Waypoint *, vector<Waypoint *>, cmp> &q, list<Waypoint *> &close_list)
{
    for (auto it = close_list.begin(); it != close_list.end();)
    {
        delete *it;
        it = close_list.erase(it);
    }
    while (!q.empty())
    {
        Waypoint *del = q.top();
        q.pop();
        delete del;
    }
    if (Waypoint::cnt != 0)
    {
        LOGERR("ERROR!,waypoint::cnt =%d\n", Waypoint::cnt);
        assert(0);
    }
}

bool Location::findPath(const Location &start, Location &target, PATH<PATH_TYPE> &res, int berth_id)
{
    LOG("now ind the way from (%d,%d) to (%d,%d) berth_id = %d\n", start.x, start.y, target.x, target.y, berth_id);

    Waypoint *magic_point = nullptr;
    char t[230][230];
    for (int i = 0; i < 230; ++i)
    {
        for (int j = 0; j < 230; ++j)
        {
            if (ch[i][j] == 1)
            {
                t[i][j] = '.';
            }
            else
            {
                t[i][j] = ch[i][j];
            }
        }
    }

    priority_queue<Waypoint *, vector<Waypoint *>, cmp> q;
    Waypoint *start_waypoint = new Waypoint(start, 0, 0);
    q.push(start_waypoint);
    maps[start.x][start.y].arrive_times.insert({Time, 514});
    list<Waypoint *> close_list;
    bool find_way = false;
    Waypoint *target_waypoint = nullptr;
    int most_cost = 0;
    int cnt_num = 0;
    int total_cost = 0;
    int T_cnt = 0;
    while (!q.empty() && !find_way && most_cost < TOO_MANY)
    {
        Waypoint *temp = q.top();
        int wide_num = 0;

        //LOG("now check (%d,%d) cost = %d\n", temp->loc.x, temp->loc.y, temp->cost);
        assert(temp->loc.x > 0 && temp->loc.x < 230 && temp->loc.y > 0 && temp->loc.y < 230);
        // LOGLOC("top = %d at (%d,%d)\n", temp->cost + temp->guess, temp->loc.x, temp->loc.y);
        q.pop();
        // LOGLOC("remain size  = %d\n", q.size());
        close_list.push_back(temp);
        Location neighbours[4] = {
            Location(temp->loc.x - 1, temp->loc.y),
            Location(temp->loc.x + 1, temp->loc.y),
            Location(temp->loc.x, temp->loc.y + 1),
            Location(temp->loc.x, temp->loc.y - 1),
        };
        int can_back = 114;
        int can_stay = 514;
        for (int i = 0; i < 4; i++)
        {
            LOGLOC("try can_go\n");
            int can_go = canGo(neighbours[i], temp->loc, Time + temp->cost + 1);
            if (can_go == 1)
            {
                wide_num++;
                int cost = temp->cost + 1;
                // This can be optimized.
                int guess = target.disTO(neighbours[i]);
                Waypoint *next = new Waypoint(neighbours[i], cost, guess, temp);
                LOGLOC("add (%d,%d) to open_list from (%d,%d)\n", next->loc.x, next->loc.y, temp->loc.x, temp->loc.y);

                most_cost = max(most_cost, next->cost + next->guess);
                // LOG("add the point(%d,%d) cost =%d,guess = %d, total = %d,from (%d,%d)\n",neighbours[i].x,neighbours[i].y,next->cost,next->guess,next->cost +next->guess, next->parent->loc.x,next->parent->loc.y);
                q.push(next);
                if (neighbours[i] == target || (berth_id != -1 && Berths[berth_id].inArea(neighbours[i])))
                {
                    find_way = true;
                    target = neighbours[i];
                    target_waypoint = next;
                    total_cost = target_waypoint->cost;
                    break;
                }
            }
            else if (can_go == 0 && (can_stay != 1))
            {
                ch[temp->loc.x][temp->loc.y] = true;
                can_stay = canGo(temp->loc, temp->loc, Time + temp->cost + 1);
            }
            else if (can_go == 114 && (can_back != 1) && temp->parent != nullptr && temp->parent->loc != temp->loc)
            {
                ch[temp->parent->loc.x][temp->parent->loc.y] = true;
                LOG("try to back(%d,%d) at Frame %d\n", temp->parent->loc.x, temp->parent->loc.y, temp->cost + 1);
                can_back = canGo(temp->parent->loc, temp->loc, Time + temp->cost + 1);
                LOG("can back = %d\n", can_back);
            }
            // DEBUG
        }
        if (can_goback)
        {
            if (can_stay == 1)
            {
                Waypoint *remake = new Waypoint(temp->loc, temp->cost + 1, temp->guess, temp);
                LOG("stay = (%d,%d) in cost %d\n", remake->loc.x, remake->loc.y, remake->cost);
                q.push(remake);
            }
            else
            {
                ch[temp->loc.x][temp->loc.y] = false;
            }
            if (can_back == 1)
            {
                temp->parent->remake();
                Waypoint *back = new Waypoint(temp->parent->loc, temp->cost + 1, temp->parent->guess, temp);
                LOG("back = (%d,%d) from (%d,%d) at frame %d\n", back->loc.x, back->loc.y, temp->loc.x, temp->loc.y, back->cost + Time);
                assert(back->loc != temp->loc);
                back->remake();
                q.push(back);
            }
            else
            {
                if (temp->parent != nullptr)
                {
                    ch[temp->parent->loc.x][temp->parent->loc.y] = false;
                }
            }
        }
        if (temp->cost > 10 && temp->cost < 15 && (wide_num >= 2 || magic_point == nullptr))
        {
            if (magic_point == nullptr)
            {
                magic_point = temp;
            }
            else
            {
                magic_point = random() % 3 == 1 ? temp : magic_point;
            }
        }

        cnt_num++;
    }
    if (find_way)
    {
        res.clear();
        LOGLOC("%d\n", target_waypoint->cost);

        // DEBUG

        Waypoint *temp = target_waypoint;
        int cnt = 0;
        while (temp != start_waypoint && cnt < target_waypoint->cost)
        {
            assert(temp != nullptr);
            assert(maps[temp->loc.x][temp->loc.y].getType() != BARRIER && maps[temp->loc.x][temp->loc.y].getType() != SEA);
            res.push_front(temp->loc);
            maps[temp->loc.x][temp->loc.y].arrive_times.insert({Time + temp->cost, 114});
            if (berth_id != -1)
            {
                maps[temp->loc.x][temp->loc.y].length_to_berth[berth_id] = total_cost - temp->cost;
            }
            temp = temp->parent;
            cnt++;
        }
        //LOG("fact cost = %d\n", cnt);
        
    }
    else
    {
        res.clear();
        assert(magic_point!=nullptr);
        Waypoint *temp = magic_point;
        LOG("random point = (%d,%d) cost = %d\n",magic_point->loc.x,magic_point->loc.y,magic_point->cost);
        int cnt = 0;
        while (temp != start_waypoint && cnt < magic_point->cost)
        {
            assert(temp != nullptr);
            assert(maps[temp->loc.x][temp->loc.y].getType() != BARRIER && maps[temp->loc.x][temp->loc.y].getType() != SEA);
            res.push_front(temp->loc);
            maps[temp->loc.x][temp->loc.y].arrive_times.insert({Time + temp->cost, 114});
            temp = temp->parent;
            cnt++;
        }
        assert(temp==start_waypoint);

        LOG("start point = (%d,%d) cost = %d\n",temp->loc.x,temp->loc.y,temp->cost);
        for(auto i:res)
        {
            LOG("(%d,%d)\n",i.x,i.y);
        }
        if (cnt != temp->cost)
        {
            LOGERR("cnt = %d but cost=%d\n", cnt, temp->cost);
            assert(cnt != temp->cost);
        }
        target = magic_point->loc;
        LOG("We can't find the way from (%d,%d) to (%d,%d):( cnt %d at frame %d\n",start_waypoint->loc.x, start_waypoint->loc.y, target.x, target.y, most_cost, Time);
    }
    clearWaypoint(q, close_list);
    LOG("Finish find the way from (%d,%d) to (%d,%d)\n", start.x, start.y, target.x, target.y);
    return find_way;
}

int canGo(const Location &target, const Location &from, int arrive_time)
{
    if (maps[target.x][target.y].getType() != BARRIER &&
        maps[target.x][target.y].getType() != SEA &&
        ch[target.x][target.y])
    {
        if (maps[target.x][target.y].arrive_times.find(arrive_time) !=
            maps[target.x][target.y].arrive_times.end())
        {
            LOG("can't move to (%d,%d) at frame %d->0<-\n", target.x, target.y, arrive_time);
            return 0;
        }
        if (maps[target.x][target.y].arrive_times.find(arrive_time - 1) !=
                maps[target.x][target.y].arrive_times.end() &&
            maps[from.x][from.y].arrive_times.find(arrive_time) !=
                maps[from.x][from.y].arrive_times.end())
        {
            LOG("can't move to (%d,%d) from (%d,%d) at frame %d because -><-\n", target.x, target.y, from.x, from.y, arrive_time);
            return 114;
        }
        return 1;
    }
    else
    {
        return -1;
    }
}
