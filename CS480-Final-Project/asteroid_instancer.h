#pragma once
#include "graphics_headers.h"
#include "shader.h"
#include "mesh.h"

class AsteroidInstancer
{
public:
	AsteroidInstancer(const char* vShader, const char* fShader, const char* fname, const char* tname, int asteroidCount, float radius);
	
	void Render(double totalTime, glm::mat4 cameraView, glm::mat4 cameraProjection);

	bool loadModelFromFile(const char* path);
	void enableShader() { instance_shader->Enable(); }
	void Update(double totalTime);

	void setRadius(float radius) { this->radius = radius; }
	void setOffset(float offset) { this->offset = offset; }

private:
	int asteroidCount;
	glm::mat4 *models;
	glm::mat4 originPoint;
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	void getShaderLocs();
	void generateModels();
	bool InitBuffers();

	Texture* m_texture;

	Shader *instance_shader;
	GLint m_view, m_proj, m_model, timeFactor;
	GLuint VB, IB, MB, vao;
	GLint posAttrib, colAttrib, tcAttrib, normAttrib;

	float radius = 15.0f;
	float offset = 0.5f;
	float rotationSpeed = 0.005f;
};

