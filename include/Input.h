#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Input {

    public:
        static bool init();
        static Input &getInstance();

        bool isKeyPressed(int keyCode) const;
        void setKeyUnpressed(int keyCode);
        void getMousePosition(int &x, int &y) const;
        void setHiddenMouse(bool hide);
        void resetCursor();
        int getXRel() const;
        int getYRel() const;
    private:
        Input& operator=(const Input&){}
        Input (const Input&){}

        Input();
        ~Input();

        friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        friend void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
        friend void cursor_scroll_callback(GLFWwindow* window, double xrel, double yrel);

        int _xrel, _yrel;
        int _xpos, _ypos;
        bool _stateKey[GLFW_KEY_LAST];
        static Input _in;
};


#endif // INPUT_H_INCLUDED
