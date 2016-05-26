#ifndef PARTICLEGENERATOR_H_INCLUDED
#define PARTICLEGENERATOR_H_INCLUDED


#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>



#define MAX_PARTICLES 10000

class Object;

struct Particle {
    int id;
	glm::vec3 pos, speed, gravity;
	unsigned char r,g,b,a;
    double life;
    double fade;
};

class ParticleGenerator 
{
    public:
        ParticleGenerator(int nb_particles);
        ~ParticleGenerator();
        void update_particles();
        
        std::vector<Particle> getParticles(){return _particles;}
        std::vector<Object*> getObjects(){return _objects;}
        void setObjects( std::vector<Object*> objs){_objects = objs;}
        
    private:
        double myRand(double min, double max);

        int _max_particles;
        std::vector<Particle> _particles;
        std::vector<Object*> _objects;
};

#endif // PARTICLEGENERATOR_H_INCLUDED
