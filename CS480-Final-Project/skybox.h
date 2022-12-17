#pragma once
#include <vector>
#include <iostream>
#include "graphics_headers.h"
#include "object.h"
#include <SOIL2/SOIL2.h>
#include "Texture.h"

using namespace std;

class SkyBox : 
	public Object
{
public:
	SkyBox(const char* tfname, int w, int h);
	unsigned int loadCubemap(const char* fname);

private:
	void setupVerticies();
	bool InitBuffers();
	void Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc);

	Texture* m_texture;

	GLint cubeMapRenderer;
	int sectionW, sectionH;
};

