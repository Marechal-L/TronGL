#include "Object.h"
#include <glm/gtc/type_ptr.hpp>

Object::Object()
{

}


Object::Object(std::vector<GLuint> ibo,Shader const& shader) : _shader(shader), _vao(0), _idBuffers(), _position(0,0,0), _direction(0,1,0), _textureId(-1)
{
    _ibo=ibo;
}

Object::Object(Particle &p)
: _direction(0,1,0), _textureId(-1)
{
	std::vector<glm::vec3> positions
	{
		p.pos + glm::vec3(0,1,0), p.pos + glm::vec3(0,0,1), p.pos + glm::vec3(0,-1,0)
	};

	glm::vec3 color = glm::vec3(p.r, p.g, p.b);
	
	std::vector<glm::vec3> colors
	{
		color,color,color
	};
	
	std::vector<GLuint> ibo
	{
		0,1,2
	};

	_shader = Shader("shaders/couleur3D.v.glsl","shaders/couleur3D.f.glsl");
	_ibo = ibo;

    createVao();
    addVertexAttrib("in_Position", positions);
    addVertexAttrib("in_Color", colors);    
}

Object::~Object()
{
    for(std::map<std::string,GLuint>::iterator it=_idBuffers.begin(); it!=_idBuffers.end(); ++it)
    {
        glDeleteBuffers(1, &it->second);
    }

    _idBuffers.clear();

    glDeleteVertexArrays(1, &_vao);
}

void Object::addShader(Shader const& shader)
{
    _shader=shader;
}

void Object::createVao()
{
    GLuint indexBuffer;

    //IBO - Indices
    indexBuffer = 0;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _ibo.size() * sizeof(GLuint) , _ibo.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    _idBuffers["ibo"]=indexBuffer;

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBindVertexArray(0);
}

void Object::translate(glm::vec3 vect)
{
	_modelWorldMatrix = glm::translate(_modelWorldMatrix,vect);
	_position = glm::vec3(_modelWorldMatrix[3][0], _modelWorldMatrix[3][1], _modelWorldMatrix[3][2]);
}

void Object::rotate(float angle, glm::vec3 normal)
{
	_modelWorldMatrix = glm::rotate(_modelWorldMatrix,angle,normal);
	_direction = glm::rotate(_direction,angle,normal);
	_position = glm::vec3(_modelWorldMatrix[3][0], _modelWorldMatrix[3][1], _modelWorldMatrix[3][2]);
}

void Object::scale(glm::vec3 vect)
{
	_modelWorldMatrix = glm::scale(_modelWorldMatrix,vect);
}

void Object::draw(glm::mat4 &projection, glm::mat4 &worldView)
{
    _shader.bind();
    glBindVertexArray(_vao);

	//glUniform1f(glGetUniformLocation(_shader.getProgramID(),"time"),(float)glfwGetTime());
	glUniformMatrix4fv(glGetUniformLocation(_shader.getProgramID(), "modelWorld"), 1, GL_FALSE, value_ptr(_modelWorldMatrix));
    glUniformMatrix4fv(glGetUniformLocation(_shader.getProgramID(), "worldView"), 1, GL_FALSE, value_ptr(worldView));
    glUniformMatrix4fv(glGetUniformLocation(_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
    if(_textureId != -1){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glUniform1i(glGetUniformLocation(_shader.getProgramID(), "colormap"), 0); //!loic: 0 not GL_TEXTURE_0 !!!!
    }

    glDrawElements(GL_TRIANGLES, _ibo.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    _shader.unbind();
}

glm::vec3 Object::getPosition() const
{
    return _position;
}


glm::vec3 Object::getDirection() const
{
	return _direction;
}
