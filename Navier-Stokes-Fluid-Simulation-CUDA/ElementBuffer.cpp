#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(uint32_t* data, uint32_t size, GLenum type) : data(data), size(size) {
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, type);
}

void ElementBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void ElementBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t ElementBuffer::getSize() {
	return size;
}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &bufferID);
}