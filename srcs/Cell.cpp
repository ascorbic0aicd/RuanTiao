#include "Cell.h"
void  Cell:: init(CellType type)
{
    this->type = type;
}

int Cell::cost() const 
{
    int cost = 1;
    switch (this->type)
    {
    case PLACE: 
        cost = 1;break;
    case BARRIER:
        cost = 2;break;
    case SEA:
        cost = Inf;break;
    default:
        break;
    }

   return cost;
}