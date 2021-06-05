#pragma once
#include <iostream>
#include <glad/glad.h>

#include "Color.h"
using namespace std;

class Texture {
public:
	Texture(const Color* data, uint32_t width, uint32_t height);
	void bind(unsigned int slot = 0);
	void load(const Color* data);
	void unbind();
	GLuint id();
	~Texture();
private:
	GLuint textureID;
	uint32_t width;
	uint32_t height;
};
