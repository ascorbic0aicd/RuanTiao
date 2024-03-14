#pragma once
#define BOAT_NUM 5
typedef enum BoatState{TRANSMIT=0,IDIE=1,WAIT=2,READY} BoatState;

class Boat
{
    friend void assignBoat();
    friend void initBoat();
private:
    int pos;
    BoatState status;
    int capacity;
    int arrive_time;
    int berth_ID;
    int ID;
    int target;
    void moveTo(int ID);
public:  
    Boat():pos(-1),status(TRANSMIT),capacity(-1),arrive_time(114514),berth_ID(-1),ID(-1),target(-1){};
    void check(int _pos,BoatState st);
    void action();
    void move();
};
extern Boat boats[BOAT_NUM + 5];
extern int boat_capacity;
void initBoat();
void assignBoat();