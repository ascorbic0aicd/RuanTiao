#include "Frame.h"
#include "Good.h"
#include "Controler.h"
#include "marcos.h"
#include <cassert>

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
        Location loc = (*it)->getLocation();
        Location ctrl_id = findCTRL(loc);
        bool success_ = ctrls[ctrl_id.x][ctrl_id.y].removeGood((*it));
        if (!success_)
        {
            LOG("[ERR]! can'remove the good in (%d,%d) !\n", (*it)->getX(), (*it)->getY());
            assert(0);
        }
        Good *g = *it;
        it = gds.erase(it);
        delete g;
    }

    // TODO
    // Do more operations
}
bool Frame::removeGood(Location &loc)
{
    for (auto gd = gds.begin(); gd != gds.end(); gd++)
    {
        if ((*gd)->getLocation() == loc)
        {
            gds.erase(gd);
            return true;
        }
    }
    return false;
}