#include "asteroid_instancer.h"

AsteroidInstancer::AsteroidInstancer(const char* vShader, const char* fShader, const char* fname, const char* tname, int asteroidCount, float radius) {
	this->asteroidCount = asteroidCount;
	this->radius = radius;
	models = new glm::mat4[asteroidCount];

	instance_shader = new Shader(vShader, fShader);
	m_texture = new Texture(tname);
	loadModelFromFile(fname);
	generateModels();
	getShaderLocs();

	if (!InitBuffers())
		printf("Some buffers not initialized!");
}

bool AsteroidInstancer::InitBuffers() {
	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &MB);
	glBindBuffer(GL_ARRAY_BUFFER, MB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * asteroidCount, &models[0], GL_STATIC_DRAW);

	return true;
}

void AsteroidInstancer::Update(double dt) {
	originPoint = glm::rotate(originPoint, (float) dt * 0.005f, glm::vec3(0, 1, 0));
}

bool AsteroidInstancer::loadModelFromFile(const char* path) {
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
					glm::vec3(mesh->mNormals[face.mIndices[k]].x,
						mesh->mNormals[face.mIndices[k]].y,
						mesh->mNormals[face.mIndices[k]].z),

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

void AsteroidInstancer::generateModels() {
	originPoint = glm::translate(glm::mat4(1), glm::vec3(0));
	srand(glfwGetTime());
	for (int i = 0; i < asteroidCount; i++) {
		glm::mat4 model = glm::mat4(1);

		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)asteroidCount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: scale between 0.0005 and 0.000025f
		float scale = 0.00005f + ((rand() % 10)/1000000);
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		models[i] = model;
	}
}

void AsteroidInstancer::getShaderLocs() {
	posAttrib = 0;
	colAttrib = 1;
	tcAttrib = 2;
	//normAttrib = 3;

	m_view = instance_shader->GetUniformLocation("viewMatrix");
	m_proj = instance_shader->GetUniformLocation("projectionMatrix");
	m_model = instance_shader->GetUniformLocation("originMatrix");

	timeFactor = instance_shader->GetUniformLocation("tf");
}

void AsteroidInstancer::Render(double totalTime, glm::mat4 cameraView, glm::mat4 cameraProjection) {

	glUniformMatrix4fv(m_view, 1, GL_FALSE, glm::value_ptr(cameraView));
	glUniformMatrix4fv(m_proj, 1, GL_FALSE, glm::value_ptr(cameraProjection));
	glUniformMatrix4fv(m_model, 1, GL_FALSE, glm::value_ptr(originPoint));


	//get sun color
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//get sun pos
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glUniformMatrix3fv(glGetUniformLocation(instance_shader->getProgram(), "normMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(m_proj * m_model)))));
	glUniform4f(glGetUniformLocation(instance_shader->getProgram(), "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(instance_shader->getProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	glUniform1f(timeFactor, (float) totalTime);

	glBindBuffer(GL_ARRAY_BUFFER, VB);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->getTextureID());
	
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colAttrib);
	glEnableVertexAttribArray(tcAttrib);

	// Set vertex attribute pointers to the load correct data
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glVertexAttribPointer(tcAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	glBindBuffer(GL_ARRAY_BUFFER, MB);

	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glDrawElementsInstanced(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0, asteroidCount);

	// Disable vertex arrays
	glDisableVertexAttribArray(posAttrib);
	glDisableVertexAttribArray(colAttrib);
	glDisableVertexAttribArray(tcAttrib);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}