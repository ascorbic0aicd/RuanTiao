#include "Cell.h"
#include "marcos.h"
#include<algorithm>
using namespace std;
vector<vector<Cell>> maps(230, vector<Cell>(230));
void Cell::init(int x, int y, CellType type)
{
    loc = Location(x, y);
    this->type = type;
}
int Cell::nearest_Berth(list<int> &black_list)
{
    int id = -1;
    int dis = 114514;
    for (int i = 0; i < BERTH_NUM; i++)
    {
        if (length_to_berth[i] == -1)
        {
            length_to_berth[i] = loc.disTO(Berths[i].getLoc());
        }
        assert(length_to_berth[i] != -1);
        LOG("dis(%d) >length_to_berth[%d](%d)?\n ", dis, i, length_to_berth[i]);
        LOG("berth[%d].havebot = %d\n",i,Berths[i].haveBoat());
        if (dis > length_to_berth[i]&&Berths[i].haveBoat()&& find(black_list.begin(),black_list.end(),i)==black_list.end())
        {

            id = i;
            dis = length_to_berth[i];
        }
    }
    LOG("nearest boat is %d cost(%d)\n",id,dis);
    assert(id != -1);
    return id;
}