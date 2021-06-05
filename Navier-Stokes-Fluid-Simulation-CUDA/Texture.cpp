#include "Texture.h"

Texture::Texture(const Color* data, uint32_t width, uint32_t height) {
	textureID = 0;
	this->width = width;
	this->height = height;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(data->r));
}

void Texture::bind(unsigned int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::load(const Color* data) {
	this->bind();
	if (data != nullptr) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(data->r));
	}
	this->unbind();
}

void Texture::unbind() {
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::id() {
	return textureID;
}

Texture::~Texture() {
	cout << "DELETED TEXTURE" << endl;
	glDeleteTextures(1, &textureID);
}