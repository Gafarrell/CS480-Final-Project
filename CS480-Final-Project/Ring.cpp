#include <cmath>
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Ring.h"
using namespace std;

//Constructor
Ring::Ring() {
	prec = 48;
	inner = 0.5f;
	outer = 0.2f;
	init();
}

//Constructor
Ring::Ring(float in, float out, int precIn, const char* fname) {
	prec = precIn;
	inner = in;
	outer = out;
	init();
	setupVertices();
	setupBuffers();

	m_texture = new Texture(fname);
	if (m_texture)
		hasTex = true;
	else
		hasTex = false;
}

//Render the object
void Ring::Render(GLint posAttribLoc, GLint colAttribLoc, GLint normalAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc)
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
	glDisableVertexAttribArray(tcAttribLoc);
	glDisableVertexAttribArray(normalAttribLoc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Convert degrees to radians
float Ring::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

//Initialize the vertices, indices, texcoords, and normals
void Ring::init() {
	numVertices = (prec + 1) * (prec + 1);
	numIndices = prec * prec * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { sTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { tTangents.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate first ring
	for (int i = 0; i < prec + 1; i++) {
		float amt = toRadians(i*360.0f / prec);

		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 5.0f, 1.0f));
		glm::vec3 initPos(rMat * glm::vec4(0.0f, outer, 0.0f, 1.0f));

		vertices[i] = glm::vec3(initPos + glm::vec3(inner, 0.0f, 0.0f));
		texCoords[i] = glm::vec2(0.0f, ((float)i / (float)prec));

		rMat = glm::rotate(glm::mat4(1.0f), amt + (3.14159f / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		tTangents[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

		sTangents[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
		normals[i] = glm::cross(tTangents[i], sTangents[i]);
	}
	// rotate the first ring about Y to get the other rings
	for (int ring = 1; ring < prec + 1; ring++) {
		for (int i = 0; i < prec + 1; i++) {
			float amt = (float)toRadians((float)ring * 360.0f / (prec));

			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			vertices[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));

			texCoords[ring*(prec + 1) + i] = glm::vec2((float)ring*2.0f / (float)prec, texCoords[i].t);

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			sTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(sTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			tTangents[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(tTangents[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			normals[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(normals[i], 1.0f));
		}
	}
	// calculate triangle indices
	for (int ring = 0; ring < prec; ring++) {
		for (int i = 0; i < prec; i++) {
			indices[((ring*prec + i) * 2) * 3 + 0] = ring*(prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2) * 3 + 2] = ring*(prec + 1) + i + 1;
			indices[((ring*prec + i) * 2 + 1) * 3 + 0] = ring*(prec + 1) + i + 1;
			indices[((ring*prec + i) * 2 + 1) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices[((ring*prec + i) * 2 + 1) * 3 + 2] = (ring + 1)*(prec + 1) + i + 1;
		}
	}
}

//Setup the vertices
void Ring::setupVertices() {
	std::vector<int> ind = getIndices();
	std::vector<glm::vec3> vert = getVertices();
	std::vector<glm::vec2> tex = getTexCoords();
	std::vector<glm::vec3> norm = getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	int numIndices = getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		Indices.push_back(indices[i]);
	}

	int numVertices = getNumVertices();
	for (int i = 0; i < numVertices; i++) {
		Vertices.push_back(Vertex(vertices[i], normals[i], texCoords[i]));
	}
	/*for (int i = 0; i < getNumVertices(); i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}*/
	
}

//Setup the buffer form vertices, indices, texcoords, normals
void Ring::setupBuffers() {
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

//Setup the model matix
void Ring::setupModelMatrix(glm::vec3 pivot, float angle, float scale) {
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);
	model *= glm::rotate(glm::mat4(1.f), angle, glm::vec3(0., 1., 0));
	model *= glm::scale(glm::vec3(scale, scale, scale));
}

//Update the model matrix
void Ring::Update(glm::mat4 matModel) {
	model = matModel;
}

int Ring::getNumVertices() { return numVertices; }
int Ring::getNumIndices() { return numIndices; }
std::vector<int> Ring::getIndices() { return indices; }
std::vector<glm::vec3> Ring::getVertices() { return vertices; }
std::vector<glm::vec2> Ring::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Ring::getNormals() { return normals; }
std::vector<glm::vec3> Ring::getStangents() { return sTangents; }
std::vector<glm::vec3> Ring::getTtangents() { return tTangents; }