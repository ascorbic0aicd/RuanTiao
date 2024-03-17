#pragma once
#define DISAPPER_TIME 1000
class Controler;
#include"Location.h"
#include "Frame.h"
class Good
{
    friend void addGood(int val, int life, int x, int y);
    friend Controler;
private:
    int val;
    int life;
    Location loc;
    // TODO
public:
    Good(int x,int y,int val, int life) : val(val), life(life + DISAPPER_TIME),loc(x,y)
    {
        frames[life%MAGIC_NUMBER].addGood(this);
    };
    inline Location getLocation(){return loc;}
    inline int getX(){return loc.x;}
    inline int getY(){return loc.y;}
    // TODO
};
void addGood(int val, int life, int x, int y);