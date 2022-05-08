#ifndef WINDOW_H
#define WINDOW_H

#include <glad.h>
#include <GLFW/glfw3.h>

#define INPUT_W 	0x0001 // bit 0
#define INPUT_A 	0x0002 // bit 1
#define INPUT_S 	0x0004 // bit 2
#define INPUT_D 	0x0008 // bit 3
#define INPUT_SPC   0x0010 // bit 4
#define INPUT_SHFT  0x0020 // bit 5
#define INPUT_CTRL  0x0040 // bit 6
// ...

namespace xen
{
	struct Window
	{
        // init glfw and glad
        Window(float width, float height)
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            _width = width;
            _height = height;
            _ptr = glfwCreateWindow(_width, _height, "xengine", NULL, NULL);
            if (!_ptr)
            {
                std::cout << "Unable to create GLFW window\n";
                glfwTerminate();
            }
            glfwMakeContextCurrent(_ptr);
            glfwSetFramebufferSizeCallback(_ptr, [](GLFWwindow* window, int width, int height){ glViewport(0, 0, width, height); });

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cout << "Unable to initialize GLAD\n";
            }

            glfwSetInputMode(_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            
            // depth testing
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            // face culling 
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }

        ~Window()
        {
            this->terminate();
        }

        // check if glfw has recieved a signal to close
        bool should_close()
        {
            return glfwWindowShouldClose(_ptr);
        }

        // terminate glfw
        void terminate()
        {
            // TODO - other cleanup?
            glfwTerminate();
        }

        // swap buffers and poll events
        void swap_and_poll()
        {
            glfwSwapBuffers(_ptr);
            glfwPollEvents();
        }

        // get the input from the window
        short get_input()
        {
            short inputBits = 0x0;

            if(glfwGetKey(_ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(_ptr, true);
            }

            if(glfwGetKey(_ptr, GLFW_KEY_W) == GLFW_PRESS) { inputBits = inputBits | INPUT_W; }
            if(glfwGetKey(_ptr, GLFW_KEY_A) == GLFW_PRESS) { inputBits = inputBits | INPUT_A; }
            if(glfwGetKey(_ptr, GLFW_KEY_S) == GLFW_PRESS) { inputBits = inputBits | INPUT_S; }
            if(glfwGetKey(_ptr, GLFW_KEY_D) == GLFW_PRESS) { inputBits = inputBits | INPUT_D; }

            return inputBits;
        }

        // process keyboard input
        void processKeyInput(bool &w, bool &a, bool &s, bool &d)
        {
            // esc
            if(glfwGetKey(_ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(_ptr, true);
            }

            // w
            if (glfwGetKey(_ptr, GLFW_KEY_W) == GLFW_PRESS) { w = true; };
            if (glfwGetKey(_ptr, GLFW_KEY_W) == GLFW_RELEASE) { w = false; };

            // a
            if (glfwGetKey(_ptr, GLFW_KEY_A) == GLFW_PRESS) { a = true; };
            if (glfwGetKey(_ptr, GLFW_KEY_A) == GLFW_RELEASE) { a = false; };

            // s
            if (glfwGetKey(_ptr, GLFW_KEY_S) == GLFW_PRESS) { s = true; };
            if (glfwGetKey(_ptr, GLFW_KEY_S) == GLFW_RELEASE) { s = false; };

            // d
            if (glfwGetKey(_ptr, GLFW_KEY_D) == GLFW_PRESS) { d = true; };
            if (glfwGetKey(_ptr, GLFW_KEY_D) == GLFW_RELEASE) { d = false; };

            // spc
            // shft
            // ctrl
        }

        // fill the window with a single colour and clear
        void set_bg(float r, float g, float b, float a)
        {
            glClearColor(r, g, b ,a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // clear bitbuffers
        void clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // calculate projection matrix
        glm::mat4 projection_matrix(float fov)
        {
            return glm::perspective(glm::radians(fov), (float)_width / (float)_height, 0.1f, 100.0f);
        }

        // set cursor position callback function
        template<typename Function>
        void set_cursor_position_callback(Function function)
        {
            glfwSetCursorPosCallback(_ptr, function);
        }

    private:
		GLFWwindow* _ptr;
		float _width;
		float _height;
	};
} // namespace xen

#endif // WINDOW_H
