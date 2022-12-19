#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  //view = glm::lookAt( glm::vec3(x, y, z), //Eye Position
  //                    glm::vec3(0.0, 0.0, 0.0), //Focus point
  //                    glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
	windowWidth = w;
	windowHeight = h;
  view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

  projection = glm::perspective( glm::radians(40.f), //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::update(double dt)
{
	view = glm::lookAt(
		cameraPos, // Move camera
		cameraFront, // Adjust focused coordinate
		cameraUp
	);
}

void Camera::setPerspective(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp) {
	this->cameraPos = cameraPosition;
	this->cameraFront = cameraFront;
	this->cameraUp = cameraUp;
}

void Camera::addSpeed(glm::vec3 s) {
	speed += s;
	if (speed != glm::vec3(0))
		speed = glm::normalize(speed);
}
