#include "marcos.h"
#include "Boat.h"
#include "Berth.h"
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cassert>
using namespace std;
int boat_capacity = -1;
extern int Time;
Boat boats[BOAT_NUM + 5];

void Boat::check(int _pos, BoatState st)
{
    // TODO
}
void assignBoat()
{
    // This is just  a stupid strategy :(
    vector<Berth> bths(BERTH_NUM);
    for (int i = 0; i < BERTH_NUM; i++)
    {
        bths[i] = Berths[i];
    }
    sort(bths.begin(), bths.end());
    boats[0].berth_ID = bths[0].getID();
    boats[0].moveTo(boats[0].berth_ID);
    for (int i = 1; i < BOAT_NUM; i++)
    {
        boats[i].berth_ID = bths[i].getID();
        boats[i].moveTo(boats[i].berth_ID);
    }
}

void initBoat()
{
    for (int i = 0; i < BOAT_NUM; i++)
    {
        boats[i].capacity = 0;
        boats[i].ID = i;
    }
    assignBoat();
}
void Boat::moveTo(int ID)
{
    target = ID;
    status = READY;
    arrive_time = Time + (ID == -1 ? 500 : Berths[ID].getTransport_time());
}
void Boat::move()
{
    // TODO

    moveTo(berth_ID);
}
void Boat::action()
{
    int num = 0;
    if (Time == arrive_time)
    {
        assert(status == TRANSMIT);
        pos = target;
        if (target == -1)
        {
            move();
            assert(status == READY);
        }
        else
        {
            status = IDIE;
        }
    }
    switch (status)
    {
    case TRANSMIT:
        break;
    case WAIT:
        // TODO
        LOG("I AM WATING\n");
        break;
    case READY:
        if (target != -1)
        {
            printf("ship %d %d\n", ID, target);
            LOG("ship %d %d\n", ID, target);
        }
        else
        {
            printf("go %d\n",ID);
        }
        status = TRANSMIT;
        break;
    case IDIE:
        num = Berths[ID].transportGood();
        capacity += num;

        if (num + capacity > boat_capacity || Time + Berths[berth_ID].getTransport_time() >= 14000)
        {
            // This is not good :(
            status = TRANSMIT;
            printf("go %d\n",ID);
        }
        break;

    default:
        LOG("THE BOAT[%d] STATE IS WRONG!\n", ID);
        assert(0);
        break;
    }
}