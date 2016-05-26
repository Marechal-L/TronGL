#include <vector>
#include <queue>
#include "Application.h"
#include "Object.h"
#include "utils.h"

Application::Application() : _name("Context OpenGl 4.1"), _width(1080), _heigh(920), _particleGenerator(ParticleGenerator(30))
{

}

Application::Application(std::string name_window, int width, int height) : _name(name_window), _width(width), _heigh(height), _particleGenerator(ParticleGenerator(30))
{
    _engine = irrklang::createIrrKlangDevice();

	if (!_engine)
	{
		printf("Could not startup engine\n");
	}else	_engine->play2D("media/mainTheme.wav", true);
    _motorNoise = _engine->play2D("media/engine.wav", true, false, true);
    _boostNoise = nullptr;
}

bool Application::createOpenGLContext()
{
    if(!initGLFW())
        return false;

    if(!initGLEW())
        return false;

    if(!Input::init())
        return false;

    return true;
}

bool Application::initGLFW()
{
    if (!glfwInit()) {
        std::cerr<<"Could not initailize GLFW."<<std::endl;
        return false;
    }

    // Create OpenGL 4.1 Core Profile and Forward Compatible Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window of given size
    _window =glfwCreateWindow(_width, _heigh, _name.c_str(), NULL, NULL);
    if (!_window)
    {
        std::cerr<<"Could not open a window"<<std::endl;
        return false;
    }

    // Make the context of the specified window current
    glfwMakeContextCurrent(_window);
    return true;
}

bool Application::initGLEW()
{
    /* GLEW Initialization */
    // Do not forget to use glewExperimental so that glewInit works with core
    // profile: http://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();
    if (GlewInitResult != GLEW_OK)
    {
        std::cerr<<"ERROR: "<< glewGetErrorString(GlewInitResult)<<std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    return true;
}

float Application::FPSCalculator(unsigned int &nbFrames, float &lastTime)
{
	float currentTime = glfwGetTime();
	nbFrames++;
	if(currentTime - lastTime >= 1.0)
	{
		float wait = 1000.0/(double)nbFrames;

		nbFrames = 0;
		lastTime += 1.0;

		return wait;
	}
	return -1;
}

void Application::renderFrame()
{
	float timer = glfwGetTime();

	if(pauseMode != 1)
	{
		updatePlayer();
		renderParticles();
	}

    _humanPlayers[0]->cameraControls(_players.size());
	_humanPlayers[1]->cameraControls(_players.size());

	if(_players.size() == 0)
	{
		pauseMode = 1;
	}

	if(pauseMode == 0)
	{
		//Viewport 1
		camera.setViewPort(0);
		std::string name;
		if(!_humanPlayers[0]->isDead())
		{
			name = "Player1";
		}
		else
		{
			if(_humanPlayers[0]->getSpectatePlayer() > _players.size()-1)
			{
				_humanPlayers[0]->setSpectatePlayer(0);
			}
			name = "Player"+std::to_string(_players[_humanPlayers[0]->getSpectatePlayer()]->getID());
		}
		camera.lookAtObjectThirdPerson(scene.getObject(name));
		scene.draw(camera);

		//Viewport 2
		camera.setViewPort(1);
		if(!_humanPlayers[1]->isDead())
		{
			name = "Player2";
		}
		else
		{
			if(_humanPlayers[1]->getSpectatePlayer() > _players.size()-1)
			{
				_humanPlayers[1]->setSpectatePlayer(0);
			}
			name = "Player"+std::to_string(_players[_humanPlayers[1]->getSpectatePlayer()]->getID());
		}
		camera.lookAtObjectThirdPerson(scene.getObject(name));
		scene.draw(camera);
	}
	else
	{
		camera.resetViewPort();
		camera.move(Input::getInstance());
		scene.draw(camera);
	}

	float renderTime = glfwGetTime() - timer;
	if(renderTime < 1000/MAX_FPS)
	{
		usleep((1000/MAX_FPS - renderTime)*1000);
	}
}


void Application::renderParticles()
{
	_particleGenerator.update_particles();
}

void showVec3(glm::vec3 vec){

    std::cout << vec.x << ":" << vec.y << ":" << vec.z << std::endl;
}

void Application::updatePlayer()
{
    for(int i=0;i<_players.size();i++)
    {
        if (!_boostNoise){
            _boostNoise = _engine->play2D("media/boost.wav", false, true, true);
        }
        if (_players[i]->isBoosted() && !_boostNoise->isFinished())
        {
            _boostNoise->setIsPaused(false);
            _motorNoise->setIsPaused(true);
        }

        if (!_players[i]->isBoosted() && _boostNoise->isFinished())
        {
            _boostNoise = _engine->play2D("media/boost.wav", false, true, true);
            _boostNoise->setIsPaused(true);
            _motorNoise->setIsPaused(false);
        }


		_players[i]->update(_grid.getGrid());
        _grid.update(_players[i]->getID(),_players[i]->getPos().x(),_players[i]->getPos().y());
    }

	for(int i=0;i<_players.size();i++)
    {
	 	if (_players[i]->isDead())
        {
			std::string name = "Player"+std::to_string(_players[i]->getID());
			std::cout << name << " is dead." << std::endl;

			_engine->play2D("media/boom.wav", false);
            _grid.removePlayer(_players[i]->getID());
            scene.deleteObjects("wall"+std::to_string(_players[i]->getID()));
            _players.erase(_players.begin()+i);
			scene.removeObject(name);
        }
    }
}

void Application::addPlayer(bool isAI)
{
	if(_players.size() == 4)
	{
		printf("No more players allowed\n");
		return;
	}
	
	
	int playerIndex = _players.size() + 1;
	std::string name = "Player"+std::to_string(playerIndex);

	Object *o = createBike(playerIndex-1);
	Position position = Grid::PLAYER_POSITIONS[playerIndex-1];
	o->translate(glm::vec3((float)position.x() / Grid::SIZE * 60*2, (float)position.y() / Grid::SIZE * 60*2 , 0.5f));
	o->rotate(Grid::PLAYER_ROTATIONS[playerIndex-1],glm::vec3(0,0,1));
	scene.addObject(name,o);

	Player *p;

	if(isAI)
	{
        IA *ia=new IA(Grid::PLAYER_POSITIONS[playerIndex-1], playerIndex, o,playerColors[playerIndex-1]);
        ia->setGrid(&_grid);
		p = ia;
	}
	else
	{
		p = new Human(Grid::PLAYER_POSITIONS[playerIndex-1], playerIndex, o,playerColors[playerIndex-1]);
	}

	Object* wall = p->getWall();
	wall->translate(glm::vec3((float)position.x() / Grid::SIZE * 60*2, (float)position.y() / Grid::SIZE * 60*2 , 0));
	wall->rotate(Grid::PLAYER_ROTATIONS[playerIndex-1],glm::vec3(0,0,1));
	std::string nameWall = "wall" +std::to_string(playerIndex);
	wall->translate(glm::vec3(0,1.0f / Grid::SIZE * 60*5,0 ));
	scene.addObject(nameWall, wall);
    _players.push_back(p);
}

int Application::run()
{
	Texture::loadResources();

    Input &in=Input::getInstance();
    in.setHiddenMouse(true);
    Object *plane = createPlane();
    scene.addObject("plane",plane);
    
    
    Object *torus = createTexturedTorus();
    torus->translate(glm::vec3(60,60,-10));
    scene.addObject("torus",torus);
	

    camera = Camera(0.5f);
    camera.init(glfwGetCurrentContext(),0.1f,1000);
    camera.lookAt(glm::vec3(0,2,30),glm::vec3(0,0,0),glm::vec3(0,0,1));

	 _grid = Grid();

	addPlayer(false);
	addPlayer(false);
    addPlayer(true);
    addPlayer(true);


    float lastTime = glfwGetTime();
	unsigned int nbFrames = 0;
	float wait;


	_players[0]->setKeyMap(GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_KP_4, GLFW_KEY_KP_6);
	_players[1]->setKeyMap(GLFW_KEY_T, GLFW_KEY_F, GLFW_KEY_H, GLFW_KEY_V, GLFW_KEY_N );

	_players[0]->setSpectatePlayer(0);
	_players[1]->setSpectatePlayer(1);

	_humanPlayers.push_back(_players[0]);
	_humanPlayers.push_back(_players[1]);

	
	//Create Particle effect
	_particleGenerator = ParticleGenerator(10);
	std::vector<Particle> particles = _particleGenerator.getParticles();
	std::vector<Object*> objs;
	
	int i=0;
	std::string name;
	for(auto it = particles.begin(); it != particles.end(); it++)
	{
		Object *o = new Object(particles[i]);
		name = "Part" + std::to_string(i);
		scene.addObject(name,o);
		objs.push_back(o);
		i++;
	}
	_particleGenerator.setObjects(objs);


	//Main loop
    while(!glfwWindowShouldClose(_window))
    {
		if((wait = FPSCalculator(nbFrames, lastTime)) != -1)
		{
			printf("%f FPS \n", 1/wait * 1000);
		}

		//Close
		if(in.isKeyPressed(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(_window,true);
			in.setKeyUnpressed(GLFW_KEY_ESCAPE);
		}

		if(in.isKeyPressed(GLFW_KEY_P))
		{
			pauseMode++;
			pauseMode %= 3;
			in.setKeyUnpressed(GLFW_KEY_P);

		}

		if(pauseMode != 1)
		{
			//Move players
            if (_motorNoise){
                _motorNoise->setIsPaused(false);
            }

			for(int i=0;i<_players.size();i++)
			{
				_players[i]->move();
				if(_players[i]->newWall())
				{
					Position position = _players[i]->getPos();
					std::string nameWall = "wall" + std::to_string(_players[i]->getID()) + "_" + std::to_string(_players[i]->getWallNum());
					Object* wall = _players[i]->getWall();
					wall->translate(glm::vec3((float)position.x() / Grid::SIZE * 60*2, (float)position.y() / Grid::SIZE * 60*2 , 0));
					wall->rotate(Grid::PLAYER_ROTATIONS[_players[i]->getDirection()],glm::vec3(0,0,1));
					wall->translate(glm::vec3(0,1.0f / Grid::SIZE * 60*5,0 ));
					scene.addObject(nameWall, wall);
				}
			}
		}
		else{
            if (_motorNoise){
                _motorNoise->setIsPaused(true);
            }
        }


		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        renderFrame();


        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
	scene.clear();
}

Application::~Application()
{
    for(int i=0;i<_players.size();i++)
    {
        Player *p=_players[i];
        delete p;
    }

    _players.clear();
	_engine->drop();

    glfwDestroyWindow(_window);
    glfwTerminate();
}
