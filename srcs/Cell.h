#pragma once
#include "Robot.h"
typedef enum CellType{PLACE='.',SEA='*',BARRIER='#',ROBOT='A',PORT='B'} CellType;
class Cell
{
private:
    CellType type;
//TODO
public:
    void init(CellType type);
};