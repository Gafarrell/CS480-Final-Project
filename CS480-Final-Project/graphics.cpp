#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

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
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	// Starship
	m_controller = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");
	m_controller->setCamera(m_camera);

	// The Sun
	m_sphere = new Sphere(64, "assets\\2k_sun.jpg");
	m_sphere->setAngle({ 5 });
	m_sphere->setOrbitalFunctions({ new None(), new None(), new None() });
	m_sphere->setOrbitDistance({ 0, 0, 0 });
	m_sphere->setRotationSpeed({ 0.15f });
	m_sphere->setScale({ 1, 1, 1 });
	m_sphere->setSpeed({ 2,2,2 });
	solarSystem.push_back(m_sphere);

	// The Earth
	m_sphere2 = new Sphere(48, "assets\\2k_earth_daymap.jpg");
	m_sphere2->setAngle({ 3 });
	m_sphere2->setOrbitalFunctions({ new Sin(), new None(), new Cos() });
	m_sphere2->setOrbitDistance({ 2.5f,2.5f,2.5f });
	m_sphere2->setRotationSpeed({ 0.35f });
	m_sphere2->setScale({ 0.5f,0.5f,0.5f });
	m_sphere2->setSpeed({0.15f, 0.15f, 0.15f});
	solarSystem.push_back(m_sphere2);

	// The moon
	m_sphere3 = new Sphere(48, "assets\\2k_moon.jpg");
	m_sphere3->setAngle({ 0.56 });
	m_sphere3->setOrbitalFunctions({ new Sin(), new Cos(), new Sin() });
	m_sphere3->setOrbitDistance({ 1.65,1.65,1.65 });
	m_sphere3->setRotationSpeed({ 0.5f });
	m_sphere3->setScale({ 0.15f,0.15f,0.15f });
	m_sphere3->setSpeed({ .25f,.25f,.25f });
	solarSystem.push_back(m_sphere3);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {

	totalTime += dt;

	// Update your animation for the solar system here.
	m_controller->Update(dt);
	m_camera->update(dt);

	modelStack.push(glm::translate(glm::mat4(1), glm::vec3(0)));

	glm::mat4 tmat, rmat, smat;

	ComputeTransforms(totalTime, m_sphere->getOrbitalFunctions(), m_sphere->getSpeed(), m_sphere->getDistance(), m_sphere->getRotationSpeed(), glm::vec3(0, 1, 0), m_sphere->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_sphere->Update(modelStack.top());

	modelStack.pop();

	ComputeTransforms(totalTime, m_sphere2->getOrbitalFunctions(), m_sphere2->getSpeed(), m_sphere2->getDistance(), m_sphere2->getRotationSpeed(), glm::vec3(0, 1, 0), m_sphere2->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat * smat;

	m_sphere2->Update(modelStack.top());

	modelStack.pop();


	ComputeTransforms(totalTime, m_sphere3->getOrbitalFunctions(), m_sphere3->getSpeed(), m_sphere3->getDistance(), m_sphere3->getRotationSpeed(), glm::vec3(0, 1, 0), m_sphere3->getScale(), tmat, rmat, smat);
	modelStack.push(modelStack.top());

	modelStack.top() *= tmat;
	modelStack.push(modelStack.top());
	modelStack.top() *= rmat; 
	modelStack.top() *= smat;

	m_sphere3->Update(modelStack.top());

	while (!modelStack.empty()) modelStack.pop();
}


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

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	//// Render starship
	//if (m_mesh != NULL) {
	//	glUniform1i(m_hasTexture, false);
	//	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
	//	if (m_mesh->hasTex) {
	//		glActiveTexture(GL_TEXTURE0);
	//		glBindTexture(GL_TEXTURE_2D, m_mesh->getTextureID());
	//		GLuint sampler = m_shader->GetUniformLocation("sp");
	//		if (sampler == INVALID_UNIFORM_LOCATION)
	//		{
	//			printf("Sampler Not found not found\n");
	//		}
	//		glUniform1i(sampler, 0);
	//		m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
	//	}
	//}

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
				glUniform1i(sampler, 0);
				object->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
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

