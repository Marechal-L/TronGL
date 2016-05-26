#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <glm/glm.hpp>
#include "Position.h"
#include "Object.h"
#include "Grid.h"


class Player
{
public:
	Player();
	virtual ~Player() {}
    Player(Position position, int id, Object *o, glm::vec3 color);
    Position getPos();
    int getDirection();
    std::vector<Position> getTrail();
    void turn(int);
    void update(int **);
    glm::vec3 color();
    void useBoost();
    int getID();
    void rollBackWall();

	bool isDead(){return _isDead;}

	int getSpectatePlayer(){return _spectatePlayer;}
	void setSpectatePlayer(int player){_spectatePlayer = player;}

    void setKeyMap(const int *keyTab);
    void setKeyMap(const int up, const int left, const int right, const int previous, const int next);
    void cameraControls(const int playersLeft);

    virtual void move() = 0;

    Object* getWall();
    bool newWall();
    int getWallNum();
    void updateWall(Object* wall);
    Object* create_wall();
    bool isBoosted();

protected:

	Position _pos;
    Position _directions[4] = {Position(0,1), Position(1,0), Position(0,-1), Position(-1,0)};
    int _direction;
    std::map<std::string, GLuint> _keyMap;
    std::vector<Position> _trail;

    int _wallNum = 0;
    int _boost;
    bool _isBoosted;
    bool _turned;
    int _cooldown;

    static const int MAXBOOST = 10;
    static const int COOLDOWNTIME = 180;
    int _id;
    bool colided(int ** grid);
	Object *_object;
	bool _isDead;

	int _spectatePlayer = 0;

	Object* _wall;
	bool _newWall = false;
	float _currentScale = 1;
	glm::vec3 _color;
};

#endif

