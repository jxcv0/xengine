#include "model.hpp"

namespace xen {

    /**
     * @brief Recursively process aiNodes within an aiScene and store the data in a Model
     * 
     * @param node node
     * @param scene scene
     */
    void Model::process_node(aiNode *node, const aiScene *scene) {

        // process all meshes in this node
        for (size_t i = 0; i < scene->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; // this line segfaults
            meshes.push_back(process_mesh(mesh, scene));
        }

        // process next node
        for (size_t i = 0; i < node->mNumChildren; i++) {
            process_node(node->mChildren[i], scene);
        }
    }

    /**
     * @brief Recursively process meshes after importing
     * 
     * @param mesh imported assimp mesh
     * @param scene imported assimp scene
     * @return Model::Mesh 
     */
    Model::Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
        
        Mesh x_mesh;
        
        // vertices
        for (size_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }

            if (mesh->HasTextureCoords(0)) {
                glm::vec2 tex_vector;
                tex_vector.x = mesh->mTextureCoords[0][i].x;
                tex_vector.y = mesh->mTextureCoords[0][i].y;
                vertex.tex_coords = vector;

                // tanget and bitangent
            } else {
                vertex.tex_coords = glm::vec2(0.0f, 0.0f);
            }
            x_mesh.vertices.push_back(vertex);
        }

        // indices from faces
        for (size_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++) {
                x_mesh.indices.push_back(j);
            }
        }

        // materials
        // aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        return x_mesh;
    }
}