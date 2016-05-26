#include "ParticleGenerator.h"
#include "Object.h"

ParticleGenerator::ParticleGenerator(int nb_particles)
{
    _max_particles=nb_particles;

    for(int i=0; i<_max_particles; i++)
    {
        Particle p;

        p.id=i;
        p.life=100.0;
        p.fade=myRand(0.1,0.05);

        p.r=myRand(0.0,1.0);
        p.g=myRand(0.0,1.0);
        p.b=myRand(0.0,1.0);
        p.a=1;

        p.speed=glm::vec3(myRand(-0.0001,0.0001), myRand(-0.0001,0.0001), myRand(-0.0001,0.0001));
        p.pos=glm::vec3(60,60,0);
        p.gravity=glm::vec3(0.0f, 0.0f, 0.0f);

        _particles.push_back(p);
    }
}

ParticleGenerator::~ParticleGenerator()
{
	
}

void ParticleGenerator::update_particles()
{
	int i=0;
	for(auto it = _objects.begin(); it != _objects.end(); it++)
	{
		(*it)->translate(_particles[i].speed);
		(*it)->translate(_particles[i].gravity);
		_particles[i].life -= _particles[i].fade;
		if(_particles[i].life < 0.0)
		{
		  //!loic: this is invalid and produces a seg-fault
		  //_particles.erase(_particles.begin() + i); 
		}
		i++;
	}
}

double ParticleGenerator::myRand(double min, double max)
{
    return (double) (min + ((float) rand() / RAND_MAX * (max - min + 1.0)));
}
