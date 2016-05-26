#include <iostream>
#include <fstream>
#include "Application.h"

int main(void)
{
    Application app("TronGl - Opengl 4.1");

    if(!app.createOpenGLContext())
        exit(-1);

    return app.run();
}
