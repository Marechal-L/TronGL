#ifndef DEF_CAMERA
#define DEF_CAMERA

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"
#include "Object.h"

class Camera
{
public:
    Camera(float speed=0.1f, float sensitivity=0.1f);
    Camera(glm::vec3 position, glm::vec3 pointCible, glm::vec3 axeVertical, float speed=0.1f, float sensitivity=0.1f);
    ~Camera();

    void init(GLFWwindow* win, float near, float far);
    void orientate(int xRel, int yRel);
    void move(Input &in);
    void lookAt();
    void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 verticalAxis);
    void lookAtObjectThirdPerson(Object const& obj);
    void setTarget(glm::vec3 const& target);
    void setPosition(glm::vec3 const& position);
	void setViewPort(int currentPlayer);
	void resetViewPort();

    glm::mat4& getWorldViewMatrix() {return _worldViewMatrix;}
    glm::mat4& getProjectionMatrix() {return _projectionMatrix;}

    void setSpeed(float speed);
    void setSensitivity(float sensitivity);
    
	

private:
    float _phi;
    float _theta;
    float _sensitivity;
    float _speed;
    glm::vec3 _orientation;

    glm::vec3 _verticalAxis;
    glm::vec3 _lateralVector;

    glm::vec3 _position;
    glm::vec3 _target;

    glm::mat4 _worldViewMatrix;
	glm::mat4 _projectionMatrix;



};

#endif
