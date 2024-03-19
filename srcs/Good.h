#pragma once
#define DISAPPER_TIME 1000
#include "Cell.h"

class Controler;
class Good
{
    friend void addGood(int val, int life, int x, int y);
    friend Controler;

private:
    int val;
    int life;
    int x;
    int y;
    // TODO
public:
    Good(int x1, int y1, int val, int _life) : x(x1), y(y1), val(val), life(_life + DISAPPER_TIME){}
    inline int getX() { return x; }
    inline int getY() { return y; }
    // TODO
    Point loc() { return Point(x, y); }
    int Life() { return life; }
};
void addGood(int val, int life, int x, int y);