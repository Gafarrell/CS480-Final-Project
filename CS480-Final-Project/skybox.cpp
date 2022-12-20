#include "skybox.h"

//Consturctor
SkyBox::SkyBox(const char* dirName, int w, int h) {
	sectionW = w / 4;
	sectionH = h / 3;

	cubeMap = loadCubemap(dirName);
	setupVerticies();
	InitBuffers();
}

//Load the cube map
unsigned int SkyBox::loadCubemap(const char* dirName) {
	return Utils::loadCubeMap(dirName);
}

//Setup the vertices and indices
void SkyBox::setupVerticies() {
	Vertices = {
  {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.,0.}},
  {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.,0.}},
  {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.,0.}},
  {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.,0.}},
  {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.,0.}},
  {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.,0.}},
  {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.,0.}},
  {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.,0.}}
	};

	Indices = {
	  2, 3, 4,
	  8, 7, 6,
	  1, 5, 6,
	  2, 6, 7,
	  7, 8, 4,
	  1, 4, 8,
	  1, 2, 4,
	  5, 8, 6,
	  2, 1, 6,
	  3, 2, 7,
	  3, 7, 4,
	  5, 1, 8
	};

	// The index works at a 0th index
	for (unsigned int i = 0; i < Indices.size(); i++)
	{
		Indices[i] = Indices[i] - 1;
	}
}

//Initialize the shader
bool SkyBox::InitShader() {
	m_position = shader->GetUniformLocation("v_matrix");
	m_projection = shader->GetUniformLocation("p_matrix");
	return true;
}


//Setup the buffers for vertices
bool SkyBox::InitBuffers() 
{

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);

	return true;
}

//Render the skybox
void SkyBox::Render(glm::mat4 vMat, glm::mat4 pMat)
{
	vMat *= glm::translate(glm::mat4(1), -glm::vec3(vMat[3][0], vMat[3][1], vMat[3][2]));
	glUniformMatrix4fv(m_position, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(m_projection, 1, GL_FALSE, glm::value_ptr(pMat));

	GLint positionLoc = shader->GetAttribLocation("position");
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLoc);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
}