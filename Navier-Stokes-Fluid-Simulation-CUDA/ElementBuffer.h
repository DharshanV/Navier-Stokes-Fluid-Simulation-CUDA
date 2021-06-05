#pragma once
#include <glad/glad.h>
#include <iostream>
using namespace std;

class ElementBuffer {
public:
	ElementBuffer(uint32_t* data, uint32_t size, GLenum type = GL_STATIC_DRAW);
	void bind() const;
	void unbind() const;

	uint32_t getSize();

	~ElementBuffer();
private:
	unsigned int bufferID;
	uint32_t* data;
	uint32_t size;
};