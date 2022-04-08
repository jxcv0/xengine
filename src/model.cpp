#include "model.hpp"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace xen {

    /**
     * @brief Load a model from a filepath
     * 
     * @param m model
     * @param path path to model file
     */
    Model import_model(const std::string &path) {
        Model model;
        Assimp::Importer importer;
        
        const aiScene *scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace);
        
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
        }


        return model;
    }
}