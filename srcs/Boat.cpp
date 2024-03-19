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
    char s[][100] = {"TRANSMIT","IDIE","WAIT","READY"};
    LOG("boat[%d] status is %s at pos %d at frame %d\n",ID, s[st],_pos,Time);
    //assert(st!=WAIT);
    if (!(status==READY||status==st||Time==1||Time==arrive_time))
    {
        LOGERR("status = %d\n",st);
        
        LOGERR("boat[%d] status need be %s but fact is %s at frame %d\n",ID, s[status],s[st],Time);
    }
    
    assert(status== READY||status == st||Time==1||Time==arrive_time);

}
void assignBoat()
{
    // This is just  a stupid strategy :(
    vector<int> idx = {0,2,4,6,8};
    vector<Berth> fun = {Berths[idx[0]],Berths[idx[1]],Berths[idx[2]],Berths[idx[3]],Berths[idx[4]]};

    for (int i = 0; i < BOAT_NUM; i++)
    {
        boats[i].berth_ID = fun[i].getID();
        boats[i].status = READY;
        boats[i].target = fun[i].getID();
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
void Boat::arrive()
{
    assert(status == TRANSMIT);
    assert(Time == arrive_time);
    LOG("ship[%d] arrive to %d at %d\n", ID,target, Time);
    if (target == -1)
    {
        capacity = 0;
        // TODO
        target = berth_ID;
        status = READY;
        move();
    }
    else
    {
        assert(target < BERTH_NUM && target >= 0);
        status = IDIE;
    }
}

void Boat::move()
{
    assert(status == READY);
    if (target == -1)
    {
        
        printf("go %d\n", ID);
        //assert(0);
        LOG("go %d at frame %d\n", ID, Time);
        arrive_time = Time + Berths[berth_ID].getTransport_time();
        LOG("Time = %d ,transportTime = %d\n",Time,Berths[berth_ID].getTransport_time());
        LOG("%d will arrive at %d\n",ID,arrive_time);
    }
    else
    {
        assert(target < BERTH_NUM && target >= 0);
        printf("ship %d %d\n", ID, target);
        Berths[target].have_boat = true;
        LOG("ship %d %d at frame %d\n", ID, target, Time);
        arrive_time = Time + Berths[target].getTransport_time();
    }
    status = TRANSMIT;
}
void Boat::action()
{
    int num = -1;
    switch (status)
    {
    case WAIT:
        LOGERR("%d IS WAITING ? at frame %d\n", ID, Time);
        assert(0);
        return;
    case READY:
        move();
        return;
    case TRANSMIT:
        if (Time == arrive_time)
        {
            arrive();
        }
        return;
    case IDIE:
        num = Berths[berth_ID].transportGood();
        capacity += num;
        if (capacity + num > boat_capacity || Time + Berths[berth_ID].transport_time > 14999)
        {
            assert(Time!=10452);
            target = -1;
            status = READY;
            move();
        }
        return;

    default:
        assert(0);
        return;
    }
}