#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "vboindexer.hpp"

typedef struct modelVertices
{
	std::vector<GLuint> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
} ModelVertices;


class Texture
{
	public:
		static void loadResources();
		static GLuint loadTGATexture(const std::string& imagepath, GLenum wrap_s=GL_REPEAT, GLenum wrap_t=GL_REPEAT, GLenum mag_filter=GL_LINEAR, GLenum min_filter=GL_LINEAR_MIPMAP_LINEAR, bool anisotropy=true);
		static char *read_tga(const char *filename, int& width, int& height, int& depth);
		static bool loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<glm::vec2> & out_uvs, std::vector<glm::vec3> & out_normals);
		static short le_short(unsigned char *bytes);

		static GLuint bikeTexture, planeTexture;
		static ModelVertices bikeModel;
};


#endif // TEXTURE_H_INCLUDED
