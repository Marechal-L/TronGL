#include <glm/glm.hpp>
#include "Player.h"


void Player::updateWall(Object* wall)
{
	wall->scale(glm::vec3(1,1/_currentScale,1));
	wall->translate(glm::vec3(0,1.0f / Grid::SIZE * 60,0 ));
	_currentScale+=1.0f / Grid::SIZE * 60;
	wall->scale(glm::vec3(1,_currentScale,1));
}

void Player::rollBackWall(){
	_wall->scale(glm::vec3(1,1/_currentScale,1));
	_wall->translate(-glm::vec3(0,1.0f / Grid::SIZE * 60*5,0 ));
	_currentScale-=1.0f / Grid::SIZE * 60 * 5;
	_wall->scale(glm::vec3(1,_currentScale,1));
}

Object* Player::create_wall()
{
	std::vector<glm::vec3> vertex
	{
		glm::vec3(0.1,-1,-1), glm::vec3(0.1,1,-1), glm::vec3(0.1,1,1), glm::vec3(0.1,-1,1),
		glm::vec3(-0.1,-1,-1), glm::vec3(-0.1,1,-1), glm::vec3(-0.1,1,1), glm::vec3(-0.1,-1,1)
	};
	/*std::vector<glm::vec3> colors
	{
		glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1),
		glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1)
	};*/
	std::vector<glm::vec3> colors
	{
		_color,_color,_color,_color,
		_color,_color,_color,_color
	};

	std::vector<GLuint> ibo
	{
        	0,1,2, 0,2,3 ,4,6,5 ,4,7,6, 2,3,6, 3,7,6 ,0,3,4 ,7,3,4, 1,2,5, 2,5,6

	};


	Object *o = new Object(ibo, Shader("shaders/couleur3D.v.glsl","shaders/couleur3D.f.glsl"));
	o->createVao();
	o->addVertexAttrib("in_Position", vertex);
	o->addVertexAttrib( "in_Color",  colors);

	return o;
}

Player::Player()
{

}

Player::Player(Position pos, int id, Object *o, glm::vec3 color)
{
    _direction = (id-1)%4;
    _pos = pos;
    _id = id;
    _turned = false;
    _object = o;
	_isDead = false;
	_boost = MAXBOOST;
	_cooldown = 0;
	_color = color;
	_wall = create_wall();

}

bool Player::newWall(){
	bool wall = _newWall;
	_newWall = false;
	return wall;
}

Object* Player::getWall(){
	return _wall;
}

Position Player::getPos(){
    return _pos;
}

std::vector<Position> Player::getTrail(){
    return _trail;
}


void Player::turn(int val){
    if(!_turned){
        _direction = (_direction + val + 4) % 4;
        _turned = true;
		_object->rotate((-PI/2)*val,glm::vec3(0,0,1));
		rollBackWall();
		_wall = create_wall();
		_wallNum++;
		_newWall = true;
		_currentScale = 1;
    }//a voir
}

int Player::getWallNum(){
	return _wallNum;
}

int Player::getDirection(){
	return _direction;
}

void Player::update(int ** grid)
{
	glm::vec3 trans = glm::vec3(0,1.0f / Grid::SIZE * 60*2,0);
    _trail.push_back(_pos);
    if(_isBoosted){
		//Move forward
        _pos = Position(_pos.x() + _directions[_direction].x(),_pos.y() + _directions[_direction].y());
        _object->translate(trans);
        updateWall(_wall);

        _isBoosted = false;
        if (colided(grid))
		{
			_isDead = true;
			return;
		}
        update(grid);
    }
	//Move forward
	_object->translate(trans);
	updateWall(_wall);
    _pos = Position(_pos.x() + _directions[_direction].x(),_pos.y() + _directions[_direction].y());
    if (colided(grid))
	{
		_isDead = true;
		return;
	}
	_turned = false;
    if(_boost<MAXBOOST){
        if (_cooldown++ >=COOLDOWNTIME){
            _boost++;
            _cooldown = 0;
        }
    }
    _isDead = false;
}

void Player::useBoost(){
    if(_boost > 0){
		printf("Boosted\n");
        _isBoosted = true;
        _boost--;
    }
}

int Player::getID(){
    return _id;
}

void Player::cameraControls(const int totalPlayers)
{
	Input &in=Input::getInstance();

	if(in.isKeyPressed(_keyMap["PREVIOUS"]))
	{
		_spectatePlayer += totalPlayers-1;
		_spectatePlayer %= totalPlayers;
		in.setKeyUnpressed(_keyMap["PREVIOUS"]);
	}
	if(in.isKeyPressed(_keyMap["NEXT"]))
	{
		_spectatePlayer++;
		_spectatePlayer %= totalPlayers;
		in.setKeyUnpressed(_keyMap["NEXT"]);
	}
}

bool Player::colided(int ** grid)
{
    return grid[_pos.x()][_pos.y()] != 0;
}

void Player::setKeyMap(const int *keyTab)
{
	setKeyMap(keyTab[0],keyTab[1],keyTab[2],keyTab[3],keyTab[4]);
}

void Player::setKeyMap(const int up, const int left, const int right, const int previous, const int next)
{
	_keyMap["UP"] = up;
	_keyMap["LEFT"] = left;
	_keyMap["RIGHT"] = right;
	_keyMap["PREVIOUS"] = previous;
	_keyMap["NEXT"] = next;
}

bool Player::isBoosted(){
    return _isBoosted;
}

