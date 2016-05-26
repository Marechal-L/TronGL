#include <Human.h>
#include <iostream>

Human::Human() : Player()
{

}

Human::Human(Position position, int id, Object *o, glm::vec3 color) : Player(position, id, o,color)
{

}

Human::~Human()
{

}

void Human::move()
{
    Input &in=Input::getInstance();

	if(in.isKeyPressed(_keyMap["UP"]))
	{
		useBoost();
		in.setKeyUnpressed(_keyMap["UP"]);
	}
	if(in.isKeyPressed(_keyMap["LEFT"]))
	{
		turn(-1);
		in.setKeyUnpressed(_keyMap["LEFT"]);
	}
	if(in.isKeyPressed(_keyMap["RIGHT"]))
	{
		turn(1);
		in.setKeyUnpressed(_keyMap["RIGHT"]);
	}
}
