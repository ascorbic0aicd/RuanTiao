#include "Good.h"
#include "Controler.h"
#include "Frame.h"
#include "marcos.h"
void  addGood(int val,int life,int x,int y)
{
    Good *gd = new Good(x, y ,val,life);
    ctrls[x/DIV+1][y/DIV + 1].addGood(gd);
    frames[(life + DISAPPER_TIME) % MAGIC_NUMBER].addGood(gd);
    LOG("add good at time =%d",life + DISAPPER_TIME);
}