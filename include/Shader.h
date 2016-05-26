#ifndef DEF_SHADER
#define DEF_SHADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class Shader
{
    public:
        Shader();
        Shader(Shader const& shader);
        Shader(std::string vertexSource, std::string fragmentSource);
        ~Shader();

        Shader& operator=(Shader const& shader);

        bool load();
        bool compileShader(GLuint &shader, GLenum type, std::string const& fichierSource);
        void bind();
        void unbind();
        GLuint getProgramID() const;

    private:
        GLuint _vertexID;
        GLuint _fragmentID;
        GLuint _programID;

        std::string _vertexSource;
        std::string _fragmentSource;
};

#endif
