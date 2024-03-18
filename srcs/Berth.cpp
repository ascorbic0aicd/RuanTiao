#include "Berth.h"
#include "algorithm"
#include <cassert>
using namespace std;
vector<Berth>Berths(BERTH_NUM);
// extern int boat_capacity;
void Berth::init(int x, int y, int transport_time, int loading_speed, int ID)
{
    LOG("init berth at(%d,%d)\n",x,y);
    loc = Location(x,y);
    this->transport_time = transport_time;
    this->loading_speed = loading_speed;
    this->capacity = 0;
    have_boat = 0;
    this->ID = ID;
}
int Berth::evalWeight() const
{
    // TODO

    return -(transport_time - 100 * loading_speed - have_boat * 1000);
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
