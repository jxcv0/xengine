#include "model.hpp"

#include <glad.h>

/**
 * @brief Set up VAO, VBO, and EBO for a mesh
 * 
 * @param m mesh
 */
void xen::setup_mesh(mesh &m) {

    // TODO - aquire lock here? or can these be overwritten on every call? (too slow surely)

    // generate buffers
    glGenVertexArrays(1, &m.VAO);
    glGenBuffers(1, &m.VBO);
    glGenBuffers(1, &m.EBO);

    // bind and buffer data
    glBindVertexArray(m.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
    glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(xen::mesh::vertex), &m.vertices[0],
        GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(unsigned int), &m.indices[0],
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

/**
 * @brief Draw a mesh using a shader program
 * 
 * @param m mesh
 * @param s the shader program
 */
void xen::draw_mesh(const mesh &m, shader &s) {
    unsigned int diffuse_no = 1;
    unsigned int specular_no = 1;

    for (unsigned int i = 0; i < m.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string num;
        std::string name = m.textures[i].type;
        if (name == "texture_diffuse") {
            num = std::to_string(diffuse_no++); 
        } else if (name == "texture_specular") {
            num = std::to_string(specular_no++);
        }
        s.set_float(("material." + name + num).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m.textures[1].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(m.VAO);
    glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}