#include "model.hpp"

#include <glad.h>

/**
 * @brief Create a mesh object
 * 
 * @param v vertices
 * @param i indices
 * @param t textures
 * @return the mesh
 */
void setup_mesh(xen::mesh m) {

    // TODO - aquire lock here for multithreading 
    
    // generate buffers
    glGenVertexArrays(1, &m.VAO_);
    glGenBuffers(1, &m.VBO_);
    glGenBuffers(1, &m.EBO_);

    // bind and buffer data
    glBindVertexArray(m.VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, m.VBO_);
    glBufferData(GL_ARRAY_BUFFER, m.vertices_.size() * sizeof(xen::mesh::vertex), &m.vertices_[0],
        GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices_.size() * sizeof(unsigned int), &m.indices_[0],
        GL_STATIC_DRAW);
    
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(xen::mesh::vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(xen::mesh::vertex),
        (void*)offsetof(xen::mesh::vertex, xen::mesh::vertex::norm));

    // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(xen::mesh::vertex),
        (void*)offsetof(xen::mesh::vertex, xen::mesh::vertex::tex_coords));
    
    // unbind
    glBindVertexArray(0);
}