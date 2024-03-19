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
#define TOO_MANY 175
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
        ch[x][y] = false;
    }
    ~Waypoint()
    {
        cnt--;
        ch[loc.x][loc.y] = true;
    }
    void remake()
    {
        ch[loc.x][loc.y] = false;

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
    LOG("now ind the way from (%d,%d) to (%d,%d) berth_id = %d\n", start.x, start.y, target.x, target.y,berth_id);

    // char t[230][230];
    // for (int i = 0; i < 230; ++i)
    // {
    //     for (int j = 0; j < 230; ++j)
    //     {
    //         if (ch[i][j] == 1)
    //         {
    //             t[i][j] = '.';
    //         }
    //         else
    //         {
    //             t[i][j] = ch[i][j];
    //         }
    //     }
    // }

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
    // int T_cnt = 0;
    while (!q.empty() && !find_way&&most_cost <TOO_MANY)
    {
        Waypoint *temp = q.top();
        assert(temp->loc.x>0&&temp->loc.x<230 &&temp->loc.y>0&&temp->loc.y<230)
        LOGLOC("top = %d at (%d,%d)\n", temp->cost + temp->guess, temp->loc.x, temp->loc.y);
        q.pop();
        // t[temp->loc.x][temp->loc.y] = T_cnt < 10 ? '0' + T_cnt : 'A' + T_cnt - 10;
        // T_cnt++;
        // T_cnt %= 36;
        // if (Time == 19)
        // {
        //     // assert(start==Location(127,26));
        //     for (int i = 1; i < 200; i++)
        //     {
        //         for (int j = 1; j < 200; j++)
        //         {
        //             LOGLOC("%c", t[i][j]);
        //         }
        //         LOGLOC("\n");
        //     }
        // }

        LOGLOC("remain size  = %d\n", q.size());
        close_list.push_back(temp);
        Location neighbours[4] = {
            Location(temp->loc.x - 1, temp->loc.y),
            Location(temp->loc.x + 1, temp->loc.y),
            Location(temp->loc.x, temp->loc.y + 1),
            Location(temp->loc.x, temp->loc.y - 1),
        };

        for (int i = 0; i < 4; i++)
        {
            bool stay = false;
            bool back = false;

            if (maps[neighbours[i].x][neighbours[i].y].getType() != BARRIER &&
                maps[neighbours[i].x][neighbours[i].y].getType() != SEA &&
                ch[neighbours[i].x][neighbours[i].y])
            {
                if (maps[neighbours[i].x][neighbours[i].y].arrive_times.find(Time + temp->cost + 1) != maps[neighbours[i].x][neighbours[i].y].arrive_times.end())
                {
                    LOG("can't move to (%d,%d) at frame %d->0<-\n", neighbours[i].x, neighbours[i].y, Time);
                    ch[temp->loc.x][temp->loc.y] = true;
                    stay = true;
                    back = true;
                    continue;
                }
                else if (maps[neighbours[i].x][neighbours[i].y].arrive_times.find(Time + temp->cost) != maps[neighbours[i].x][neighbours[i].y].arrive_times.end() &&
                         maps[temp->loc.x][temp->loc.y].arrive_times.find(Time + temp->cost + 1) != maps[temp->loc.x][temp->loc.y].arrive_times.end())
                {
                    LOG("can't move to (%d,%d) at frame %d because -><-\n", neighbours[i].x, neighbours[i].y, Time);
                    back = true;
                    ch[temp->loc.x][temp->loc.y] = true;
                    continue;
                }
                // DEBUG

                int cost = temp->cost + 1;
                // This can be optimized.
                int guess = target.disTO(neighbours[i]);
                Waypoint *next = new Waypoint(neighbours[i], cost, guess, temp);
                LOGLOC("add (%d,%d) to open_list from (%d,%d)\n", next->loc.x, next->loc.y, temp->loc.x, temp->loc.y);

                most_cost = max(most_cost, next->cost + next->guess);
                // LOG("add the point(%d,%d) cost =%d,guess = %d, total = %d,from (%d,%d)\n",neighbours[i].x,neighbours[i].y,next->cost,next->guess,next->cost +next->guess, next->parent->loc.x,next->parent->loc.y);
                q.push(next);
                if (stay == true)
                {
                    temp->remake();
                    Waypoint *remake = new Waypoint(temp->loc, cost, temp->guess, temp);
                    q.push(remake);
                }
                if (back)
                {
                    if (temp->parent->loc != temp->loc)
                    {
                        temp->parent->remake();
                        Waypoint *back = new Waypoint(temp->parent->loc, cost, temp->parent->guess, temp);
                        q.push(back);
                    }
                }

                if (neighbours[i] == target||(berth_id!=-1&& Berths[berth_id].inArea(neighbours[i])) )
                {
                    find_way = true;
                    target = neighbours[i];
                    target_waypoint = next;
                    total_cost = target_waypoint->cost;
                    break;
                }
            }
        }
        cnt_num++;
    }
    if (find_way)
    {
        res.clear();
        LOG("FIND THE WAY!cost =%d\n", target_waypoint->cost);
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

            // for (auto it : maps[temp->loc.x][temp->loc.y].arrive_times)
            // {
            //     LOGLOC("will arrive at Frame %d to (%d,%d)\n", it.first, temp->loc.x, temp->loc.y);
            // }
            temp = temp->parent;
            cnt++;
        }
        // for (auto i : res)
        // {
        //     LOGLOC("(%d,%d)\n", i.x, i.y);
        // }
        if (cnt != target_waypoint->cost)
        {
            LOGERR("cnt = %d but cost=%d\n", cnt, target_waypoint->cost);
            assert(cnt != target_waypoint->cost);
        }
    }
    else
    {
        LOG("We can't find the way from (%d,%d) to (%d,%d):( cnt %d at frame %d\n", start.x, start.y, target.x, target.y, most_cost, Time);
    }
    clearWaypoint(q, close_list);

    LOG("Finish find the way from (%d,%d) to (%d,%d)\n", start.x, start.y, target.x, target.y);
    return find_way;
}