#include "Berth.h"
Berth Berths[BERTH_NUM + 5];
// extern int boat_capacity;
void Berth::init(int x, int y, int transport_time, int loading_speed)
{
    this->x = x;
    this->y = y;
    this->transport_time = transport_time;
    this->loading_speed = loading_speed;
    this->capacity = 0;
    have_boat =0;
}
int Berth::evalWeight() const
{
    //TODO
    
    return -(transport_time - 100*loading_speed - have_boat*1000);
}
bool Berth::operator<(const Berth &other) const
{
    return evalWeight() < other.evalWeight();
}