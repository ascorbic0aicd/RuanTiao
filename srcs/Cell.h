#pragma once
#include "Robot.h"
#include <list>
#include<unordered_map>
#include<vector>
using namespace std;
typedef enum CellType
{
    PLACE = '.',
    SEA = '*',
    BARRIER = '#',
    ROBOT = 'A',
    PORT = 'B'
} CellType;
class Cell
{
private:
    CellType type;

    // TODO
public:
    bool has_inited = false;
    Location loc;
    unordered_map<int,int> arrive_times;
    vector<int> length_to_berth;
    int now_id;
    inline int disToBerth(Berth &bth)
    {
        int ret = length_to_berth[bth.getID()];
        if (ret==-1)
        {
            ret = loc.disTO(bth.getLoc());
            length_to_berth[bth.getID()] = ret;
        }
        return ret;
        
        
    }
    int nearest_Berth(list<int> &black_list);
    Cell() : loc(-1,-1),type(BARRIER),length_to_berth(BERTH_NUM,-1), now_id(-1){};
    inline CellType getType() { return type; }
    void init(int x,int y,CellType type);
};
extern vector<vector<Cell>> maps;
