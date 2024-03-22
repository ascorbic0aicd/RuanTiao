#pragma once
#define BERTH_NUM 10
#include <vector>
#include "Location.h"
using namespace std;
class Boat;
class Berth
{
    friend Boat;
    friend void adjust();
    friend void redistribution();
private:
    Location loc;
    int transport_time;
    int loading_speed;
    int capacity;
    int have_boat;
    int ID;
    int priority;
    int evalWeight() const;

public:
    Berth() : loc(-1, -1) {}
    inline bool haveBoat() const { return have_boat; }
    inline Location getLoc() const { return loc; }
    inline int getID() const { return ID; }
    inline int getTransport_time() const { return transport_time; }
    inline bool inArea(const Location &other)
    {
        //LOG("bth[%d] loc = (%d,%d) other = (%d,%d)\n",ID,loc.x,loc.y,other.x,other.y);
        return other.x >= loc.x && other.y >= loc.y && (other.x - loc.x <= 3) && (other.y - loc.y <= 3);
    }
    void init(int x, int y, int transport_time, int loading_speed, int ID);
    bool operator<(const Berth &) const;
    int transportGood();
    void pullGood();
    void adjust(int p){priority = p;}
};

extern vector<Berth> Berths;
extern vector<int> choose_bid;
void adjust();