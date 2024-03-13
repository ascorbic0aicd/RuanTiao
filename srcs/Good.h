#pragma once
#define DISAPPER_TIME 1000
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
    Good(int val, int life) : val(val), life(life + DISAPPER_TIME)
    {

    };
    // TODO
};
void addGood(int val, int life, int x, int y);