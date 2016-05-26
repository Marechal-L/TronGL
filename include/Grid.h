#ifndef GRID_H
#define GRID_H

#define PI 3.1415

#include "Position.h"

class Grid
{
public:
    Grid();
    void update(int, int, int);
    void removePlayer(int);
    int** getGrid();
    void display();

    const static int SIZE = 300;

    static const Position PLAYER_POSITIONS[];
	static const float PLAYER_ROTATIONS[];

private:
    int ** _grid;

};

#endif // GRID_H
