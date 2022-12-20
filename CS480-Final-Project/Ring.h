#include <cmath>
#include "object.h"
#include "Texture.h"
#include "trig_functions.h"
class Ring
{
private:
	int numVertices;
	int numIndices;
	int prec;
	float inner;
	float outer;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> sTangents;
	std::vector<glm::vec3> tTangents;
	std::vector<Vertex>Vertices;
	std::vector<unsigned int> Indices;
	glm::mat4 model;
	glm::vec3 pivotLocation;

	
	void init();
	float toRadians(float degrees);

	GLuint VB;
	GLuint IB;
	GLuint TB;
	GLuint NB;
	Texture* m_texture;

	std::vector<float> angle;
	std::vector<float> speed;
	std::vector<float> scale;
	std::vector<float> distance;
	std::vector<float> rotationSpeed;
	std::vector<TrigFunction*> orbitFunctions;

	GLuint vao;

public:

	void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint normalAttribLoc, GLint tcAttribLoc, GLint hasTex);


	Ring();
	Ring(float inner, float outer, int prec, const char* fname);
	void setupVertices();
	void setupBuffers();
	void setupModelMatrix(glm::vec3 pivot, float angle, float scale);
	void Update(glm::mat4 matModel);
	int getNumVertices();
	int getNumIndices();
	std::vector<int> getIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getStangents();
	std::vector<glm::vec3> getTtangents();
	glm::mat4 GetModel() { return model; }
	GLuint getTextureID() { return m_texture->getTextureID(); }
	std::vector<float> getAngle() { return angle; }
	std::vector<float> getSpeed() { return speed; }
	std::vector<float> getScale() { return scale; }
	std::vector<float> getDistance() { return distance; }
	std::vector<float> getRotationSpeed() { return rotationSpeed; }
	std::vector<TrigFunction*> getOrbitalFunctions() { return orbitFunctions; }
	void setScale(std::vector<float> scale) { this->scale = scale; }
	void setAngle(std::vector<float> angle) { this->angle = angle; }
	void setSpeed(std::vector<float> speed) { this->speed = speed; }
	void setOrbitDistance(std::vector<float> distance) { this->distance = distance; }
	void setRotationSpeed(std::vector<float> rotationSpeed) { this->rotationSpeed = rotationSpeed; }
	void setOrbitalFunctions(std::vector<TrigFunction*> orbitalFunctions) { this->orbitFunctions = orbitalFunctions; }

	bool hasTex;
};