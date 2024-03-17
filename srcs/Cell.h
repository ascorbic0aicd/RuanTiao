#pragma once
#include "Robot.h"
typedef enum CellType{PLACE='.',SEA='*',BARRIER='#',ROBOT='A',PORT='B'} CellType;
class Cell
{
private:
    CellType type;
//TODO
public:
    Cell():type(BARRIER){};
    inline CellType getType(){return type;}
    void init(CellType type);
};
extern vector<vector<Cell>> maps;
