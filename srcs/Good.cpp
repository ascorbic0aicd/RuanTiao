#include "Good.h"
#include "Controler.h"
void  addGood(int val,int life,int x,int y)
{
    Good *gd = new Good(x, y ,val,life);
    ctrls[x/DIV+1][y/DIV + 1].addGood(gd);
}