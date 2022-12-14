#include "mesh.h"

//Constructor
Mesh::Mesh()
{
	// Vertex Set Up
	// No mesh

	// Model Set Up
	pivotLocation = glm::vec3(0.f, 0.f, 0.f);
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}

}

//Constructor
Mesh::Mesh(glm::vec3 pivot, const char* fname)
{
	// Vertex Set Up
	loadModelFromFile(fname);

	// Model Set Up
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}

	hasTex = false;
}

//Constructor
Mesh::Mesh(glm::vec3 pivot, const char* fname, const char* tname)
{
	// Vertex Set Up
	loadModelFromFile(fname);

	// Model Set Up
	pivotLocation = pivot;
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("some buffers not initialized.\n");
	}

	// load texture from file
	m_texture = new Texture(tname);
	if (m_texture)
		hasTex = true;
	else
		hasTex = false;
}

//Destructor
Mesh::~Mesh()
{
	Vertices.clear();
	Indices.clear();
}

//Update the starship matix
void Mesh::Update(double dt)
{
	if (!spectateMode) {
		// Add velocity to position
		glm::vec3 forwardVector = glm::normalize(glm::vec3(model[2][0], model[2][1], model[2][2]));
		glm::vec3 rightVector = glm::normalize(glm::vec3(model[0][0], model[0][1], model[0][2]));

		glm::mat4 tmat = glm::translate(glm::mat4(1), (shipPosition += (forwardSpeed * forwardVector * (float)dt)));

		// Rotate ship based on mouse offset
		horizAngle += dt * xMouseDelta;
		vertAngle -= dt * yMouseDelta;

		xMouseDelta = yMouseDelta = 0;

		vertAngle = glm::clamp(vertAngle, -3.1414f / 2, 3.1414f / 2);
		horizAngle = horizAngle > (2 * 3.1415) ? horizAngle - (2 * 3.1415) : horizAngle < 0 ? horizAngle + (2 * 3.1415) : horizAngle;

		glm::mat4 rmat = glm::rotate(glm::mat4(1), horizAngle, glm::vec3(0, 1, 0));
		rmat *= glm::rotate(glm::mat4(1), vertAngle, glm::vec3(1, 0, 0));

		glm::mat4 smat = glm::scale(glm::mat4(1), glm::vec3(uniformScale));

		model = tmat * rmat * smat;
	}
	else 
	{
		toSpectate->horizRotateSpectator(horizontalSpeed / 100);
		toSpectate->vertRotateSpectator(forwardSpeed / 100);

		model = glm::translate(toSpectate->getSpectatorModel(), glm::vec3(0, 0, (toSpectate->getScale()[0]*3)));
		model *= glm::scale(glm::mat4(1), glm::vec3(uniformScale));

		shipPosition = glm::vec3(model[3][0], model[3][1], model[3][2]);
	}

	updateCamera();
}

//Update the camera view
void Mesh::updateCamera() {
	glm::vec3 localForward = glm::normalize(glm::vec3(
		model[2][0],
		model[2][1],
		model[2][2]
	));

	glm::vec3 localUp = glm::normalize(glm::vec3(
		model[1][0],
		model[1][1],
		model[1][2]
	));

	if (!spectateMode) {

		// Ship rotates along the x
		glm::vec3 thirdPersonPositionOffset = -localForward * -thirdPersonOffsets.x;
		thirdPersonPositionOffset += localUp * thirdPersonOffsets.y;

		m_camera->setPerspective(shipPosition + thirdPersonPositionOffset, shipPosition + (localUp * thirdPersonVerticalFocusOffset), localUp);
	}
	else {
		glm::vec3 firstPersonPositionOffset = localUp * firstPersonOffsets.y;

		glm::mat4 specModel = toSpectate->getSpectatorModel();
		glm::vec3 focalPoint = glm::vec3(specModel[3][0], specModel[3][1], specModel[3][2]);

		std::cout << "Focal point: " << focalPoint[0] << ", " << focalPoint[1] << ", " << focalPoint[2] << std::endl;

		m_camera->setPerspective(shipPosition, focalPoint, -localUp);
	}
}

//Return the ship model matrix
glm::mat4 Mesh::GetModel()
{
	return model;
}

//Render the ship
void Mesh::Render(GLint posAttribLoc, GLint colAttribLoc)
{

	glBindVertexArray(vao);

	// Enable vertex attibute arrays for each vertex attrib
	glEnableVertexAttribArray(posAttribLoc);
	glEnableVertexAttribArray(colAttribLoc);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Set vertex attribute pointers to the load correct data
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(posAttribLoc);
	glDisableVertexAttribArray(colAttribLoc);
}

//Render the ship
void Mesh::Render(GLint posAttribLoc, GLint colAttribLoc, GLint normalAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc)
{
	glBindVertexArray(vao);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Enable vertex attibute arrays for each vertex attrib
	glEnableVertexAttribArray(posAttribLoc);
	glEnableVertexAttribArray(colAttribLoc);
	glEnableVertexAttribArray(normalAttribLoc);
	glEnableVertexAttribArray(tcAttribLoc);

	// Set vertex attribute pointers to the load correct data
	glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	//change the offset
	glVertexAttribPointer(tcAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	glVertexAttribPointer(normalAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	// If has texture, set up texture unit(s) Update here to activate and assign texture unit
	if (m_texture != NULL) {
		glUniform1i(hasTextureLoc, true);
	}
	else
		glUniform1i(hasTextureLoc, false);


	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	// Disable vertex arrays
	glDisableVertexAttribArray(posAttribLoc);
	glDisableVertexAttribArray(colAttribLoc);
	glDisableVertexAttribArray(normalAttribLoc);
	glDisableVertexAttribArray(tcAttribLoc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Initialize buffers for the vertices and indices
bool Mesh::InitBuffers() {

	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return true;
}

//Load the model from file
bool Mesh::loadModelFromFile(const char* path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene) {
		printf("couldn't open the .obj file. \n");
		return false;
	}

	const int ivertTotalSize = 2 * sizeof(aiVector3D);

	int iTotalVerts = 0;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;
		for (int j = 0; j < iMeshFaces; j++) {
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < 3; k++) {
				// update here for each mesh's vertices to assign position, normal, and texture coordinates
				Vertices.push_back(Vertex(
					// Position vector
					glm::vec3(mesh->mVertices[face.mIndices[k]].x,
						mesh->mVertices[face.mIndices[k]].y,
						mesh->mVertices[face.mIndices[k]].z),



					// Color vector
					glm::vec3(mesh->mVertices[face.mIndices[k]].x,
						mesh->mVertices[face.mIndices[k]].y,
						mesh->mVertices[face.mIndices[k]].z),

					glm::vec2(mesh->mTextureCoords[0][face.mIndices[k]].x, 
						mesh->mTextureCoords[0][face.mIndices[k]].y)
				));
			}
		}
		iTotalVerts += mesh->mNumVertices;
	}

	for (int i = 0; i < Vertices.size(); i++) {
		Indices.push_back(i);
	}


	return true;
}

//Change mode to spectate
void Mesh::spectate(Sphere* toSpectate, float spectateDistance) {
	this->toSpectate = toSpectate;
	m_camera->spectate();
	this->spectateMode = true;
}

//Reset the camera view
void Mesh::resetSpectatorView() {
	if (spectateMode) {
		m_camera->resetZoom();
		toSpectate->resetSpectatorModel();
	}
}

//Change mode to explore
void Mesh::explore() {
	if (spectateMode) {
		m_camera->noSpectate();
		m_camera->resetZoom();
		toSpectate->resetSpectatorModel();
		this->spectateMode = false;
	}
}
