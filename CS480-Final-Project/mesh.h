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
    void addSpeed(glm::vec3 speed) { this->speed = speed; }
    void setSpeed(glm::vec3 speed) { this->speed = speed; }
    void setCamera(Camera* camera) { this->m_camera = camera; }

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
    glm::vec3 shipPosition = glm::vec3(0, -10, -16);
    glm::vec3 thirdPersonPositionOffset = glm::vec3(0, -1, -1), thirdPersonFocusOffset = glm::vec3(0,0.5f,0);
    glm::vec3 speed;

    float uniformScale = 0.05f;
    float horizAngle = 0;
    float vertAngle = -3.1415 / 4;

    Texture* m_texture;

    GLuint vao;


    void updateCamera();

};

#endif