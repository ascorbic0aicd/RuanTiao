#include "Frame.h"
#include "Good.h"
#include"Controler.h"
#include"marcos.h"
#include<cassert>
#include <algorithm>
 vector<Frame> frames(MAGIC_NUMBER);
void Frame::addGood(Good *good)
{
    gds.push_back(good);
}
void Frame::work()
{
    // remove the good
    for (auto it = gds.begin(); it != gds.end();)
    {
        
        bool success_ = ctrls[(*it)->getX()/DIV+1][(*it)->getY()/DIV+1].removeGood((*it));
        if (!success_)
        {
            LOGERR("[ERR]! can'remove the good in (%d,%d) !\n",(*it)->getX(),(*it)->getY());
            assert(0);
        }
        Good *g = *it;
        it = gds.erase(it);
        delete g;
        
    }

    //TODO
    //Do more operations
}

bool Frame::remove(Good* good)
{
    auto it = find(gds.begin(), gds.end(), good);
    if (it == gds.end())
    {
        LOG("can't find the good in CTRL!");
        return false;
    }
    gds.erase(it);
    return true;
}