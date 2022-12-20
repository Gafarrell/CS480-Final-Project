#ifndef MESH_H
#define MESH_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"
#include "trig_functions.h"
#include "camera.h"
#include "sphere.h"

class Mesh
{
public:
    Mesh();
    Mesh(glm::vec3 pivot, const char* fname);
    Mesh(glm::vec3 pivot, const char* fname, const char* tname);

    ~Mesh();
    void Update(double dt);
    void Render(GLint posAttrib, GLint colAttrib);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint normalAttribLoc, GLint tcAttribLoc, GLint hasTex);
    void spectate(Sphere* toSpectate, float spectateDistance);
    void explore();

    glm::mat4 GetModel();

    bool InitBuffers();
    bool loadModelFromFile(const char* path);

    void setTranslation(glm::vec3 translation) { this->shipPosition = translation; }

    void addForwardSpeed(float speed) { this->forwardSpeed = spectateMode ? speed*10 : forwardSpeed + speed; }
    void addHorizontalSpeed(float speed) { this->horizontalSpeed = spectateMode ? speed*10 : horizontalSpeed + speed; }
    void brakes() { this->forwardSpeed = this->horizontalSpeed = 0; }

    void setDirection(glm::vec3 direction) { this->direction = direction; }
    void setRotateDeltas(double xDelta, double yDelta) { this->xMouseDelta = xDelta; this->yMouseDelta = yDelta; m_camera->setRotateDelta(xDelta, yDelta); }
    
    void setCamera(Camera* camera) { this->m_camera = camera; }
    void setUniformScale(float scale) { this->uniformScale = scale; }
    void addZoom(float zoom) { if (spectateMode) m_camera->addZoom(zoom); }
    void resetSpectatorView();

    glm::vec3 getPosition() { return shipPosition;}

    bool isSpectateMode() { return spectateMode; };

    bool hasTex;
    GLuint getTextureID() { return m_texture->getTextureID(); }

private:
    double xMouseDelta = 0, yMouseDelta = 0;
    glm::vec3 pivotLocation;
    glm::mat4 model;
    glm::mat4 spectatorModel;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    GLuint VB;
    GLuint IB;
    GLuint NB;
    GLuint TB;

    Camera* m_camera;
    Sphere* toSpectate;
    float spectateDistance = 0.005f;
    
    glm::vec3 shipPosition = glm::vec3(0, -0.25,-1);
    glm::vec3 direction = glm::vec3(0, 0, 0);

    glm::vec2 thirdPersonOffsets = glm::vec2(-2.5f, 1);
    glm::vec2 firstPersonOffsets = glm::vec2(-3, -0.000015f );

    float forwardSpeed = 0.0f, horizontalSpeed = 0.0f;
    float thirdPersonVerticalFocusOffset = 0.5f;

    float uniformScale = 0.005f;
    float horizAngle = 0.f;
    float vertAngle = 0;
    bool spectateMode = false;

    Texture* m_texture;

    GLuint vao;


    void updateCamera();

};

#endif