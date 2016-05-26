#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "Texture.h"

#define PI 3.1415

std::vector<glm::vec3> playerColors
{
		glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1),glm::vec3(1,0,1), glm::vec3(1,1,1), glm::vec3(0.5f,0.8f,1)
};

glm::vec3 torusPoint(double theta, double phi, double R, double r)
{
    double D=(R + r*cos(phi));
    return glm::vec3(D*cos(theta), D*sin(theta), r*sin(phi));
}

void displayMat4(const glm::mat4& m)
{
    std::cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl
              << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl
              << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl
              << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
}

Object createTorus()
{
	double R = 1, r = 0.25;
	int nbBins = 50;
	
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> uvs;

	std::vector<GLuint> ibo;

    int N = nbBins * nbBins;

    for(int i=0; i<nbBins; i++)
    {
		for(int j=0; j<nbBins; j++)
		{
			//Conversion en Radian
			float theta = i * 360/nbBins * PI/180;
			float phi = j * 360/nbBins * PI/180;

			glm::vec3 point = torusPoint(theta, phi, R, r);
			vertex.push_back(point);
			colors.push_back(glm::vec3(1,0,1));
			uvs.push_back(glm::vec2(1/(float)nbBins * i,1/(float)nbBins * j));

			//numPoint = j*nbBins + i
			//Triangle 1
			ibo.push_back(j*nbBins + i);
			ibo.push_back(((j+1)%nbBins)*nbBins + i);
			ibo.push_back(j*nbBins + (i+1)%nbBins);

			//Triangle 2
			ibo.push_back(((j+1)%nbBins)*nbBins + (i+1)%nbBins);
			ibo.push_back(j*nbBins + (i+1)%nbBins);
			ibo.push_back(((j+1)%nbBins)*nbBins + i);
		}
	}

	Object obj(ibo,Shader("shaders/couleur3D.v.glsl","shaders/couleur3D.f.glsl"));
    obj.createVao();
    obj.addVertexAttrib("in_Position", vertex);
    obj.addVertexAttrib("in_Color", colors);

	return obj;
}

Object* createTexturedTorus()
{	
	double R = 1, r = 0.25;
	int nbBins = 50;
	
    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> uvs;

	std::vector<GLuint> ibo;

    int N = nbBins * nbBins;

    for(int i=0; i<nbBins; i++)
    {
		for(int j=0; j<nbBins; j++)
		{
			//Conversion en Radian
			float theta = i * 360/nbBins * PI/180;
			float phi = j * 360/nbBins * PI/180;

			glm::vec3 point = torusPoint(theta, phi, R, r);
			vertex.push_back(point);
			colors.push_back(glm::vec3(1,0,1));

			uvs.push_back(glm::vec2((float) 1/(float)nbBins * j,(float)  1/(float)nbBins * i));
			

			//numPoint = j*nbBins + i
			//Triangle 1
			ibo.push_back(j*nbBins + i);
			ibo.push_back(((j+1)%nbBins)*nbBins + i);
			ibo.push_back(j*nbBins + (i+1)%nbBins);

			//Triangle 2
			ibo.push_back(((j+1)%nbBins)*nbBins + (i+1)%nbBins);
			ibo.push_back(j*nbBins + (i+1)%nbBins);
			ibo.push_back(((j+1)%nbBins)*nbBins + i);
		}
	}
	
	GLuint textureId = Texture::loadTGATexture("textures/brick_colormap.tga");
    glActiveTexture(GL_TEXTURE0 + textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	Object *obj = new Object(ibo,Shader("shaders/texture.v.glsl","shaders/texture.f.glsl"));

    obj->createVao();
    obj->addVertexAttrib("in_Position", vertex);
    obj->addVertexAttrib("in_Color", colors);
    obj->addVertexAttrib("in_UV",  uvs, 2);
    obj->setTextureId(textureId);
   
	return obj;
}

Object* createPlane()
{
	std::vector<glm::vec3> vertex
	{
		glm::vec3(0,0,0), glm::vec3(2,0,0), glm::vec3(2,2,0), glm::vec3(0,2,0),
		glm::vec3(0,0,0), glm::vec3(2,0,0), glm::vec3(2,2,0), glm::vec3(0,2,0)
	};
	std::vector<glm::vec3> colors
	{
		glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1),
		glm::vec3(1,0,0), glm::vec3(0,0,1), glm::vec3(0,0,1), glm::vec3(0,0,1)
	};
	std::vector<glm::vec2> uvs
	{
		glm::vec2(0,1), glm::vec2(1,1), glm::vec2(1,0), glm::vec2(0,0)
	};

	for(glm::vec2 &uv : uvs)
	{
		uv *= 10;
	}
	
	std::vector<GLuint> ibo
	{
		0,1,2, 0,2,3 ,4,6,5 ,4,7,6
	};


	Object *obj = new Object(ibo,Shader("shaders/texture.v.glsl","shaders/texture.f.glsl"));

    obj->createVao();
    obj->addVertexAttrib("in_Position", vertex);
    obj->addVertexAttrib("in_Color", colors);
    obj->addVertexAttrib("in_UV", uvs,2);
	obj->setTextureId(Texture::planeTexture);

	obj->scale(glm::vec3(60,60,1));

	return obj;
}

Object* createBike(int colorIndex)
{
	std::vector<glm::vec3> colors;

	for(unsigned int i=0;i<Texture::bikeModel.vertices.size();i++)
	{
		colors.push_back(playerColors[colorIndex]);	
	}

	Object *obj = new Object(Texture::bikeModel.indices, Shader("shaders/bike.v.glsl","shaders/bike.f.glsl"));

    obj->createVao();
    obj->addVertexAttrib("in_Position", Texture::bikeModel.vertices);
    obj->addVertexAttrib("in_Normal", Texture::bikeModel.normals);
    obj->addVertexAttrib("in_UV",  Texture::bikeModel.uvs, 2);
	obj->addVertexAttrib("in_Color", colors);
    obj->setTextureId(Texture::bikeTexture);
	
	return obj;
}

#endif // UTILS_H_INCLUDED
