#ifndef IA_H
#define IA_H

#include "Player.h"

class IA : public Player
{
	public:
        IA();
		IA(Position position, int id, Object *o, glm::vec3 color);
		void move() override;
		void setGrid(Grid *grid);
		~IA();
    private:
        int hasWall(Position pos, Position direction, int nb_cell);
        Grid *_grid;
};

#endif
