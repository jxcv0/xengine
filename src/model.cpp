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
            aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
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
        Mesh m;
            // HERE
        return m;
    }


}