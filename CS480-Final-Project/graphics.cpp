#include "graphics.h"

// Constructor
Graphics::Graphics()
{

}

//Destructor
Graphics::~Graphics()
{

}

//Initialize the graphics, setup shaders, setup planets
bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif

	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader("shaders\\genericVertShader.glsl", "shaders\\genericFragShader.glsl");

	if (m_shader->getProgram() == 0) {
		cout << "Failed to load generic shader." << endl;
		return false;
	}
	collectShPrLocs();


	skybox_shader = new Shader("shaders\\skyboxVertShader.glsl", "shaders\\skyboxFragShader.glsl");

	if (skybox_shader->getProgram() == 0) {
		cout << "Unable to load skybox shader." << endl;
		return false;
	}

	sun_shader = new Shader("shaders\\sunVertShader.txt", "shaders\\sunFragShader.txt");

	if (sun_shader->getProgram() == 0) {
		cout << "Unable to load sun shader." << endl;
		return false;
	}


	// Starship
	m_controller = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");
	m_controller->setCamera(m_camera);


	skyBox = new SkyBox("assets\\Highres-Cubemap", 200, 200);
	skyBox->setShader(skybox_shader);

	// The Sun
	m_sun = new Sphere(64, "assets\\2k_sun.jpg");
	m_sun->setAngle(vector<float>({ 5 }));
	m_sun->setOrbitalFunctions(std::vector<TrigFunction*>({ new None(), new None(), new None() }));
	m_sun->setOrbitDistance(vector<float>({ 0,0,0 }));
	m_sun->setRotationSpeed(vector<float>({ 0.15f }));
	m_sun->setScale(vector<float>({ 1,1,1 }));
	m_sun->setSpeed(vector<float>({ 2,2,2 }));
	//solarSystem.push_back(m_sun);

	// Mercury
	m_mercury = new Sphere(48, "assets\\Mercury.jpg");
	m_mercury->setAngle(vector<float>({ 2.5 }));
	m_mercury->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_mercury->setOrbitDistance(vector<float>({ 3.0f,3.0f,3.0f }));
	m_mercury->setRotationSpeed(vector<float>({ 0.06f }));
	m_mercury->setScale(vector<float>({ 0.1f,0.1f,0.1f }));
	m_mercury->setSpeed(vector<float>({ 0.35f, 0.35f, 0.35f }));
	solarSystem.push_back(m_mercury);

	// Venus
	m_venus = new Sphere(48, "assets\\Venus.jpg");
	m_venus->setAngle(vector<float>({ 3 }));
	m_venus->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_venus->setOrbitDistance(vector<float>({ 6.0f,6.0f,6.0f }));
	m_venus->setRotationSpeed(vector<float>({ 0.03f }));
	m_venus->setScale(vector<float>({ 0.25f,0.25f,0.25f }));
	m_venus->setSpeed(vector<float>({ 0.25f, 0.25f, 0.25f }));
	solarSystem.push_back(m_venus);


	// The Earth
	m_earth = new Sphere(48, "assets\\2k_earth_daymap.jpg");
	m_earth->setAngle(vector<float>({ 3 }));
	m_earth->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_earth->setOrbitDistance(vector<float>({ 9.0f,9.0f,9.0f }));
	m_earth->setRotationSpeed(vector<float>({ 0.35f }));
	m_earth->setScale(vector<float>({ 0.25f,0.25f,0.25f }));
	m_earth->setSpeed(vector<float>({ 0.15f, 0.15f, 0.15f }));
	solarSystem.push_back(m_earth);

	// The moon
	m_moon = new Sphere(48, "assets\\2k_moon.jpg");
	m_moon->setAngle(vector<float>({ 0.56 }));
	m_moon->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new Cos(), new Sin() }));
	m_moon->setOrbitDistance(vector<float>({ 1.,1.,1. }));
	m_moon->setRotationSpeed(vector<float>({ 0.5f }));
	m_moon->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
	m_moon->setSpeed(vector<float>({ .25f,.25f,.25f }));
	solarSystem.push_back(m_moon);

	// Mars
	m_mars = new Sphere(48, "assets\\Mars.jpg");
	m_mars->setAngle(vector<float>({ 3 }));
	m_mars->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_mars->setOrbitDistance(vector<float>({ 12.0f,12.0f,12.0f }));
	m_mars->setRotationSpeed(vector<float>({ 0.35f }));
	m_mars->setScale(vector<float>({ 0.18f,0.18f,0.18f }));
	m_mars->setSpeed(vector<float>({ 0.08f, 0.08f, 0.08f }));
	solarSystem.push_back(m_mars);

	// Ceres
	m_ceres = new Sphere(48, "assets\\Ceres.jpg");
	m_ceres->setAngle(vector<float>({ 3 }));
	m_ceres->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_ceres->setOrbitDistance(vector<float>({ 14.5f,14.5f,14.5f }));
	m_ceres->setRotationSpeed(vector<float>({ 0.35f }));
	m_ceres->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
	m_ceres->setSpeed(vector<float>({ 0.07f, 0.07f, 0.07f }));
	solarSystem.push_back(m_ceres);

	// Jupiter
	m_jupiter = new Sphere(48, "assets\\Jupiter.jpg");
	m_jupiter->setAngle(vector<float>({ 3 }));
	m_jupiter->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_jupiter->setOrbitDistance(vector<float>({ 17.0f,17.0f,17.0f }));
	m_jupiter->setRotationSpeed(vector<float>({ 0.9f }));
	m_jupiter->setScale(vector<float>({ 0.5f,0.5f,0.5f }));
	m_jupiter->setSpeed(vector<float>({ 0.06f, 0.06f, 0.06f }));
	solarSystem.push_back(m_jupiter);

	//Jupiter moons
		//Lo
		m_lo = new Sphere(48, "assets\\Lo.jpg");
		m_lo->setAngle(vector<float>({ 3 }));
		m_lo->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_lo->setOrbitDistance(vector<float>({ 1.,1.,1. }));
		m_lo->setRotationSpeed(vector<float>({ 0.3f }));
		m_lo->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_lo->setSpeed(vector<float>({ .25f,.25f,.25f }));
		solarSystem.push_back(m_lo);

		//Europa
		m_europa = new Sphere(48, "assets\\europa.jpg");
		m_europa->setAngle(vector<float>({ 3 }));
		m_europa->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_europa->setOrbitDistance(vector<float>({ 1.25,1.25,1.25 }));
		m_europa->setRotationSpeed(vector<float>({ 0.3f }));
		m_europa->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_europa->setSpeed(vector<float>({ .2f,.2f,.2f }));
		solarSystem.push_back(m_europa);

		//Ganymede
		m_ganymede = new Sphere(48, "assets\\ganymede.jfif");
		m_ganymede->setAngle(vector<float>({ 3 }));
		m_ganymede->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_ganymede->setOrbitDistance(vector<float>({ 1.37,1.37,1.37 }));
		m_ganymede->setRotationSpeed(vector<float>({ 0.3f }));
		m_ganymede->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_ganymede->setSpeed(vector<float>({ .15f,.15f,.15f }));
		solarSystem.push_back(m_ganymede);

		//Callisto
		m_callisto = new Sphere(48, "assets\\callisto.jpg");
		m_callisto->setAngle(vector<float>({ 3 }));
		m_callisto->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_callisto->setOrbitDistance(vector<float>({ 1.75,1.75,1.75 }));
		m_callisto->setRotationSpeed(vector<float>({ 0.3f }));
		m_callisto->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_callisto->setSpeed(vector<float>({ .1f,.1f,.1f }));
		solarSystem.push_back(m_callisto);

	// Saturn
	m_saturn = new Sphere(48, "assets\\Saturn.jpg");
	m_saturn->setAngle(vector<float>({ 3 }));
	m_saturn->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_saturn->setOrbitDistance(vector<float>({ 20.0f,20.0f,20.0f }));
	m_saturn->setRotationSpeed(vector<float>({ 0.82f }));
	m_saturn->setScale(vector<float>({ 0.4f,0.4f,0.4f }));
	m_saturn->setSpeed(vector<float>({ 0.05f, 0.05f, 0.05f }));
	solarSystem.push_back(m_saturn);

	//Saturn moons
		//Titan
		m_titan = new Sphere(48, "assets\\titan.jfif");
		m_titan->setAngle(vector<float>({ 3 }));
		m_titan->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_titan->setOrbitDistance(vector<float>({ 1.5,1.5,1.5 }));
		m_titan->setRotationSpeed(vector<float>({ 0.5f }));
		m_titan->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_titan->setSpeed(vector<float>({ .45f,.45f,.45f }));
		solarSystem.push_back(m_titan);

		//Iapteus
		m_iapteus = new Sphere(48, "assets\\iapteus.jfif");
		m_iapteus->setAngle(vector<float>({ 3 }));
		m_iapteus->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_iapteus->setOrbitDistance(vector<float>({ 1.75,1.75,1.75 }));
		m_iapteus->setRotationSpeed(vector<float>({ 0.3f }));
		m_iapteus->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_iapteus->setSpeed(vector<float>({ .25f,.25f,.25f }));
		solarSystem.push_back(m_iapteus);

	// Uranus
	m_uranus = new Sphere(48, "assets\\Uranus.jpg");
	m_uranus->setAngle(vector<float>({ 3 }));
	m_uranus->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_uranus->setOrbitDistance(vector<float>({ 23.0f,23.0f,23.0f }));
	m_uranus->setRotationSpeed(vector<float>({ 0.6f }));
	m_uranus->setScale(vector<float>({ 0.3f,0.3f,0.3f }));
	m_uranus->setSpeed(vector<float>({ 0.03f, 0.03f, 0.03f }));
	solarSystem.push_back(m_uranus);

	//Uranus moons
		//Oberon
		m_oberon = new Sphere(48, "assets\\oberon.jfif");
		m_oberon->setAngle(vector<float>({ 3 }));
		m_oberon->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_oberon->setOrbitDistance(vector<float>({ 1.5,1.5,1.5 }));
		m_oberon->setRotationSpeed(vector<float>({ 0.3f }));
		m_oberon->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_oberon->setSpeed(vector<float>({ .55f,.55f,.55f }));
		solarSystem.push_back(m_oberon);

		//Ariel
		m_ariel = new Sphere(48, "assets\\ariel.jfif");
		m_ariel->setAngle(vector<float>({ 3 }));
		m_ariel->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_ariel->setOrbitDistance(vector<float>({ 1.75,1.75,1.75 }));
		m_ariel->setRotationSpeed(vector<float>({ 0.5f }));
		m_ariel->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_ariel->setSpeed(vector<float>({ .35f,.35f,.35f }));
		solarSystem.push_back(m_ariel);

	// Neptune
	m_neptune = new Sphere(48, "assets\\Neptune.jpg");
	m_neptune->setAngle(vector<float>({ 3 }));
	m_neptune->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_neptune->setOrbitDistance(vector<float>({ 26.0f,26.0f,26.0f }));
	m_neptune->setRotationSpeed(vector<float>({ 0.5f }));
	m_neptune->setScale(vector<float>({ 0.3f,0.3f,0.3f }));
	m_neptune->setSpeed(vector<float>({ 0.02f, 0.02f, 0.02f }));
	solarSystem.push_back(m_neptune);

	//Nepture moon
		//Triton
		m_triton = new Sphere(48, "assets\\triton.jfif");
		m_triton->setAngle(vector<float>({ 3 }));
		m_triton->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
		m_triton->setOrbitDistance(vector<float>({ 1.75,1.75,1.75 }));
		m_triton->setRotationSpeed(vector<float>({ 0.3f }));
		m_triton->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
		m_triton->setSpeed(vector<float>({ .25f,.25f,.25f }));
		solarSystem.push_back(m_triton);

	// Pluto
	m_pluto = new Sphere(48, "assets\\Mercury.jpg");
	m_pluto->setAngle(vector<float>({ 2 }));
	m_pluto->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_pluto->setOrbitDistance(vector<float>({ 28.8f,28.8f,28.8f }));
	m_pluto->setRotationSpeed(vector<float>({ 0.35f }));
	m_pluto->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
	m_pluto->setSpeed(vector<float>({ 0.01f, 0.01f, 0.01f }));
	solarSystem.push_back(m_pluto);

	// Haumea
	m_haumea = new Sphere(48, "assets\\Haumea.jpg");
	m_haumea->setAngle(vector<float>({ 3 }));
	m_haumea->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_haumea->setOrbitDistance(vector<float>({ 29.2f,29.2f,29.2f }));
	m_haumea->setRotationSpeed(vector<float>({ 0.35f }));
	m_haumea->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
	m_haumea->setSpeed(vector<float>({ 0.008f, 0.008f, 0.008f }));
	solarSystem.push_back(m_haumea);

	// Eris
	m_eris = new Sphere(48, "assets\\Eris.jpg");
	m_eris->setAngle(vector<float>({ 3 }));
	m_eris->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_eris->setOrbitDistance(vector<float>({ 30.0f,30.0f,30.0f }));
	m_eris->setRotationSpeed(vector<float>({ 0.35f }));
	m_eris->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
	m_eris->setSpeed(vector<float>({ 0.006f, 0.006f, 0.006f }));
	solarSystem.push_back(m_eris);

	// Haley's Comet
	m_halcomet = new Sphere(48, "assets\\Uranus.jpg");
	m_halcomet->setAngle(vector<float>({ 5 }));
	m_halcomet->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new Cos(), new Cos() }));
	m_halcomet->setOrbitDistance(vector<float>({ 30.0f,3.5f,3.0f }));
	m_halcomet->setRotationSpeed(vector<float>({ 0.35f }));
	m_halcomet->setScale(vector<float>({ 0.05f,0.05f,0.05f }));
	m_halcomet->setSpeed(vector<float>({ 0.15, 0.15f, 0.15f }));

	solarSystem.push_back(m_halcomet);

	//Asteroid Belts
	m_asteroids = new AsteroidInstancer("shaders\\instanceVertShader.glsl", "shaders\\instanceFragShader.glsl", "assets\\asteroid.obj", "assets\\asteroid.jpg", 700, 15.0f);
	m_outer_asteroids = new AsteroidInstancer("shaders\\instanceVertShader.glsl", "shaders\\instanceFragShader.glsl", "assets\\asteroid.obj", "assets\\asteroid.jpg", 1000, 29.0f);
	
	//Rings
	m_saturn_ring = new Ring(.60,1,48,"assets\\Saturn.jpg");
	m_saturn_ring->setAngle(vector<float>({ 3 }));
	m_saturn_ring->setOrbitalFunctions(std::vector<TrigFunction*>({ new Sin(), new None(), new Cos() }));
	m_saturn_ring->setOrbitDistance(vector<float>({ 20.0f,20.0f,20.0f }));
	m_saturn_ring->setRotationSpeed(vector<float>({ 0.35f }));
	m_saturn_ring->setScale(vector<float>({ 1.5f,0.f,1.5f }));
	m_saturn_ring->setSpeed(vector<float>({ 0.05f, 0.05f, 0.05f }));

	//enable depth testing
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

//Update the position of all objects
void Graphics::HierarchicalUpdate2(double dt) {


	totalTime += dt;

	// Update your animation for the solar system here.
	m_controller->Update(dt);
	m_camera->update(dt);

	modelStack.push(glm::translate(glm::mat4(1), glm::vec3(0)));

	glm::mat4 tmat, rmat, smat;

	//Sun transform
	ComputeTransforms(totalTime, m_sun->getOrbitalFunctions(), m_sun->getSpeed(), m_sun->getDistance(), m_sun->getRotationSpeed(), glm::vec3(0, 1, 0), m_sun->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_sun->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_sun->Update(modelStack.top());

	modelStack.pop();

	//Mercury transform
	ComputeTransforms(totalTime, m_mercury->getOrbitalFunctions(), m_mercury->getSpeed(), m_mercury->getDistance(), m_mercury->getRotationSpeed(), glm::vec3(0, 1, 0), m_mercury->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_mercury->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_mercury->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Venus transform
	ComputeTransforms(totalTime, m_venus->getOrbitalFunctions(), m_venus->getSpeed(), m_venus->getDistance(), m_venus->getRotationSpeed(), glm::vec3(0, 1, 0), m_venus->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_venus->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_venus->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Earth transform
	ComputeTransforms(totalTime, m_earth->getOrbitalFunctions(), m_earth->getSpeed(), m_earth->getDistance(), m_earth->getRotationSpeed(), glm::vec3(0, 1, 0), m_earth->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_earth->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_earth->Update(modelStack.top());

	modelStack.pop();

	//Moon transform
	ComputeTransforms(totalTime, m_moon->getOrbitalFunctions(), m_moon->getSpeed(), m_moon->getDistance(), m_moon->getRotationSpeed(), glm::vec3(0, 1, 0), m_moon->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_moon->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_moon->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();
	modelStack.pop();

	//Mars transform
	ComputeTransforms(totalTime, m_mars->getOrbitalFunctions(), m_mars->getSpeed(), m_mars->getDistance(), m_mars->getRotationSpeed(), glm::vec3(0, 1, 0), m_mars->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_mars->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_mars->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Ceres transform
	ComputeTransforms(totalTime, m_ceres->getOrbitalFunctions(), m_ceres->getSpeed(), m_ceres->getDistance(), m_ceres->getRotationSpeed(), glm::vec3(0, 1, 0), m_ceres->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_ceres->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_ceres->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Jupiter transform
	ComputeTransforms(totalTime, m_jupiter->getOrbitalFunctions(), m_jupiter->getSpeed(), m_jupiter->getDistance(), m_jupiter->getRotationSpeed(), glm::vec3(0, 1, 0), m_jupiter->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_jupiter->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_jupiter->Update(modelStack.top());

	modelStack.pop();

	//Lo transform
	ComputeTransforms(totalTime, m_lo->getOrbitalFunctions(), m_lo->getSpeed(), m_lo->getDistance(), m_lo->getRotationSpeed(), glm::vec3(0, 1, 0), m_lo->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_lo->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_lo->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Europa transform
	ComputeTransforms(totalTime, m_europa->getOrbitalFunctions(), m_europa->getSpeed(), m_europa->getDistance(), m_europa->getRotationSpeed(), glm::vec3(0, 1, 0), m_europa->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_europa->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_europa->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Ganymede transform
	ComputeTransforms(totalTime, m_ganymede->getOrbitalFunctions(), m_ganymede->getSpeed(), m_ganymede->getDistance(), m_ganymede->getRotationSpeed(), glm::vec3(0, 1, 0), m_ganymede->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_ganymede->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_ganymede->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Callisto transform
	ComputeTransforms(totalTime, m_callisto->getOrbitalFunctions(), m_callisto->getSpeed(), m_callisto->getDistance(), m_callisto->getRotationSpeed(), glm::vec3(0, 1, 0), m_callisto->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_callisto->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_callisto->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();
	modelStack.pop();

	//Saturn transform
	ComputeTransforms(totalTime, m_saturn->getOrbitalFunctions(), m_saturn->getSpeed(), m_saturn->getDistance(), m_saturn->getRotationSpeed(), glm::vec3(0, 1, 0), m_saturn->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_saturn->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_saturn->Update(modelStack.top());

	modelStack.pop();

	//Titan transform
	ComputeTransforms(totalTime, m_titan->getOrbitalFunctions(), m_titan->getSpeed(), m_titan->getDistance(), m_titan->getRotationSpeed(), glm::vec3(0, 1, 0), m_titan->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_titan->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_titan->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Iapteus transform
	ComputeTransforms(totalTime, m_iapteus->getOrbitalFunctions(), m_iapteus->getSpeed(), m_iapteus->getDistance(), m_iapteus->getRotationSpeed(), glm::vec3(0, 1, 0), m_iapteus->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_iapteus->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_iapteus->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();
	modelStack.pop();

	//Uranus transform
	ComputeTransforms(totalTime, m_uranus->getOrbitalFunctions(), m_uranus->getSpeed(), m_uranus->getDistance(), m_uranus->getRotationSpeed(), glm::vec3(0, 1, 0), m_uranus->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_uranus->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_uranus->Update(modelStack.top());

	modelStack.pop();

	//Ariel transform
	ComputeTransforms(totalTime, m_ariel->getOrbitalFunctions(), m_ariel->getSpeed(), m_ariel->getDistance(), m_ariel->getRotationSpeed(), glm::vec3(0, 1, 0), m_ariel->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_ariel->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_ariel->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Oberon transform
	ComputeTransforms(totalTime, m_oberon->getOrbitalFunctions(), m_oberon->getSpeed(), m_oberon->getDistance(), m_oberon->getRotationSpeed(), glm::vec3(0, 1, 0), m_oberon->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_oberon->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_oberon->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();
	modelStack.pop();

	//Nepture transform
	ComputeTransforms(totalTime, m_neptune->getOrbitalFunctions(), m_neptune->getSpeed(), m_neptune->getDistance(), m_neptune->getRotationSpeed(), glm::vec3(0, 1, 0), m_neptune->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_neptune->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_neptune->Update(modelStack.top());

	modelStack.pop();

	//Triton transform
	ComputeTransforms(totalTime, m_triton->getOrbitalFunctions(), m_triton->getSpeed(), m_triton->getDistance(), m_triton->getRotationSpeed(), glm::vec3(0, 1, 0), m_triton->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_triton->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat;
	modelStack.top() *= smat;

	m_triton->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();
	modelStack.pop();

	//Pluto transform
	ComputeTransforms(totalTime, m_pluto->getOrbitalFunctions(), m_pluto->getSpeed(), m_pluto->getDistance(), m_pluto->getRotationSpeed(), glm::vec3(0, 1, 0), m_pluto->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_pluto->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_pluto->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Huamea transform
	ComputeTransforms(totalTime, m_haumea->getOrbitalFunctions(), m_haumea->getSpeed(), m_haumea->getDistance(), m_haumea->getRotationSpeed(), glm::vec3(0, 1, 0), m_haumea->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_haumea->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_haumea->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Eris transform
	ComputeTransforms(totalTime, m_eris->getOrbitalFunctions(), m_eris->getSpeed(), m_eris->getDistance(), m_eris->getRotationSpeed(), glm::vec3(0, 1, 0), m_eris->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_eris->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_eris->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Halley's Comet transform
	ComputeTransforms(totalTime, m_halcomet->getOrbitalFunctions(), m_halcomet->getSpeed(), m_halcomet->getDistance(), m_halcomet->getRotationSpeed(), glm::vec3(0, 1, 0), m_halcomet->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	m_halcomet->UpdateSpecModel(modelStack.top());
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_halcomet->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Saturn ring transform
	ComputeTransforms(totalTime, m_saturn_ring->getOrbitalFunctions(), m_saturn_ring->getSpeed(), m_saturn_ring->getDistance(), m_saturn_ring->getRotationSpeed(), glm::vec3(0, 1, 0), m_saturn_ring->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	modelStack.top() *= rmat * smat;

	m_saturn_ring->Update(modelStack.top());

	modelStack.pop();
	modelStack.pop();

	//Asteroid Belts
	m_asteroids->Update(dt);
	m_outer_asteroids->Update(dt);

	while (!modelStack.empty()) modelStack.pop();
}

//Compute the transform of the objects
void Graphics::ComputeTransforms(double dt, std::vector<TrigFunction*> orbitFunctions, std::vector<float> speed, std::vector<float> dist,
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale,
	glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) 
{
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(orbitFunctions[0]->execute(speed[0] * dt) * dist[0], orbitFunctions[1]->execute(speed[1] * dt) * dist[1], orbitFunctions[2]->execute(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

//Render all objects in the scene
void Graphics::Render()
{
	////clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render skybox
	skybox_shader->Enable();

	skyBox->Render(m_camera->GetView(), m_camera->GetProjection());

	//Render asteroids
	m_asteroids->enableShader();

	m_asteroids->Render(totalTime, m_camera->GetView(), m_camera->GetProjection());

	m_outer_asteroids->enableShader();
	
	m_outer_asteroids->Render(totalTime, m_camera->GetView(), m_camera->GetProjection());

	//get sun color
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//get sun pos
	glm::vec3 lightPos = glm::vec3(0.0f,0.0f,0.0f);

	//Render the Sun
	sun_shader->Enable();
	glUniformMatrix4fv(sun_shader->GetUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(sun_shader->GetUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));


	if (m_sun != NULL) {
		glUniformMatrix4fv(sun_shader->GetUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
		if (m_sun->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sun->Render(m_positionAttrib, m_colorAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Start the generic shader program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));


	// Render starship
	if (m_controller != NULL && !m_controller->isSpectateMode()) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_controller->GetModel()));
		if (m_controller->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_controller->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}

			glUniformMatrix3fv(m_shader->GetUniformLocation("normMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetProjection() * m_controller->GetModel())))));
			glUniformMatrix4fv(m_shader->GetUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_controller->GetModel()));
			glUniform4f(m_shader->GetUniformLocation("lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(m_shader->GetUniformLocation("lightPos"), lightPos.x, lightPos.y, lightPos.z);

			glUniform1i(sampler, 0);
			m_controller->Render(m_positionAttrib, m_colorAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);

		}
	}

	// Rendering algorithm for solar system.
	for (int i = 0; i < solarSystem.size(); i++) {
		Sphere* object = solarSystem[i];

		if (object != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(object->GetModel()));
			if (object->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, object->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}

				glUniformMatrix3fv(m_shader->GetUniformLocation("normMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetProjection() * object->GetModel())))));
				glUniformMatrix4fv(m_shader->GetUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(object->GetModel()));
				glUniform4f(m_shader->GetUniformLocation("lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
				glUniform3f(m_shader->GetUniformLocation("lightPos"), lightPos.x, lightPos.y, lightPos.z);

				glUniform1i(sampler, 0);
				object->Render(m_positionAttrib, m_colorAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
				
			}
		}
	}


	if (m_saturn_ring != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn_ring->GetModel()));
		if (m_saturn_ring->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn_ring->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}

			glUniformMatrix3fv(m_shader->GetUniformLocation("normMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_camera->GetProjection() * m_saturn_ring->GetModel())))));
			glUniformMatrix4fv(m_shader->GetUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_saturn_ring->GetModel()));
			glUniform4f(m_shader->GetUniformLocation("lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(m_shader->GetUniformLocation("lightPos"), lightPos.x, lightPos.y, lightPos.z);

			glUniform1i(sampler, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			m_saturn_ring->Render(m_positionAttrib, m_colorAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);

		}
	}


	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}

//Get shader variables locations
bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the normal vertex attribute
	m_normalAttrib = m_shader->GetAttribLocation("v_normal");
	if (m_normalAttrib == -1)
	{
		printf("v_normal attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

//Print OpenGL errors
std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

//Spectator mode 
void Graphics::spectate() {
	if (m_controller->isSpectateMode()) return;
	Sphere* spectate = m_sun;
	glm::vec3 ship = m_controller->getPosition();
	glm::vec3 smallest = ship;
	
	for (int i = 1; i < solarSystem.size(); i++) {
		glm::mat4 planetModel = solarSystem[i]->GetModel();
		glm::vec3 planetPosition = glm::vec3(planetModel[3][0], planetModel[3][1], planetModel[3][2]);

		glm::vec3 distance = planetPosition - ship;

		if (glm::length(distance) < glm::length(smallest)) {
			spectate = solarSystem[i];
			smallest = distance;
		}
		std::cout << "Distance: " << glm::length(distance) << endl;
	}

	m_controller->spectate(spectate, 1.5f);
}

