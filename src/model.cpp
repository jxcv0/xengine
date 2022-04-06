#include "model.hpp"

#include <glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

/**
 * @brief Set up VAO, VBO, and EBO for a mesh
 * 
 * @param m mesh
 */
void xen::setup_mesh(Mesh &m) {

    // TODO - aquire lock here? or can these be overwritten on every call? (too slow surely)

    // generate buffers
    glGenVertexArrays(1, &m.VAO);
    glGenBuffers(1, &m.VBO);
    glGenBuffers(1, &m.EBO);

    // bind and buffer data
    glBindVertexArray(m.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
    glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(xen::Mesh::Vertex), &m.vertices[0],
        GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(unsigned int), &m.indices[0],
        GL_STATIC_DRAW);
    
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(xen::Mesh::Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(xen::Mesh::Vertex),
        (void*)offsetof(xen::Mesh::Vertex, xen::Mesh::Vertex::norm));

    // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(xen::Mesh::Vertex),
        (void*)offsetof(xen::Mesh::Vertex, xen::Mesh::Vertex::tex_coords));
    
    // unbind
    glBindVertexArray(0);
}

/**
 * @brief Draw a mesh using a shader program
 * 
 * @param m mesh
 * @param s the shader program
 */
void xen::draw_mesh(const Mesh &m, Shader &s) {
    unsigned int diffuse_no = 1;
    unsigned int specular_no = 1;
    unsigned int normal_no = 1;
    unsigned int height_no = 1;

    for (unsigned int i = 0; i < m.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string num;
        std::string name = m.textures[i].type;
        if (name == "texture_diffuse") {
            num = std::to_string(diffuse_no++); 
        } else if (name == "texture_specular") {
            num = std::to_string(specular_no++);
        } else if (name == "texture_normal") {
            num = std::to_string(normal_no++);
        } else if (name == "texture_height") {
            num = std::to_string(height_no++);
        }
        s.set_float(("material." + name + num).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m.textures[i].id);
    }

    glBindVertexArray(m.VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m.indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

/**
 * @brief Load a model from a filepath
 * 
 * @param m model
 * @param path path to model file
 */
void xen::load_model(Model &m, const std::string &path) {

    Assimp::Importer imp;
    const aiScene *scene = imp.ReadFile(path,
          aiProcess_Triangulate
        | aiProcess_GenSmoothNormals
        | aiProcess_FlipUVs
        | aiProcess_CalcTangentSpace);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << imp.GetErrorString() << std::endl;
        return;
    }

    m.dir = path.substr(0, path.find_last_of('/'));

    process_node(m, scene->mRootNode, scene);
}

void process_node(xen::Model &m, aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m.meshes.push_back(process_mesh(m, mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; i++) {
        process_node(m, node->mChildren[i], scene);
    }
}

xen::Mesh process_mesh(xen::Model &m, aiMesh *mesh, const aiScene *scene) {

    std::vector<xen::Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<xen::Mesh::Texture> textures;

    // vertices
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        xen::Mesh::Vertex vert;
        glm::vec3 vec3;

        vec3.x = mesh->mVertices[i].x;
        vec3.y = mesh->mVertices[i].y;
        vec3.z = mesh->mVertices[i].z;
        vert.pos = vec3;

        if (mesh->HasNormals()) {
            vec3.x = mesh->mNormals[i].x;
            vec3.y = mesh->mNormals[i].y;
            vec3.z = mesh->mNormals[i].z;
            vert.norm = vec3;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec2;

            // assuming that vertices have only one set of vertex coords
            vec2.x = mesh->mTextureCoords[0][i].x;
            vec2.y = mesh->mTextureCoords[0][i].y;
            vert.tex_coords = vec2;

            // tangent?
            // bitangent?

        } else {
            vert.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vert);
    }
    
    // faces
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    // materials - assuming naming convention
    aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

    // diffuse
    std::vector<xen::Mesh::Texture> diffuseMaps =
        load_mat_tex(m, mat, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // specular
    std::vector<xen::Mesh::Texture> specularMaps =
        load_mat_tex(m, mat, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // normal
    std::vector<xen::Mesh::Texture> normalMaps = load_mat_tex(m, mat, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // height
    std::vector<xen::Mesh::Texture> heightMaps = load_mat_tex(m, mat, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    xen::Mesh x_mesh;
    x_mesh.vertices = vertices;
    x_mesh.indices = indices;
    x_mesh.textures = textures;
    return x_mesh;
}

std::vector<xen::Mesh::Texture> load_mat_tex(
    xen::Model &m,
    aiMaterial *mat,
    aiTextureType type,
    std::string type_name
) {
    std::vector<xen::Mesh::Texture> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for (size_t j = 0; m.loaded_textures.size(); i++) {
            if (std::strcmp(m.loaded_textures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(m.loaded_textures[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            xen::Mesh::Texture texture;
            texture.id = tex_from_file(str.C_Str(), m.dir);
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m.loaded_textures.push_back(texture);
        }
        return textures;   
    }
}

unsigned int tex_from_file(const char *path, const std::string &dir) {
    std::string filename(path);
    filename = dir + '/' + filename;

    unsigned int tex_id;
    glGenTextures(1, &tex_id);
    
    int w, h, no;
    unsigned char *data = stbi_load(filename.c_str(), &w, &h, &no, 0);

    if (data) {
        GLenum format;
        if (no == 1) {
            format = GL_RED;
        } else if (no == 3) {
            format = GL_RGB;
        } else if (no = 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Unable to load texture from " << path << std::endl;
        stbi_image_free(data);
    }

    return tex_id;
}

/**
 * @brief Draw a model using a shader program
 * 
 * @param m mesh
 * @param s shader program
 */
void xen::draw_model(const Shader &s) {

}