#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

const unsigned int width = 1920;
const unsigned int height = 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void delete_already_linked_shaders(unsigned int* shadersArr, int size);

int main()
{
	
	// Initialize the GLFW Library
	// ---------------------------
	glfwInit();

	// Hints - options provided to guide the windows behavior
	// but they are not strict req's or guarenteed settings
	// will be set for the next call to glfwCreateWindow
	// ----------------------------------------------------
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creates a window with the specified width and height.
	// This determines the size of the window on the screen	
	// As long as monitor param is not null, it will create full screen window on the monitor specified
	GLFWwindow* window = glfwCreateWindow(width, height, "Hello Triangle", NULL, NULL);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW Window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// When you create a window, the windowing library also internally
	// creates an associated OpenGL context specifically for that window.
	// Making the windows context current assures that OpenGL flushes all its 
	// rendering results to the specified window.
	glfwMakeContextCurrent(window);

	// Callbacks
	// ---------
	// Callback to handle resize of openGL viewport when window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Callback handle input
	glfwSetKeyCallback(window, key_callback);

	// Callback handle mouse
	// glfwSetCursorPosCallback(window, cursor_position_callback);

	// Mouse Setup
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	// Initialize GLAD (OpenGL Loader Generator).
	// This allows us our code to access functions on the 
	// GPU drivers. OS returns a functino pointer allowing us to call the functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Sets the OpenGL viewport independently of the window size.
	// Specifies portion of the window where rendering output will be displayed.
	glViewport(0, 0, width, height);

	// build and compile our shader program
	// ------------------------------------
	Shader shaderProgram("vertex.glsl", "fragment.glsl");
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	GLfloat vertices[] =
	{
		// Positions
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,	0.5f, 0.0f,
	};

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));

	VAO1.LinkVBO(VBO1, 0);

	// Unbind all buffers to prevent accidental modifications
	VAO1.Unbind();
	VBO1.UnBind();

	// Render loop - Every iteration is known as a frame
	// ------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Set the clear color for current rendering context
		// glClear uses color that is set above when clearing the specified buffer
		glClearColor((159.0f / 255.0f), (184.0f / 255.0f), (173.0f / 255.0f), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		VAO1.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		/*
		glfwSwapBuffers(window) is a function call that updates the screen by swapping
		the back buffer (where rendering occurs) with the front buffer (what the user sees),
		ensuring that the latest rendered image is displayed.

		In CG we have a front and a back buffer. The GPU uses
		multiple buffers to store stages of the rendering process.
		The back buffer represents the area where the rendering is performed off-screen.
		The front buffer represents the visible area. So when you swap buffers, GLFW exchanges
		the content of the back buffer, with the front buffer
		*/
		glfwSwapBuffers(window);
		// Checks if any events are triggered (keyboard, mouse)
		// and updates the windows state.
		glfwPollEvents();
	}

	// Delete all objects that have been created
	VAO1.Delete();
	VBO1.Delete();
	shaderProgram.Delete();

	// Delete window before endeding the program
	glfwDestroyWindow(window);

	// Terminate GLFW before ending program
	glfwTerminate();
	return 0;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
};

void delete_already_linked_shaders(unsigned int* shadersArr, int size)
{
	for (int i = 0; i < size; ++i)
	{
		glad_glDeleteShader(i);
	}

};
