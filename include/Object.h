#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include <map>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Shader.h"
#include "Input.h"

#include "ParticleGenerator.h"


#define PI 3.1415

class Object
{
	public:
		Object();
		Object(std::vector<GLuint> ibo,Shader const& shader);
		Object(Particle &p);
		~Object();

        void addShader(Shader const& shader);
		void createVao();
		void translate(glm::vec3 vect);
		void rotate(float angle, glm::vec3 normal);
		void scale(glm::vec3 vect);
		void draw(glm::mat4 &projection, glm::mat4 &modelview);
		void autoMove(float velocity);
		GLuint getVao(){return _vao;}

		glm::vec3 getDirection() const;
		glm::vec3 getPosition() const;

		void setTextureId(GLuint texture){_textureId = texture;}
		GLuint getTextureId(){return _textureId;}

		template<typename T>
        void addVertexAttrib(std::string nom_buffer, std::vector<T> buffer, int group_elements=3, GLenum type_element=GL_FLOAT)
        {
            typename std::vector<T>::value_type type;

			GLuint id_buffer = 0;
			glGenBuffers(1, &id_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, id_buffer);
			glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(type), buffer.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			_idBuffers[nom_buffer]=id_buffer;

            glBindVertexArray(_vao);
            glBindBuffer(GL_ARRAY_BUFFER, id_buffer);
            GLint index = glGetAttribLocation(_shader.getProgramID(), nom_buffer.c_str());
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, group_elements, type_element, GL_FALSE, 0, NULL);
            glBindVertexArray(0);
        }

    private:
		GLuint _vao;
		Shader _shader;
		GLuint _textureId;
		glm::vec3 _position, _direction;
		glm::mat4 _modelWorldMatrix;
		std::vector<GLuint> _ibo;
		std::map<std::string, GLuint> _idBuffers;

};

#endif // OBJECT_H_INCLUDED
