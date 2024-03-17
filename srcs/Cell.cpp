#include "Cell.h"
#include "marcos.h"

vector<vector<Cell>> maps(230, vector<Cell>(230));
void  Cell:: init(CellType type)
{
    this->type = type;
}
