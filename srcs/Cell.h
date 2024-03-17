#pragma once
#include <vector>
#include <list>
#include <iostream>
using namespace std;
#define BERTH_NUM 10

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
    Point& operator=(const Point &other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
};

class Cell
{
private:
    CellType type;
    
    // TODO
public:
    list<Point> toBerth[BERTH_NUM];
    Cell():type(SEA){}
    void init(CellType type);
    int cost() const;
    CellType Type(){return type;}
};
