#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>

namespace xen
{
	// window datatype
	struct Window
	{
		GLFWwindow* ptr;
		int scrWidth = 1600;
		int scrHeight = 900;
	};

	// initialize GLFW and GLAD then draw a window
	void initWindow(Window &window)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window.ptr = glfwCreateWindow(window.scrWidth, window.scrHeight, "xengine", NULL, NULL);
		if (window.ptr == NULL)
		{
			std::cout << "Unable to create GLFW window\n";
			glfwTerminate();
		}
		glfwMakeContextCurrent(window.ptr);
		glfwSetFramebufferSizeCallback(window.ptr, [](GLFWwindow* window, int width, int height){ glViewport(0, 0, width, height); });

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Unable to initialize GLAD\n";
		}

		glfwSetInputMode(window.ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		// depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// face culling 
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	// check if glfw has recieved a signal to close
	bool windowShouldClose(Window &window)
	{
		return glfwWindowShouldClose(window.ptr);
	}

	// terminate glfw
	void terminate()
	{
		// other cleanup?
		glfwTerminate();
	}

	// process key presses
	void processEsc(GLFWwindow *window)
	{
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	// swap buffers and poll events
	void swapThenPoll(Window &window)
	{
		glfwSwapBuffers(window.ptr);
		glfwPollEvents();
	}
	
	// fill the window with a single colour
	void fill(float r, float g, float b, float a)
	{
		glClearColor(r, g, b ,a);
	}

	// clear gl buffers
	void clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// calculate projection matrix
	glm::mat4 projectionMatrix(Window &window)
	{
		return glm::perspective(glm::radians(55.0f), (float)window.scrWidth / (float)window.scrHeight, 0.1f, 100.0f);
	}

	// set cursor position callback function
	void setCursorPositionCallback(Window &window, GLFWcursorposfun function)
	{
		glfwSetCursorPosCallback(window.ptr, function);
	}

	// process keyboard input
	void processKeyInput(Window &window, bool &w, bool &a, bool &s, bool &d)
	{
		if (glfwGetKey(window.ptr, GLFW_KEY_W) == GLFW_PRESS) { w = true; };
		if (glfwGetKey(window.ptr, GLFW_KEY_S) == GLFW_PRESS) { s = true; };
		if (glfwGetKey(window.ptr, GLFW_KEY_A) == GLFW_PRESS) { a = true; };
		if (glfwGetKey(window.ptr, GLFW_KEY_D) == GLFW_PRESS) { d = true; };
	}
} // namespace xen

#endif // WINDOW_H
