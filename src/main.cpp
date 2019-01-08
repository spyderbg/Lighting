#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <ostream>
#include <iostream>

#include "Camera.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


float mix_factor = 0.5f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static void error_callback(int error, const char* description)
{
	std::cout << "Error: " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mix_factor -= 0.01f;
		if(mix_factor < 0.0f) mix_factor = 0.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mix_factor += 0.01f;
		if(mix_factor > 1.0f) mix_factor = 1.0f;
	}

	auto cameraSpeed = 0.05f;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float fov = 45.0f;
float yaw, pitch, roll;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	float sesitivity = 0.05f;
	xoffset *= sesitivity;
	yoffset *= sesitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	glm::vec3 front;
	front.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}


float points[] = {
	-0.5f, 0.5f,
	0.5f, 0.5f,
	0.5f, -0.5f,
	-0.5f, -0.5f
};
int main()
{
	GLFWwindow* window;
	GLuint vertex_buffer;
	GLuint vertex_array;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);


	if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// init shaders 
	Shader shader("res\\simple.vs", "res\\simple.fs", "res\\simple.gs");
	shader.Compile();

	// init buffers
	glGenBuffers(1, &vertex_buffer);

	// set VAO
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// create viewport
	std::cout << "Create viewport" << std::endl;
	glViewport(0, 0, 800, 600);

	// main loop
	while(!glfwWindowShouldClose(window))
	{
		// process input
		process_input(window);

		// rendering commands here
		// ...
		glClearColor(.2f, .3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		glDrawArrays(GL_POINTS, 0, 4);


		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
