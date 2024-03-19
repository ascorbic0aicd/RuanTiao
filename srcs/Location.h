#pragma once
#include <cmath>
#include <cassert>
#include "marcos.h"

using namespace std;
struct Location
{
    int x, y;
    Location(int x, int y) : x(x), y(y) {}
    int inline disTO(const Location &other) const
    {
        int ret = abs(x - other.x) + abs(y - other.y);
        // LOG("abs(%d -%d )(x =%d) + abs(%d - %d)(y=%d) = %d\n",x,other.x,abs(x-other.x),y,other.y,abs(y-other.y),ret);
        return ret;
    }
    int inline disTo(int ox, int oy) const
    {
        return abs(x - ox) + abs(y - oy);
    }
    direction inline directonTo(Location &other) const
    {
        //char s[][1000] = {"RIGHT", "LEFT",  "UP","DOWN"};

        direction ret;
        if (other==*this)
        {
            return STAY;
        }
        
        LOG("(%d,%d) to (%d,%d)\n",x,y,other.x,other.y);
        if (other.x == x)
        {
            //LOGERR("x==other.x?\n",x,other.x);
            if (!(abs(other.y - y) == 1))
            {
                LOG("FROM (%d,%d) to (%d,%d)?\n", x, y, other.x, other.y);
            }

            assert(abs(other.y - y) == 1);
            ret = y > other.y ? LEFT : RIGHT;
        }
        else
        {
            //LOGERR("y(%d)==other.y(%d)?\n",y,other.y);
            if (!(y == other.y && (abs(other.x - x) == 1)))
            {
                LOGERR("FROM (%d,%d) to (%d,%d)\n", x, y, other.x, other.y);
            }

            assert(y == other.y && (abs(other.x - x) == 1));
            ret = x > other.x ? UP : DOWN;
        }
        //LOG("(%d,%d) to (%d,%d) need ret(%d) %s\n",x,y,other.x,other.y,ret,s[ret]);
        return ret;
    }
    Location inline getLeft(){return Location(x,y-1);}
    Location inline getRight(){return Location(x,y+1);}
    Location inline getUp(){return Location(x-1,y);}
    Location inline getDown(){return Location(x+1,y);}
    bool findPath(const Location &start, const Location &target, PATH<PATH_TYPE> &res,bool to_berth);
    bool operator==(const Location &other) const;
    bool operator!=(const Location &other) const;
};
