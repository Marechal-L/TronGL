#include "Camera.h"


Camera::Camera(float speed, float sensitivity) : _phi(0.0), _theta(0.0), _orientation(), _verticalAxis(0, 0, 1), _lateralVector(), _position(), _target(), _sensitivity(sensitivity), _speed(speed)
{

}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 verticalAxis, float speed, float sensitivity) : _phi(0.0), _theta(0.0), _orientation(), _verticalAxis(verticalAxis), _speed(speed),
                                                                                  _lateralVector(), _position(position), _target(target), _sensitivity(sensitivity)
{
    setTarget(target);
}

void Camera::init(GLFWwindow* win, float near, float far)
{
	int width = 0, height = 0;
	glfwGetWindowSize(win, &width, &height);
	_projectionMatrix = glm::perspective(45.0f, width/(float)(height/2), near, far);
}

void Camera::setViewPort(int currentPlayer)
{
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	glViewport(0,currentPlayer*height/2,width,height/2);
}

void Camera::resetViewPort()
{
	int width = 0, height = 0;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
	glViewport(0,0,width,height);
}

void Camera::orientate(int xRel, int yRel)
{
    _phi += yRel * _sensitivity;
    _theta += xRel * _sensitivity;

    if(_phi > 89.0)
        _phi = 89.0;

    else if(_phi < -89.0)
        _phi = -89.0;

    float phiRadian = _phi * M_PI / 180;
    float thetaRadian = _theta * M_PI / 180;

    if(_verticalAxis.x == 1.0)
    {
        _orientation.x = sin(phiRadian);
        _orientation.y = cos(phiRadian) * cos(thetaRadian);
        _orientation.z = cos(phiRadian) * sin(thetaRadian);
    }
    else if(_verticalAxis.y == 1.0)
    {
        _orientation.x = cos(phiRadian) * sin(thetaRadian);
        _orientation.y = sin(phiRadian);
        _orientation.z = cos(phiRadian) * cos(thetaRadian);
    }
    else
    {
        _orientation.x = cos(phiRadian) * cos(thetaRadian);
        _orientation.y = cos(phiRadian) * sin(thetaRadian);
        _orientation.z = sin(phiRadian);
    }

    _lateralVector = cross(_verticalAxis, _orientation);
    _lateralVector = normalize(_lateralVector);


    _target = _position + _orientation;
}

void Camera::move(Input &in)
{
    orientate(in.getXRel(), in.getYRel());

    if(in.isKeyPressed(GLFW_KEY_W))
    {
        _position = _position + _orientation * _speed;
        _target = _position + _orientation;
    }

    if(in.isKeyPressed(GLFW_KEY_S))
    {
        _position = _position - _orientation * _speed;
        _target = _position + _orientation;
    }

    if(in.isKeyPressed(GLFW_KEY_A))
    {
        _position = _position + _lateralVector * _speed;
        _target = _position + _orientation;
    }

    if(in.isKeyPressed(GLFW_KEY_D))
    {
        _position = _position - _lateralVector * _speed;
        _target = _position + _orientation;
    }

    in.resetCursor();
	lookAt();
}

void Camera::setSpeed(float speed)
{
    _speed=speed;
}

void Camera::setSensitivity(float sensitivity)
{
    _sensitivity=sensitivity;
}

void Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 verticalAxis)
{
	_position = position;
	_target = target;
	_verticalAxis = verticalAxis;
	lookAt();
}

void Camera::lookAt()
{
    _worldViewMatrix = glm::lookAt(_position, _target, _verticalAxis);
}

void Camera::lookAtObjectThirdPerson(Object const& obj)
{
    glm::vec3 target=obj.getPosition();
    glm::vec3 direction_obj=normalize(obj.getDirection());

    float dv=7.0f * sin(PI/4);
    float dh=7.0f * cos(PI);

    _position=glm::vec3(target.x, target.y, dv) + direction_obj * dh;
    _target=target+direction_obj;

    lookAt();
}

void Camera::setTarget(glm::vec3 const& target)
{
    _target=target;
    _orientation = _target - _position;
    _orientation = normalize(_orientation);

    if(_verticalAxis.x == 1.0)
    {
        _phi = asin(_orientation.x);
        _theta = acos(_orientation.y / cos(_phi));

        if(_orientation.y < 0)
            _theta *= -1;
    }
    else if(_verticalAxis.y == 1.0)
    {
        _phi = asin(_orientation.y);
        _theta = acos(_orientation.z / cos(_phi));

        if(_orientation.z < 0)
            _theta *= -1;
    }
    else
    {
        _phi = asin(_orientation.x);
        _theta = acos(_orientation.z / cos(_phi));

        if(_orientation.z < 0)
            _theta *= -1;
    }

    _phi = _phi * 180 / M_PI;
    _theta = _theta * 180 / M_PI;
}

void Camera::setPosition(glm::vec3 const& position)
{
    _position=position;
    _target = _position+_orientation;
}

Camera::~Camera()
{

}
