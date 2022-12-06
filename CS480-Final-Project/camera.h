#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    glm::vec3 getPosition() { return cameraPos; }
    void update(double dt);

    void setSpeed(glm::vec3 s) { speed = s; }
    void addSpeed(glm::vec3 s);
    void addZoom(float zoom) { this->zoom += zoom; }

    void setZoom(double zoom) { this->zoom = zoom; }

    void setForwardSpeed(float speed) { forwardSpeed = speed; }
    void setHorizontalSpeed(float speed) { horizSpeed = speed; }
    void setVertSpeed(float speed) { vertSpeed = speed; }

    void setRotateDelta(float xd, float yd) { xDelta = xd; yDelta = yd; }
    void setRotationSpeed(float rotationSpeed) { this->rotationSpeed = rotationSpeed; }

  
  private:
    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;
    
    int windowWidth, windowHeight;

    glm::vec3 speed = glm::vec3(1);
    glm::vec3 cameraFront = glm::vec3(), cameraPos = glm::vec3(x,y,z), cameraUp = glm::vec3(0, 1, 0);
    double zoom = 0;

    float horizAngle = 0;
    float vertAngle = -3.1415 / 4;

    float rotationSpeed = 0.05f, xDelta = 0, yDelta = 0;

    float forwardSpeed = 0, horizSpeed = 0, vertSpeed = 0;
};

#endif /* CAMERA_H */
