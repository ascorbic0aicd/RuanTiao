#include "Location.h"
#include "Robot.h"
#include "Cell.h"
#include "marcos.h"
#include <queue>
#include <cassert>
#include <map>
#include <list>

extern char ch[230][230];
#define TOO_MANY 114514
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
    bool operator<(const Waypoint &other) const
    {
        // LOG("cmp cost(%d) + guess(%d) <cmp cost(%d) + guess(%d) = ",cost,guess,other.cost,other.guess);
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
bool Location::findPath(const Location &start, const Location &target, PATH<PATH_TYPE> &res)
{
    LOGERR("now ind the way from (%d,%d) to (%d,%d)\n", start.x, start.y, target.x, target.y);
    priority_queue<Waypoint *, vector<Waypoint *>, cmp> q;
    Waypoint *start_waypoint = new Waypoint(start, 0, 0);
    q.push(start_waypoint);
    list<Waypoint *> close_list;
    bool find_way = false;
    Waypoint *target_waypoint = nullptr;
    int most_cost = 0;
    int cnt_num = 0;

    while (!q.empty() && !find_way)
    {
        Waypoint *temp = q.top();
        // LOG("top = %d at (%d,%d)\n",temp->cost+temp->guess,temp->loc.x,temp->loc.y);
        q.pop();
        if (temp->cost >= TOO_MANY)
        {
            most_cost = temp->cost;
            break;
        }
        close_list.push_back(temp);
        Location neighbours[4] = {
            Location(temp->loc.x - 1, temp->loc.y),
            Location(temp->loc.x + 1, temp->loc.y),
            Location(temp->loc.x, temp->loc.y + 1),
            Location(temp->loc.x, temp->loc.y - 1),
        };

        for (int i = 0; i < 4; i++)
        {

            if (maps[neighbours[i].x][neighbours[i].y].getType() != BARRIER &&
                maps[neighbours[i].x][neighbours[i].y].getType() != SEA &&
                ch[neighbours[i].x][neighbours[i].y])
            {
                int cost = temp->cost + 1;
                // This can be optimized.
                int guess = target.disTO(neighbours[i]);
                Waypoint *next = new Waypoint(neighbours[i], cost, guess, temp);
                //LOG("add the point(%d,%d) cost =%d,guess = %d, total = %d,from (%d,%d)\n",neighbours[i].x,neighbours[i].y,next->cost,next->guess,next->cost +next->guess, next->parent->loc.x,next->parent->loc.y);
                q.push(next);
                if (neighbours[i] == target)
                {
                    find_way = true;
                    target_waypoint = next;
                    break;
                }
            }
        }
        cnt_num++;
    }
    if (find_way)
    {
        LOG("FIND THE WAY!cost =%d\n", target_waypoint->cost);
        Waypoint *temp = target_waypoint;
        int cnt = 0;
        while (temp != start_waypoint && cnt < target_waypoint->cost)
        {
            assert(temp!=nullptr);
            assert(maps[temp->loc.x][temp->loc.y].getType()!=BARRIER &&maps[temp->loc.x][temp->loc.y].getType()!=SEA);
            res.push_front(temp->loc);
            temp = temp->parent;

            cnt++;
        }
        for(auto i:res)
        {
            LOG("(%d,%d)\n",i.x,i.y);
        }
        if (cnt != target_waypoint->cost)
        {
            LOGERR("cnt = %d but cost=%d\n", cnt, target_waypoint->cost);
            assert(cnt != target_waypoint->cost);
        }
    }
    else
    {
        LOGERR("We can't find the way:( cnt %d\n", most_cost);
        LOGERR("%c",maps[target.x][target.y].getType());
    }
    clearWaypoint(q, close_list);

    LOG("Finish find the way from (%d,%d) to (%d,%d)\n", start.x, start.y, target.x, target.y);
    return find_way;
}