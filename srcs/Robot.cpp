#include "Robot.h"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <functional>
extern const int N;
extern vector<vector<Cell>> maps;
int rbt_idx = 0;

int heuristic(Point goal, Point dest)
{
    return abs(goal.x - dest.x) + abs(goal.y - dest.y);
}

Robot *robots[ROBOT_NUM + 5];

void Robot::checkStatus(int x, int y, bool have_good, bool can_move)
{
    // TODO
}

struct PIP
{
    Point loc;
    int cost;
    PIP() : cost(0) {}
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
    bool operator==(const PIP &other)
    {
        return loc == other.loc && cost == other.cost;
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

        if (current == goal)
            break;
        Point neighbor[4] = {Point(current.x - 1, current.y), Point(current.x + 1, current.y), Point(current.x, current.y - 1), Point(current.x - 1, current.y + 1)};
        for (int i = 0; i < 4; i++)
        {
            Point next = neighbor[i];
            int new_cost = cost_so_far[current] + maps[current.x][current.y].cost();
            if (!cost_so_far.count(next) || new_cost < cost_so_far[next])
            {
                cost_so_far[next] = new_cost;
                int priority = new_cost + heuristic(next, goal);
                frontier.push(PIP(next, priority));
                came_from[next] = current;
            }
        }
    }

    std::list<Point> path;
    Point current = goal;
    while (current != start)
    {
        path.push_front(current);
        current = came_from[current];
    }
    path.push_front(start);
    return path;
}

void Robot::action()
{
    Point now = path.front();
    path.pop_front();
    if (path.empty())
    {
        if (status == MOVING)
        {
            printf("get %1", id);
            changeStatus(HAVE_GOOD);
        }
        else if (status == HAVE_GOOD)
        {
            printf("pull %1", id);
            changeStatus(FREE);
        }
    }
    Point next = path.front();
    int dir = 0;
    if (next.x - now.x == 1 && next.y == now.y)
    {
        dir = 0;
    }
    else if (next.x - now.x == -1 && next.y == now.y)
    {
        dir = 1;
    }
    else if (next.x == now.x && next.y - now.y == 1)
    {
        dir = 2;
    }
    else if (next.x == now.x && next.y - now.y == -1)
    {
        dir = 3;
    }
    printf("move %1 %2",id,dir);
    if (path.size() == 1)
    {
        if (status == MOVING)
        {
            printf("get %1", id);
            changeStatus(HAVE_GOOD);
        }
        else if(status==HAVE_GOOD)
        {
            printf("pull %1", id);
            changeStatus(FREE);
        }
        path.clear();
    }
}