#include "Grid.h"
#include <stdlib.h>
#include <iostream>

const Position Grid::PLAYER_POSITIONS[] = {Position(SIZE/2,SIZE/8), Position(SIZE/8,SIZE/2) , Position(SIZE/2,(7*SIZE)/8), Position((7*SIZE)/8,SIZE/2), Position(SIZE/8,SIZE/8), Position((7*SIZE)/8,(7*SIZE)/8) };
const float Grid::PLAYER_ROTATIONS[] = {0,-PI/2,PI,PI/2, -PI/2, PI/2};

Grid::Grid()
{
    _grid = new int*[SIZE];
    for(int i=0; i<SIZE; i++){
        _grid[i] = new int[SIZE];
    }
    for(int i=0; i<SIZE; i++) {
        for(int j=0; j<SIZE; j++)
            if(i==0||j==0||i==SIZE-1||j==SIZE-1)
                _grid[i][j]=-1;
            else _grid[i][j]=0;
    }
}

void Grid::update(int id, int x, int y){
    _grid[x][y] = id;
}

int** Grid::getGrid(){
    return _grid;
}

void Grid::display()
{
	for(int i=0;i<SIZE;i++)
	{
		for(int j=0;j<SIZE;j++)
		{
			printf("%d",_grid[i][j]);
		}
		printf("\n");
	}
}

void Grid::removePlayer(int id){
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++)
            if(_grid[i][j] == id)
                _grid[i][j]=0;
    }
}

