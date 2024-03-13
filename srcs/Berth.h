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
    int evalWeight() const;
public:
    Berth() {}
    void init(int x, int y, int transport_time, int loading_speed);
    bool operator<(const Berth &) const;
};

extern Berth Berths[BERTH_NUM + 5];