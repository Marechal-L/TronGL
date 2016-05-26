#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unistd.h>

#include <irrKlang.h>
#include "conio.h"

#include "Input.h"
#include "Camera.h"
#include "Object.h"

#include "IA.h"
#include "Human.h"
#include "Grid.h"

#include "ParticleGenerator.h"

#define MAX_FPS 60.0

class Scene
{
	public:

		Scene()
		{
			
		}

		void addObject(std::string name, Object *o)
		{
			objects[name] = o;
		}
		void removeObject(std::string name)
		{
			delete objects[name];
			objects.erase(name);
		}

		Object& getObject(std::string name)
		{
			return *(objects[name]);
		}

		bool hasObject(std::string name)
		{
			auto it = objects.find(name);
			return it != objects.end();
		}

		void draw(Camera &camera)
		{
			typedef std::map<std::string, Object*>::iterator it_type;
			for(it_type it = objects.begin(); it != objects.end(); it++)
			{
				it->second->draw(camera.getProjectionMatrix(), camera.getWorldViewMatrix());
			}
		}

		void deleteObjects(std::string pattern)
		{
			typedef std::map<std::string, Object*>::iterator it_type;

			std::vector<std::string> toRemove;
			for(it_type it = objects.begin(); it != objects.end(); it++)
			{
				if(it->first.find(pattern) != std::string::npos){
					toRemove.push_back(it->first);
				}
			}
			for(std::string str : toRemove){
				removeObject(str);
			}
		}

		void clear()
		{
			typedef std::map<std::string, Object*>::iterator it_type;
			for(it_type it = objects.begin(); it != objects.end(); it++)
			{
				GLuint vao = it->second->getVao();
				glDeleteVertexArrays(1,&vao);
			}
		}

	private:

		std::map<std::string, Object*> objects;
};

class Application {

    public:
        Application();
        Application(std::string name_window, int width=1080, int height=920);
        bool createOpenGLContext();
        int run();
        ~Application();
    private:
        bool initGLFW();
        bool initGLEW();

        float FPSCalculator(unsigned int &nbFrames, float &lastTime);
        void renderFrame();
        void renderParticles();
        void updatePlayer();
        void addPlayer(bool isAI);

        std::vector <Player *> _humanPlayers;
        std::vector <Player *> _players;

        Grid _grid;
        std::string _name;
        int _width, _heigh;
        GLFWwindow* _window;
        Scene scene;
        Camera camera;

		unsigned int pauseMode = 0;
        unsigned int cameraMode = 1;
		unsigned int currentPlayerView = 0;

		irrklang::ISoundEngine* _engine;
		irrklang::ISound* _motorNoise;
		irrklang::ISound* _boostNoise;
		
		ParticleGenerator _particleGenerator;
};

#endif
