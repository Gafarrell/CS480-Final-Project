#ifndef MESH_H
#define MESH_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"
#include "trig_functions.h"
#include "camera.h"

class Mesh
{
public:
    Mesh();
    Mesh(glm::vec3 pivot, const char* fname);
    Mesh(glm::vec3 pivot, const char* fname, const char* tname);

    ~Mesh();
    void Update(double dt);
    void Render(GLint posAttrib, GLint colAttrib);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);

    glm::mat4 GetModel();

    bool InitBuffers();
    bool loadModelFromFile(const char* path);

    void setTranslation(glm::vec3 translation) { this->shipPosition = translation; }

    void setSpeed(float speed) { this->speedScalar = speed; }

    void setDirection(glm::vec3 direction) { this->direction = direction; }
    void setRotateDeltas(double xDelta, double yDelta) { this->xMouseDelta = xDelta; this->yMouseDelta = yDelta; }
    
    void setCamera(Camera* camera) { this->m_camera = camera; }
    void setUniformScale(float scale) { this->uniformScale = scale; }

    bool hasTex;
    GLuint getTextureID() { return m_texture->getTextureID(); }



private:
    double xMouseDelta = 0, yMouseDelta = 0;
    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    GLuint VB;
    GLuint IB;

    Camera* m_camera;
    
    glm::vec3 shipPosition = glm::vec3(0, -0.25,-1);
    glm::vec3 direction = glm::vec3(0, 0, 0);

    glm::vec2 thirdPersonOffsets = glm::vec2(-200, 50);
    glm::vec3 firstPersonOffset = glm::vec3(0), firstPersonFocus = glm::vec3(0);

    float speedScalar = 1.f;
    float thirdPersonVerticalFocusOffset = 25.f;

    float uniformScale = 0.005f;
    float horizAngle = 0.f;
    float vertAngle = 0;

    Texture* m_texture;

    GLuint vao;


    void updateCamera();

};

#endif