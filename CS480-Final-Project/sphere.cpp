#include "sphere.h"

//Constuctor
Sphere::Sphere()
{
    init(20);
    setupVertices();
    setupBuffers();
    //setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);
}

//Constuctor
Sphere::Sphere(int prec) { // prec is precision, or number of slices

    init(prec);
    setupVertices();
    setupBuffers();
    //setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);
    hasTex = false;
}

//Constructor
Sphere::Sphere(int prec, const char* fname) { // prec is precision, or number of slices

    init(prec);
    setupVertices();
    setupBuffers();
    //setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);

        // load texture from file
    m_texture = new Texture(fname);
    if (m_texture)
        hasTex = true;
    else
        hasTex = false;
}

//Render the sphere
void Sphere::Render(GLint positionAttribLoc, GLint colorAttribLoc)
{
    glBindVertexArray(vao);
    // Enable Vertext Attributes
    glEnableVertexAttribArray(positionAttribLoc);
    glEnableVertexAttribArray(colorAttribLoc);
    //glEnableVertexAttribArray(normalAttribLoc);

    // Bind your VBO buffer(s) and then setup vertex attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));


    // Bind your index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    // Render
    glDrawArrays(GL_TRIANGLES, 0, getNumIndices());

    // Disable Vertex Attribuates
    glDisableVertexAttribArray(positionAttribLoc);
    glDisableVertexAttribArray(colorAttribLoc);
    //glDisableVertexAttribArray(normalAttribLoc);
}

//Render the sphere
void Sphere::Render(GLint posAttribLoc, GLint colAttribLoc, GLint normalAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc)
{
    //Bind VAO
    glBindVertexArray(vao);
    // Enable vertex attibute arrays for each vertex attrib

    //Set vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glEnableVertexAttribArray(posAttribLoc);
    glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), 0);

    //Set texture attributes
    glBindBuffer(GL_ARRAY_BUFFER, TB);
    glEnableVertexAttribArray(tcAttribLoc);
    glVertexAttribPointer(tcAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(texCoords[0]), 0);

    //Set normal attributes
    glBindBuffer(GL_ARRAY_BUFFER, NB);
    glEnableVertexAttribArray(normalAttribLoc);
    glVertexAttribPointer(normalAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(normals[0]), 0);


    // If has texture, set up texture unit(s): update here for texture rendering
    if (m_texture != NULL) {
        glUniform1i(hasTextureLoc, true);
    }
    else
        glUniform1i(hasTextureLoc, false);


    // Bind your Element Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    // Render
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    // Disable vertex arrays
    glDisableVertexAttribArray(posAttribLoc);
    glDisableVertexAttribArray(colAttribLoc);
    glDisableVertexAttribArray(tcAttribLoc);
    glDisableVertexAttribArray(normalAttribLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Setup the vertices, indices, texcoords, normals
void Sphere::setupVertices() {

    std::vector<int> ind = getIndices();
    std::vector<glm::vec3> vert = getVertices();
    std::vector<glm::vec2> tex = getTexCoords();
    std::vector<glm::vec3> norm = getNormals();

    int numIndices = getNumIndices();
    for (int i = 0; i < numIndices; i++) {
        Indices.push_back(indices[i]);
    }

    int numVertices = getNumVertices();
    for (int i = 0; i < numVertices; i++) {
        Vertices.push_back(Vertex(vertices[i], normals[i], texCoords[i]));
    }
}

//Setup buffers for vertices, indices, texcoords, normals
void Sphere::setupBuffers() {
    // For OpenGL 3
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &TB);
    glBindBuffer(GL_ARRAY_BUFFER, TB);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);

    glGenBuffers(1, &NB);
    glBindBuffer(GL_ARRAY_BUFFER, NB);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

//Setup the model matrix
void Sphere::setupModelMatrix(glm::vec3 pivot, float angle, float scale) {
    pivotLocation = pivot;
    model = glm::translate(glm::mat4(1.0f), pivotLocation);
    model *= glm::rotate(glm::mat4(1.f), angle, glm::vec3(0., 1., 0));
    model *= glm::scale(glm::vec3(scale, scale, scale));
}

//Update the model matrix
void Sphere::Update(glm::mat4 matModel) {
    model = matModel;
}

//Update the spectator matrix
void Sphere::UpdateSpecModel(glm::mat4 specModel) {
    this->spectatorModel = specModel;
    spectatorModel *= glm::rotate(glm::mat4(1), spectateHorizRotation, glm::vec3(0, 1, 0));
    spectatorModel *= glm::rotate(glm::mat4(1), spectateVertRotation, glm::vec3(1, 0, 0));
}

//Convert degrees to radians
float Sphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

//Initialize the sphere object
void Sphere::init(int prec) {
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
    for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            float y = (float)cos(toRadians(180.f - i * 180.f / prec));
            float x = -(float)cos(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
        }
    }

    // calculate triangles indices
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            int index = 6 * (i * prec + j);
			indices[index] = i * (prec + 1) + j;
			indices[index + 1] = i * (prec + 1) + j + 1;
			indices[index + 2] = (i + 1) * (prec + 1) + j;
			indices[index + 3] = i * (prec + 1) + j + 1;
			indices[index + 4] = (i + 1) * (prec + 1) + j + 1;
			indices[index + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}

// accessors
int Sphere::getNumVertices() { return numVertices; }
int Sphere::getNumIndices() { return numIndices; }
std::vector<int> Sphere::getIndices() { return indices; }
std::vector<glm::vec3> Sphere::getVertices() { return vertices; }
std::vector<glm::vec2> Sphere::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Sphere::getNormals() { return normals; }