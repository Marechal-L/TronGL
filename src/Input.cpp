#include "Input.h"
#include "cstring"

Input Input::_in=Input();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Input &in=Input::getInstance();

    switch(action)
    {
        case GLFW_PRESS:
            if(key==-1)
                break;
            in._stateKey[key]=true;
        case GLFW_REPEAT:
            break;
        case GLFW_RELEASE:
            if(key==-1)
                break;
            in._stateKey[key]=false;
            break;
        default:
            break;
    }
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    Input &in=Input::getInstance();

    in._xrel=in._xpos-xpos;
    in._yrel=in._ypos-ypos;

    in._xpos=xpos;
    in._ypos=ypos;
}

void cursor_scroll_callback(GLFWwindow* window, double xrel, double yrel)
{
    Input &in=Input::getInstance();

    in._xrel=xrel;
    in._yrel=yrel;
}

bool Input::init()
{
    GLFWwindow *window = glfwGetCurrentContext();

    if(!window)
        return false;

    glfwSetKeyCallback(window, key_callback);
    //glfwSetScrollCallback (window, cursor_scroll_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

    return true;
}

Input::Input()
{
    memset(_stateKey, 0, GLFW_KEY_LAST*sizeof(bool));
    _xrel=0;
    _yrel=0;
}

Input::~Input() {}

Input& Input::getInstance()
{
    return _in;
}

bool Input::isKeyPressed(int keyCode) const
{
    return _stateKey[keyCode];
}

void Input::setKeyUnpressed(int keyCode)
{
    _stateKey[keyCode]=false;
}

void Input::getMousePosition(int &x, int &y) const
{
    x=_xpos;
    y=_ypos;
}

void Input::setHiddenMouse(bool hide)
{
    GLFWwindow *window=glfwGetCurrentContext();

    if(hide)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

}

void Input::resetCursor()
{
    _xrel=0;
    _yrel=0;
}

int Input::getXRel() const
{
    return _xrel;
}

int Input::getYRel() const
{
    return _yrel;
}


