#ifndef HUMAIN_H_INCLUDED
#define HUMAIN_H_INCLUDED

#include <Player.h>

class Human : public Player {
    public:
        Human();
        Human(Position position, int id, Object *o, glm::vec3 color);
        ~Human();

        void move() override;
};


#endif // HUMAIN_H_INCLUDED
