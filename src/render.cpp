#include "render.hpp"
#include "camera.hpp"

/**
 * @brief Initialize the render manager
 * 
 */
void xen::RenderManager::start_up() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1600, 900, "xengine", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow *window, int width, int height){ glViewport(0, 0, width, height); });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/**
 * @brief Set the mouse pos callback
 * 
 * @param callback 
 */
void xen::RenderManager::set_mouse_pos_callback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(window, callback);
}

/**
 * @brief Shut down the render manager
 * 
 */
void xen::RenderManager::shut_down() {
    glfwTerminate();
}

bool xen::RenderManager::should_close() {
    return glfwWindowShouldClose(window);
}

GLFWwindow* xen::RenderManager::window_ptr() {
    return window;
}

void xen::RenderManager::buffer_model(Model &model) {
    // generate buffers
    glGenVertexArrays(1, &model.VAO);
    glGenBuffers(1, &model.VBO);
    glGenBuffers(1, &model.EBO);

    // bind and buffer data
    glBindVertexArray(model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(Model::Vertex), &model.vertices[0],
        GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(unsigned int), &model.indices[0],
        GL_STATIC_DRAW);
    
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex),
        (void*)offsetof(Model::Vertex, Model::Vertex::norm));

    // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex),
        (void*)offsetof(Model::Vertex, Model::Vertex::tex_coords));
    
    // unbind
    glBindVertexArray(0);
}

// TODO - replace model with xen::Renderable?
void xen::RenderManager::draw(Model &model, Shader &shader) {

    // TODO - textures

    glBindVertexArray(model.VAO);
    glDrawElements(
        GL_TRIANGLES, static_cast<unsigned int>(model.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}