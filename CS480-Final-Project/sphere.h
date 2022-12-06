#include "object.h"
#include "Texture.h"
#include "trig_functions.h"

class Sphere :
    public Object
{
public:
    Sphere();


    void Render(GLint positionAttribLoc, GLint colorAttribLoc);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);

    glm::mat4 GetModel() { return model; }
    void Update(glm::mat4 matModel);

    Sphere(int prec);
    Sphere(int prec, const char* fname);

    int getNumVertices();
    int getNumIndices();
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();

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

    void setupVertices();
    void setupBuffers();
    void setupModelMatrix(glm::vec3 pivotLoc, float angle, float scale);

    int numVertices;
    int numIndices;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    void init(int);
    float toRadians(float degrees);

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors

 

};