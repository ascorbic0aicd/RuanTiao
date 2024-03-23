#include "Berth.h"
#include "algorithm"
#include "Controler.h"
#include <cassert>
using namespace std;
vector<Berth>Berths(BERTH_NUM);
vector<int> choose_bid;
extern int boat_capacity;
const int c1=1;
const int c2=2;
const int c3=1000;
void Berth::init(int x, int y, int transport_time, int loading_speed, int ID)
{
    LOGERR("init berth %d at(%d,%d)\n",ID,x,y);
    
    loc = Location(x,y);
    this->transport_time = transport_time;
    this->loading_speed = loading_speed;
    this->capacity = 0;
    have_boat = 0;
    this->ID = ID;
    priority=0;
    Location ctrl_id = findCTRL(loc);
    ctrls[ctrl_id.x][ctrl_id.y].addBerth(this);
}
int Berth::evalWeight() const
{
    // TODO
    return (boat_capacity/loading_speed) * c1 + transport_time * 2 * c2 + have_boat * c3;
    //return -(transport_time - 100 * loading_speed - have_boat * 1000);
}
bool Berth::operator<(const Berth &other) const
{
    return evalWeight() < other.evalWeight();
}

int Berth::transportGood()
{
    int ret = std::min(loading_speed, capacity);
    capacity -= ret;
    assert(capacity >= 0);
    return ret;
}

void Berth::pullGood()
{
    capacity++;
}

void adjust()
{
    vector<Berth> priber=Berths;
    sort(priber.begin(),priber.end());
    for(int i=0;i<BERTH_NUM;i++)
    {
        for(int j=0;j<BERTH_NUM;j++)
        {
            if(j==priber[i].ID)
            {
                Berths[j].priority=i;
            }
        }
    }
}
