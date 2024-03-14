#pragma once
#include <vector>
#include <list>
#include <iostream>
#include "Berth.h"
using namespace std;


typedef enum CellType
{
    PLACE = '.',
    SEA = '*',
    BARRIER = '#',
    ROBOT = 'A',
    PORT = 'B'
} CellType;
#define Inf 99999999

struct Point
{
    int x;
    int y;
    Point():x(0),y(0){}
    Point(int f, int s) : x(f), y(s) {}
    Point(const Point &other)
    {
        x = other.x;
        y = other.y;
    }
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point &other) const
    {
        return x != other.x || y != other.y;
    }
};

class Cell
{
private:
    CellType type;
    list<Point> toBerth[BERTH_NUM];
    // TODO
public:
    Cell():type(PLACE){}
    void init(CellType type);
    int cost() const;
};
