#include "Boat.h"
int boat_capacity = -1;
Boat boats[BOAT_NUM + 5];

void Boat:: check(int _pos,BoatState st)
{
    //TODO
}
void assignBoat()
{
    //TODO
}
void initBoat()
{
    for (int i = 0; i < BOAT_NUM; i++)
    {
        boats[i].capacity = boat_capacity;
    }
    
}