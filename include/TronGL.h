#ifndef MY_OWN
#define MY_OWN

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/func_trigonometric.hpp>

void init();
void initGLFW();
void initGLEW();
void make_resources();
void make_shader_program();
void make_triangle();
void make_torus();
void make_torus_triangle();
void renderFrame();
void shutDown(int return_code);
void make_wall(glm::vec3, glm::vec3);


void faceCulling(bool activate);
void depthTest(bool activate);
void *file_contents(const char* filename, GLint *length);

#endif
