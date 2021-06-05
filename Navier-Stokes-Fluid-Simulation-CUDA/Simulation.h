#pragma once
#include <vector>
#include "Color.h"
#include <glm/vec2.hpp>

typedef unsigned int uint;

enum class ColorMode {
	DEFAULT,
	HSV,
	VELOCITY
};

class Simulation {
public:
	Simulation(uint width, uint height, uint N);

	~Simulation();

	void step(float dt);
	void result(Color* color, ColorMode colorMode);

	void addDensity(uint x, uint y, uint amount);
	void addVelocity(uint x, uint y, uint du, uint dv);
	void addWind(float speed);
	void addBox();

private:
	void fadeDensity();
	Color Hsv(int hue, float sat, float val, float d);
	float MapToRange(float val, float minIn, float maxIn, float minOut, float maxOut);
	Color mapPixelToCell(uint pixelX, uint pixelY, ColorMode colorMode);
private:
	uint width;
	uint height;
	uint N;

	float* d;
	float* p_d;
	float* u;
	float* v;
	float* p_u;
	float* p_v;
};