#pragma once

#define BERTH_NUM 10

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
};

extern Berth Berths[BERTH_NUM + 5];