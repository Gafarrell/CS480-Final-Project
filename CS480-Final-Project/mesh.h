#ifndef MESH_H
#define MESH_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"
#include "trig_functions.h"

class Mesh
{
public:
    Mesh();
    Mesh(glm::vec3 pivot, const char* fname);
    Mesh(glm::vec3 pivot, const char* fname, const char* tname);

    ~Mesh();
    void Update(glm::mat4 model);
    void Render(GLint posAttrib, GLint colAttrib);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);

    glm::mat4 GetModel();

    bool InitBuffers();
    bool loadModelFromFile(const char* path);

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
    GLuint getTextureID() { return m_texture->getTextureID(); }



private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    Texture* m_texture;

    GLuint vao;

    std::vector<float> angle;
    std::vector<float> speed;
    std::vector<float> scale;
    std::vector<float> distance;
    std::vector<float> rotationSpeed;
    std::vector<TrigFunction*> orbitFunctions;
};

#endif