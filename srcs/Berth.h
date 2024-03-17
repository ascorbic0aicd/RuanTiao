#pragma once
#define BERTH_NUM 10
#include<vector>
#include "Location.h"
using namespace std;
class Berth
{
private:
    Location loc;
    int transport_time;
    int loading_speed;
    int capacity;
    int have_boat;
    int ID;
    int evalWeight() const;
public:
    Berth():loc(-1,-1) {}
    inline Location getLoc()const {return loc;}
    inline int getID()const{return ID;}
    inline int getTransport_time()const {return transport_time;}
    void init(int x, int y, int transport_time, int loading_speed,int ID);
    bool operator<(const Berth &) const;
    int transportGood();
};

extern vector<Berth>Berths;