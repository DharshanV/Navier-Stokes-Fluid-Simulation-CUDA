#include <iostream>

#include "GLFW.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Simulation.h"

using namespace glm;

uint index(uint x, uint y);
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void renderQuad(Shader& quadShader, VertexArray& VAO, ElementBuffer& EBO, Texture& quadTexture);

constexpr uint N = 200;
constexpr uint WIDTH = 600;
constexpr uint HEIGHT = 300;
ColorMode colorMode = ColorMode::DEFAULT;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	srand(time(0));

	//==================
	//Setup GLFW and OpenGL
	GLFW window(WIDTH, HEIGHT, "Navier-Stokes Fluid Simulation");

	if (!window.isGood()) { glfwTerminate(); return EXIT_FAILURE; }
	window.setClearColor({ 0.0f,0,0,1.0f });
	//==================

	//==================
	//Create our 2D quad
	Shader quadShader("QuadShader.vert", "QuadShader.frag");
	quadShader.use();

	float vertices[] = {
		// positions         // texture coords
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // top right
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // top left 
	};
	uint indices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//send to the GPU
	VertexArray VAO;
	VertexBuffer VBO(vertices, sizeof(vertices));
	ElementBuffer EBO(indices, sizeof(indices));

	VertexBufferLayout layout;
	layout.push<float>(3);
	layout.push<float>(2);
	VAO.addBuffer(VBO, layout);
	//==================

	//==================
	//create quad texture
	Color* data = new Color[WIDTH * HEIGHT];
	for (uint y = 0; y < HEIGHT; y++) {
		for (uint x = 0; x < WIDTH; x++) {
			data[index(x,y)] = { 0, 0 , 0, 0};
		}
	}
	Texture quadTexture(&data[0], WIDTH, HEIGHT);
	//==================

	//==================
	//Create Simulation
	Simulation fluidSimulation(WIDTH, HEIGHT, N);
	//=================

	//==================
	//Render our quad
	double lastTime = glfwGetTime();
	while (!window.close()) {
		processInput(window.getWindow());
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();

		//fluidSimulation.addBox();
		fluidSimulation.addWind(1.0f);
		for (int j = N / 2 - 5; j <= N / 2 + 5; j++) {
			fluidSimulation.addDensity(6, j, 200);
		}
		fluidSimulation.step(0.1f);
		fluidSimulation.result(&data[0], colorMode);
		quadTexture.load(&data[0]);

		renderQuad(quadShader, VAO, EBO, quadTexture);

		window.swapBuffers();
		window.getEvents();
	}

	delete[] data;
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		colorMode = ColorMode::DEFAULT;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		colorMode = ColorMode::HSV;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		colorMode = ColorMode::VELOCITY;
}

void renderQuad(Shader& quadShader,VertexArray& VAO,ElementBuffer& EBO,Texture& quadTexture) {
	quadShader.use();
	VAO.bind();
	quadTexture.bind();
	glDrawElements(GL_TRIANGLES, EBO.getSize(), GL_UNSIGNED_INT, 0);
	quadTexture.unbind();
	VAO.unbind();
}

uint index(uint x, uint y) {
	return y * WIDTH + x;
}