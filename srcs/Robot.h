#pragma once
typedef enum direction
{
    RIGHT,
    LEFT,
    UP,
    DOWN
} direction;
typedef enum ROBOT_STATUS
{
    BROKEN,
    HAVE_GOOD, // go to berth
    FREE,
    MOVING // go to goods
} ROBOT_STATUS;
extern int rbt_idx;

#include "Controler.h"
#include "cmath"
#include "Cell.h"
#include "marcos.h"
#define ROBOT_NUM 10

class Robot
{
    int id;
    int x, y;
    ROBOT_STATUS status;
    direction step;
    ROBOT_STATUS store_status;
    int Berth_ID;

public:
    list<Point> path;

    Robot() {}

    Robot(int i, int startX, int startY) : id(i), x(startX), y(startY), status(FREE), step(UP),store_status(FREE),Berth_ID(i)
    {
        ctrls[x / DIV + 1][y / DIV + 1].addRobot(this);
        //LOG("addRobot %d %d %d\n",id, x / DIV + 1,y / DIV + 1);
    }
    inline int disTO(int x, int y) { return abs(this->x - x) + abs(this->y - y); }
    inline ROBOT_STATUS getStatus() { return status; };
    inline ROBOT_STATUS StoStatus() { return store_status; };
    void changeStatus(ROBOT_STATUS st);
    void changeStatus(ROBOT_STATUS st,int id);
    void changestoStatus(ROBOT_STATUS st);
    void checkStatus(int x, int y, bool have_good, bool can_move);
    list<Point> AStarPath(Point goal, Point dest);
    Point loc() { return Point(x, y); }
    
    void move();
    int ID(){return id;}
    inline int getX(){return x;}
    inline int getY(){return y;}
    void getpull();
};

extern Robot *robots[ROBOT_NUM + 5];
