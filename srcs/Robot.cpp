#include "Robot.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <functional>
#include <assert.h>
#include "marcos.h"
extern const int N;
extern const int n;
extern vector<vector<Cell>> maps;
int rbt_idx = 0;

int heuristic(Point a, Point b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

Robot *robots[ROBOT_NUM + 5];

void Robot::checkStatus(int x, int y, bool have_good, bool can_move)
{
    // TODO
    if (status == BROKEN)
    {
        if (x != this->x || y != this->y)
        {
            LOG("error loc:id=%d, this->x=%d,x=%d,this->y=%d,y=%d, step=%d, status=%d, can_move=%d\n", id, this->x, x, this->y, y, step, status, can_move);
        }
        if (can_move == 1)
        {
            status = store_status;
            store_status=BROKEN;
        }
    }
    else
    {
        if (can_move == 0)
        {
            if (status = HAVE_GOOD)
            {
                store_status = HAVE_GOOD;
            }
            else
            {
                store_status = FREE;
            }
            status = BROKEN;
            path.clear();
        }
        else if (x - this->x == 1 && step != DOWN || x - this->x == -1 && step != UP || y - this->y == 1 && step != RIGHT || y - this->y == -1 && step != LEFT)
        {
            LOG("error loc:id=%d, this->x=%d,x=%d,this->y=%d,y=%d, step=%d, status=%d, can_move=%d\n", id, this->x, x, this->y, y, step, status, can_move);
        }
    }

    if (have_good == 1 && !(status == HAVE_GOOD || status == BROKEN && store_status == HAVE_GOOD))
    {
        LOG("dont carry good,have_good=%d,status=%d, path=%d, can_move=%d\n", have_good, status, path.empty(), can_move);
    }

    // assert(have_good == 1 && status == HAVE_GOOD);
    // assert(can_move == 0 && status == BROKEN);
}
void Robot::changeStatus(ROBOT_STATUS st)
{
    status = st;
}

void Robot::changeStatus(ROBOT_STATUS st,int id)
{
    status = st;
    Berth_ID=id;
}
void Robot::changestoStatus(ROBOT_STATUS st)
{
    store_status=st;
}


struct PIP
{
    Point loc;
    int cost;
    PIP()
    {
        loc = Point(0, 0);
        cost = 0;
    }
    PIP(Point l, int c)
    {
        cost = c;
        loc = l;
    }
    PIP(const PIP &other)
    {
        loc = other.loc;
        cost = other.cost;
    }
    bool operator==(const PIP &other) const
    {
        return loc == other.loc && cost == other.cost;
    }
    PIP &operator=(const PIP &other)
    {
        if (this != &other)
        {
            loc = other.loc;
            cost = other.cost;
        }
        return *this;
    }
};

struct Compare
{
    bool operator()(const PIP &a, const PIP &b)
    {
        return a.cost > b.cost;
    }
};

struct PointHash1
{
    size_t operator()(const Point &p) const
    {
        return hash<int>()(p.x) ^ hash<int>()(p.y);
    }
};

struct PointHash2
{
    size_t operator()(const Point &p) const
    {
        return hash<int>()(p.x) ^ hash<int>()(p.y);
    }
};

list<Point> Robot::AStarPath(Point start, Point goal)
{
    // LOG("robot astarpath qid=%d\n", id);
    priority_queue<PIP, vector<PIP>, Compare> frontier;
    frontier.push(PIP(start, 0));

    unordered_map<Point, Point, PointHash1> came_from;
    unordered_map<Point, int, PointHash2> cost_so_far;
    came_from[start] = {-1, -1};
    cost_so_far[start] = 0;
    while (!frontier.empty())
    {
        Point current = frontier.top().loc;
        frontier.pop();
        // LOG("88\n");
        if (current == goal)
            break;

        vector<Point> neighbor;
        if (current.x - 1 >= 1 && maps[current.x - 1][current.y].Type() != BARRIER && maps[current.x - 1][current.y].Type() != SEA)
        {
            neighbor.push_back(Point(current.x - 1, current.y));
        }
        if ((current.x + 1) <= 200 && maps[current.x + 1][current.y].Type() != BARRIER && maps[current.x + 1][current.y].Type() != SEA)
        {
            neighbor.push_back(Point(current.x + 1, current.y));
        }
        if (current.y - 1 >= 1 && maps[current.x][current.y - 1].Type() != BARRIER && maps[current.x][current.y - 1].Type() != SEA)
        {
            neighbor.push_back(Point(current.x, current.y - 1));
        }
        if ((current.y + 1) <= 200 && maps[current.x][current.y + 1].Type() != BARRIER && maps[current.x][current.y + 1].Type() != SEA)
        {
            neighbor.push_back(Point(current.x, current.y + 1));
        }

        for (int i = 0; i < neighbor.size(); i++)
        {
            // LOG("94 %d \n", i);
            Point next = neighbor[i];
            int new_cost = cost_so_far[current] + 1;
            // LOG("cost %d %d % d\n", new_cost, maps[current.x][current.y].cost(), cost_so_far[next]);
            if (!cost_so_far.count(next) || new_cost < cost_so_far[next])
            {
                cost_so_far[next] = new_cost;
                int priority = new_cost + heuristic(next, goal);
                frontier.push(PIP(next, priority));
                came_from[next] = current;
            }
            // LOG("118\n");
        }
    }
    list<Point> path;
    if (!came_from.count(goal))
    {
        return path;
    }

    Point current = goal;
    LOG("Path goal(%d,%d)\n", goal.x, goal.y);
    LOG("Path start(%d,%d)\n", start.x, start.y);
    while (current != start)
    {
        path.push_front(current);
        // LOG("Point(%d,%d)\n", current.x, current.y);
        current = came_from[current];
    }
    // path.push_front(start);
    //  LOG("Path start(%d,%d)\n", current.x, current.y);
    return path;
}

void Robot::getpull()
{
    // LOG("robot act id=%d\n", id);
    if (status == BROKEN || path.empty())
    {
        return;
    }
    Point now = path.front();
    this->x = now.x;
    this->y = now.y;

    if (status==HAVE_GOOD && maps[x][y].toBerth[Berth_ID].empty())
    {
        maps[x][y].toBerth[Berth_ID] = path;
    }

    if (now.x != x || now.y != y)
    {
        LOG("error path:now.x=%d,x=%d,now.y=%d,y=%d\n", now.x, x, now.y, y);
    }
    path.pop_front();
    if (path.empty())
    {
        if (status == MOVING)
        {
            printf("get %d\n", id);
            changeStatus(HAVE_GOOD);
        }
        else if (status == HAVE_GOOD)
        {
            printf("pull %d\n", id);
            changeStatus(FREE);
        }
    }
    
}
void Robot::move()
{
    // LOG("robot act id=%d\n", id);
    if (status == BROKEN || path.empty())
    {
        return;
    }

    Point next = path.front();
    int dir = 0;
    if (next.y == y)
    {
        if (next.x - x == 1)
        {
            dir = DOWN;
        }
        else
        {
            dir = UP;
        }
    }
    else
    {
        if (next.y - y == 1)
        {
            dir = RIGHT;
        }
        else
        {
            dir = LEFT;
        }
    }
    step = (direction)dir;
    printf("move %d %d\n", id, dir);
    // if (path.size() == 1)
    // {
    //     if (status == MOVING)
    //     {
    //         printf("get %d\n", id);
    //         changeStatus(HAVE_GOOD);
    //     }
    //     else if (status == HAVE_GOOD)
    //     {
    //         printf("pull %d\n", id);
    //         changeStatus(FREE);
    //     }
    //     path.clear();
    // }
}