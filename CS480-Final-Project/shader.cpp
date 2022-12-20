#include "shader.h"
#include "Utils.h"

//Constructor
Shader::Shader(const char* vShaderFile, const char* fShaderFile)
{
    m_shaderProg = Utils::createShaderProgram(vShaderFile, fShaderFile);
}

//Destructor
Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

//Initialize the shader
bool Shader::Initialize()
{
  return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  return true;
}

//Tell OpenGL to use this shader
void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}

//Get location of uniform
GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

//Get location of attribute
GLint Shader::GetAttribLocation(const char* pAttribName)
{
    GLuint Location = glGetAttribLocation(m_shaderProg, pAttribName);

    if (Location == -1) {
        printf("Warning! Unable to get the location of attribute '%s'\n", pAttribName);
    }

    return Location;
}
