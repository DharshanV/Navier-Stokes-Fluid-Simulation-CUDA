#include "Simulation.h"
#include "Numerics.h"
#include <iostream>

Simulation::Simulation(uint width, uint height,uint N) : width(width), height(height), N(N) {
	uint size = N * N;

	d = new float[size];
	p_d = new float[size];
	u = new float[size];
	p_u = new float[size];
	v = new float[size];
	p_v = new float[size];

	memset(d, 0, size * sizeof(float));
	memset(p_d, 0, size * sizeof(float));
	memset(u, 0, size * sizeof(float));
	memset(p_u, 0, size * sizeof(float));
	memset(v, 0, size * sizeof(float));
	memset(p_v, 0, size * sizeof(float));
}

Simulation::~Simulation() {
	delete[] d;
	delete[] p_d;
	delete[] u;
	delete[] p_u;
	delete[] v;
	delete[] p_v;
}

void Simulation::step(float dt) {
	// velocity
	diffuse(1, p_u, u, 0.0000001f, dt, 16, N);
	diffuse(2, p_v, v, 0.0000001f, dt, 16, N);

	project(p_u, p_v, u, v, 16, N);

	advect(1, u, p_u, p_u, p_v, dt, N);
	advect(2, v, p_v, p_u, p_v, dt, N);

	project(u, v, p_u, p_v, 16, N);

	//density
	diffuse(0, p_d, d, 0.0f, dt, 16, N);
	advect(0, d, p_d, u, v, dt, N);
	fadeDensity();
}

void Simulation::addDensity(uint x, uint y, uint amount) {
	d[IX(x, y, N)] += amount;
}

void Simulation::addVelocity(uint x, uint y, uint du, uint dv) {
	u[IX(x, y, N)] += du;
	v[IX(x, y, N)] += dv;
}

void Simulation::addWind(float speed) {
	for (int j = N / 2 - 5; j <= N / 2 + 5; j+=2) {
		u[IX(3, j, N)] += speed;
	}
}

void Simulation::result(Color* color, ColorMode colorMode) {
	for (uint y = 0; y < height; y++) {
		for (uint x = 0; x < width; x++) {
			color[y * width + x] = mapPixelToCell(x,y,colorMode);
		}
	}
}

void Simulation::fadeDensity() {
	uint size = N * N;
	for (int i = 0; i < size; i++) {
		float newD = d[i];
		d[i] = (newD - 0.1f < 0) ? 0 : newD - 0.1f;
	}
}

Color Simulation::Hsv(int hue, float sat, float val, float d) {
	hue %= 360;
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h) {
		default:
		case 0:
		case 6: return Color(val * 255, t * 255, p * 255, d);
		case 1: return Color(q * 255, val * 255, p * 255, d);
		case 2: return Color(p * 255, val * 255, t * 255, d);
		case 3: return Color(p * 255, q * 255, val * 255, d);
		case 4: return Color(t * 255, p * 255, val * 255, d);
		case 5: return Color(val * 255, p * 255, q * 255, d);
	}
}

float Simulation::MapToRange(float val, float minIn, float maxIn, float minOut, float maxOut) {
	float x = (val - minIn) / (maxIn - minIn);
	float result = minOut + (maxOut - minOut) * x;
	return (result < minOut) ? minOut : (result > maxOut) ? maxOut : result;
}

Color Simulation::mapPixelToCell(uint pixelX, uint pixelY, ColorMode colorMode) {
	Color cellColor(0,0,0,0);
	float dx = (float)width / (float)N;
	float dy = (float)height / (float)N;

	uint x = pixelX / dx;
	uint y = pixelY / dy;

	switch (colorMode) {
	case ColorMode::HSV:
		cellColor = Hsv((this->d[IX(x, y, N)]), 1, 1, 255);
		break;
	case ColorMode::VELOCITY:
	{
		int r = (int)this->MapToRange(u[IX(x, y, N)], -0.05f, 0.05f, 0, 255);
		int g = (int)this->MapToRange(v[IX(x, y, N)], -0.05f, 0.05f, 0, 255);
		cellColor = Color(r, g, 255);
		break;
	}
	case ColorMode::DEFAULT:
	default:
		cellColor = Color(255, 255, 255, (d[IX(x, y, N)] > 255) ? 255 : d[IX(x, y, N)]);
		break;
	}

	return cellColor;
}

void Simulation::addBox() {
	uint center = N / 2;
	uint side = 15;
	for (uint y = center- side; y <= center+ side; y++) {
		for (uint x = center-side-20; x <= center+ side-20; x++) {
			u[IX(x, y, N)] = v[IX(x, y, N)] = p_u[IX(x, y, N)] = p_v[IX(x, y, N)] = d[IX(x, y, N)] = 0;
		}
	}
}