#include "Texture.h"

//Constructor
Texture::Texture(const char* fileName) {

	loadTexture(fileName);
	initializeTexture();
}

//Constructor
Texture::Texture() {
	m_TextureID = -1;
	printf("No Texture Data Provided.");
}

//Load textur from file
bool Texture::loadTexture(const char* texFile) {
	m_TextureID = SOIL_load_OGL_texture(texFile, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (!m_TextureID) {
		printf("Failed: Could not open texture file!\n");
		return false;
	}
	return true;
}

//Initialize the texture
bool Texture::initializeTexture() {

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);

	
	return true;
}



