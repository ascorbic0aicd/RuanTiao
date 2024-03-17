#include "Good.h"
#include "Controler.h"
#include"marcos.h"
void addGood(int val, int life, int x, int y)
{
    Good *gd = new Good(x,y,val, life);
    LOGLOC("new done\n");
    ctrls[(x-1) / DIV][(y-1) / DIV].addGood(gd);
}