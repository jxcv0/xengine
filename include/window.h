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
		int scrWidth = 800;
		int scrHeight = 600;
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
	}

	// check if glfw has recieved a signal to close
	bool windowShouldClose(Window &window)
	{
		return glfwWindowShouldClose(window.ptr);
	}

	// process key presses
	void processKeyPress(GLFWwindow *window)
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
}

#endif // WINDOW_H
