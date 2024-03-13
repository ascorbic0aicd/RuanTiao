#pragma once
#define BOAT_NUM 5
typedef enum BoatState{TRANSMIT=0,IDIE=1,WAIT=2} BoatState;

class Boat
{
    friend void initBoat();
private:
    int pos;
    BoatState status;
    int capacity;
    int arrive_time;
public:  
    Boat():pos(-1),status(TRANSMIT),capacity(-1),arrive_time(114514){};
    void check(int _pos,BoatState st);
};
extern Boat boats[BOAT_NUM + 5];
extern int boat_capacity;
void initBoat();
void assignBoat();