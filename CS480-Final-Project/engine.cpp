

#include "engine.h"
#include "glm/ext.hpp"

Engine* engine = nullptr;
static void windowFocusCallback(GLFWwindow* window, int focused);
static void scroll_wheel_callback(GLFWwindow* window, double scrollX, double scrollY);

Engine::Engine(const char* name, int width, int height)
{
    if (engine == nullptr) engine = this;
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  if(!m_window->Initialize())
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }


  glfwSetWindowFocusCallback(m_window->getWindow(), windowFocusCallback);
  glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwSetCursorPos(m_window->getWindow(), m_WINDOW_WIDTH / 2, m_WINDOW_HEIGHT / 2);
  glfwSetScrollCallback(m_window->getWindow(), scroll_wheel_callback);

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  double lastTimeStamp = 0;

  while (!glfwWindowShouldClose(m_window->getWindow()))
  {
      double dt = glfwGetTime() - lastTimeStamp;
      lastTimeStamp = glfwGetTime();

      ProcessInput();
      Display(m_window->getWindow(), dt);
      glfwPollEvents();
  }
  m_running = false;

}

void Engine::adjustZoom(float zoom) {
    if (m_focused)
        m_graphics->getCamera()->addZoom(zoom);
}

void Engine::ProcessInput()
{
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);


    // Update camera animation here.
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) != GLFW_PRESS && glfwGetKey(m_window->getWindow(), GLFW_KEY_S) != GLFW_PRESS)
        m_graphics->getCamera()->setForwardSpeed(0);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) != GLFW_PRESS && glfwGetKey(m_window->getWindow(), GLFW_KEY_D) != GLFW_PRESS)
        m_graphics->getCamera()->setHorizontalSpeed(0);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) != GLFW_PRESS && glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        m_graphics->getCamera()->setVertSpeed(0);

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_graphics->getCamera()->setForwardSpeed(1);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_graphics->getCamera()->setForwardSpeed(-1);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_graphics->getCamera()->setHorizontalSpeed(1);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_graphics->getCamera()->setHorizontalSpeed(-1);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_graphics->getCamera()->setVertSpeed(1);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_graphics->getCamera()->setVertSpeed(-1);


    if (m_focused) {
        double xPos, yPos;
        glfwGetCursorPos(m_window->getWindow(), &xPos, &yPos);

        float xDelta = (m_WINDOW_WIDTH / 2) - (float)xPos, yDelta = (m_WINDOW_HEIGHT / 2) - (float)yPos;
        m_graphics->getCamera()->setRotateDelta(xDelta, yDelta);

        glfwSetCursorPos(m_window->getWindow(), m_WINDOW_WIDTH / 2, m_WINDOW_HEIGHT / 2);
    }



}

unsigned int Engine::getDT()
{
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
    return (long long) glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdate2(time);
}

void Engine::setFocused(bool focused) {
    if (focused) {
        glfwSetCursorPos(m_window->getWindow(), m_WINDOW_WIDTH / 2, m_WINDOW_HEIGHT / 2);
        m_focused = true;
    }
    else {
        m_graphics->getCamera()->setSpeed(glm::vec3(0));
        m_focused = false;
    }
}

static void windowFocusCallback(GLFWwindow* window, int focused) {
    if (focused)
        engine->setFocused(true);
    else
        engine->setFocused(false);
}

static void scroll_wheel_callback(GLFWwindow* window, double scrollX, double scrollY) {
    engine->adjustZoom(scrollY);
}

