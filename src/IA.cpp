#include "IA.h"

#include <ctime>
#include <cmath>

IA::IA() : Player()
{
}

IA::IA(Position position, int id, Object *o, glm::vec3 color) : Player(position,id,o,color)
{
    srand(time(NULL));
}

void IA::move()
{
    int **grid=_grid->getGrid();
    Position direction=_directions[_direction];

    int posWallF = hasWall(_pos, direction, 100)+1;

    if(posWallF==1 || (rand()%(posWallF*20)==1))
    {
        int dG = (_direction + 1 + 4) % 4;
        int dD = (_direction - 1 + 4) % 4;

        int posWallG=hasWall(_pos, _directions[dG], 100);
        int posWallD=hasWall(_pos, _directions[dD], 100);

        if(posWallG>posWallD)
            turn(1);
        else
            turn(-1);

        return;
    }

    /*if(rand()%150==1)
    {
        int dG = (_direction + 1 + 4) % 4;
        int dD = (_direction - 1 + 4) % 4;

        int posWallG=hasWall(_pos, _directions[dG], 10);
        int posWallD=hasWall(_pos, _directions[dD], 10);

        if(posWallG > posWallD && posWallG>1)
            turn(1);
        else
            turn(-1);
    }*/
}

int IA::hasWall(Position pos, Position direction, int nb_cell)
{
    int **grid=_grid->getGrid();

    for(int i=0; i<nb_cell; i++)
    {
        pos.setX(pos.x()+direction.x());
        pos.setY(pos.y()+direction.y());

        if(grid[pos.x()][pos.y()]!=0)
            return i;
    }

    return nb_cell;
}

void IA::setGrid(Grid *grid)
{
    _grid=grid;
}

IA::~IA()
{

}
