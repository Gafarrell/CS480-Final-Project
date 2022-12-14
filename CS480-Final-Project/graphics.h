#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "skybox.h"
#include "asteroid_instancer.h"
#include "Ring.h"

#define numVBOs 2;
#define numIBs 2;

#define SPECTATE_DISTANCE 1.5f


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void HierarchicalUpdate2(double dt);
    void Render();
    void spectate();

    Mesh* getController() { return m_controller; }

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<TrigFunction*> orbitFunctions, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera* m_camera;
    Shader* m_shader;
    Shader* skybox_shader;
    Shader* instance_shader;
    Shader* sun_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_normalAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;

    SkyBox* skyBox;

    vector<Sphere*> solarSystem;

    Sphere* m_sun;
    Sphere* m_mercury;
    Sphere* m_venus;
    Sphere* m_earth;
    Sphere* m_moon;
    Sphere* m_mars;
    Sphere* m_ceres;
    Sphere* m_jupiter;
    Sphere* m_saturn;
    Sphere* m_uranus;
    Sphere* m_neptune;
    Sphere* m_pluto;
    Sphere* m_haumea;
    Sphere* m_eris;
    Sphere* m_halcomet;
    Sphere* m_lo;
    Sphere* m_europa;
    Sphere* m_ganymede;
    Sphere* m_callisto;
    Sphere* m_titan;
    Sphere* m_iapteus;
    Sphere* m_ariel;
    Sphere* m_oberon;
    Sphere* m_triton;

    Ring* m_saturn_ring;

    AsteroidInstancer* m_asteroids;
    AsteroidInstancer* m_outer_asteroids;

    int renderingProgram, renderingProgramCubeMap;
    int skyBoxTexture;

    Mesh* m_controller;

    long double totalTime = 0;
};

#endif /* GRAPHICS_H */
