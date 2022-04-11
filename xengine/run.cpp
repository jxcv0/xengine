#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "window.h"

int main(int argc, char const *argv[]) {
	xen::Window globalWindow;	// extern from window.h
	xen::initWindow(globalWindow);

	while (!xen::windowShouldClose(globalWindow))
	{
		xen::processKeyPress(globalWindow.ptr);
		glfwSwapBuffers(globalWindow.ptr);
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}
