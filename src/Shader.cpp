#include "Shader.h"
#include <fstream>

Shader::Shader() : _vertexID(0), _fragmentID(0), _programID(0), _vertexSource(), _fragmentSource()
{
}


Shader::Shader(Shader const &shader)
{
    _vertexSource = shader._vertexSource;
    _fragmentSource = shader._fragmentSource;

    load();
}


Shader::Shader(std::string vertexSource, std::string fragmentSource) : _vertexID(0), _fragmentID(0), _programID(0),
                                                                       _vertexSource(vertexSource), _fragmentSource(fragmentSource)
{
}


Shader::~Shader()
{
    glDetachShader(_programID, _vertexID);
    glDetachShader(_programID, _fragmentID);

    glDeleteShader(_vertexID);
    glDeleteShader(_fragmentID);
    glDeleteProgram(_programID);
}

Shader& Shader::operator=(Shader const& shader)
{
    _vertexSource = shader._vertexSource;
    _fragmentSource = shader._fragmentSource;

    load();

    return *this;
}


bool Shader::load()
{
    if(glIsShader(_vertexID) == GL_TRUE)
        glDeleteShader(_vertexID);
    if(glIsShader(_fragmentID) == GL_TRUE)
        glDeleteShader(_fragmentID);
    if(glIsProgram(_programID) == GL_TRUE)
        glDeleteProgram(_programID);


    if(!compileShader(_vertexID, GL_VERTEX_SHADER, _vertexSource))
        return true;
    if(!compileShader(_fragmentID, GL_FRAGMENT_SHADER, _fragmentSource))
        return true;

    _programID = glCreateProgram();
    glAttachShader(_programID, _vertexID);
    glAttachShader(_programID, _fragmentID);

    glLinkProgram(_programID);


    // Vérification du linkage
    GLint erreurLink(0);
    glGetProgramiv(_programID, GL_LINK_STATUS, &erreurLink);

    if(erreurLink != GL_TRUE)
    {
        // Récupération de la taille de l'erreur
        GLint tailleErreur(0);
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &tailleErreur);

        // Allocation de mémoire
        char *erreur = new char[tailleErreur + 1];


        // Récupération de l'erreur
        glGetShaderInfoLog(_programID, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        // Affichage de l'erreur
        std::cout << erreur << std::endl;


        // Libération de la mémoire et retour du booléen false
        delete[] erreur;
        glDeleteProgram(_programID);

        return true;
    }

    return false;
}


bool Shader::compileShader(GLuint &shader, GLenum type, std::string const &fichierSource)
{
    shader = glCreateShader(type);


    // Vérification du shader
    if(shader == 0)
    {
        std::cout << "Erreur, le type de shader (" << type << ") n'existe pas" << std::endl;
        return false;
    }

    // Flux de lecture
    std::ifstream fichier(fichierSource.c_str());


    // Test d'ouverture
    if(!fichier)
    {
        std::cout << "Erreur le fichier " << fichierSource << " est introuvable" << std::endl;
        glDeleteShader(shader);

        return false;
    }

    // Strings permettant de lire le code source
    std::string ligne;
    std::string codeSource;


    while(getline(fichier, ligne))
        codeSource += ligne + '\n';

    fichier.close();


    const GLchar* chaineCodeSource = (const GLchar*)codeSource.c_str();


    glShaderSource(shader, 1, &chaineCodeSource, 0);
    glCompileShader(shader);


    // Vérification de la compilation
    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);


    // S'il y a eu une erreur
    if(erreurCompilation != GL_TRUE)
    {
        GLint tailleErreur(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);

        char *erreur = new char[tailleErreur + 1];

        glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';

        std::cout << erreur << std::endl;

        delete[] erreur;
        glDeleteShader(shader);

        return false;
    }

    return true;
}

void Shader::bind()
{
    glUseProgram(_programID);
}

void Shader::unbind()
{
    glUseProgram(0);
}

GLuint Shader::getProgramID() const
{
    return _programID;
}
