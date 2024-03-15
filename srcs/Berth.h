#pragma once
#include "Cell.h"


class Berth
{
private:
    int x;
    int y;
    int transport_time;
    int loading_speed;
    int capacity;
    int have_boat;
    int ID;
    int evalWeight() const;
public:
    Berth() {}
    inline int getID()const{return ID;}
    inline int getTransport_time()const {return transport_time;}
    void init(int x, int y, int transport_time, int loading_speed,int ID);
    bool operator<(const Berth &) const;
    int transportGood();
    inline int getX(){return x;}
    inline int getY(){return y;}
    Point loc(){return Point(x,y);}
};

extern Berth Berths[BERTH_NUM + 5];