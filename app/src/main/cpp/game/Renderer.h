#ifndef AWA_RENDERER_H
#define AWA_RENDERER_H

#include <string>
#include <android/asset_manager.h>
#include "common.h"
#include "../core/Geometry.h"
#include "../core/Shader.h"

class Renderer {
public:
    Renderer(AAssetManager* assetManager);

    void initShaders();

    void renderActor(Geometry* renderTarget, glm::mat4* mvpMatrix, glm::mat4* modelMatrix);
    void renderDebug(Geometry* renderTarget, glm::mat4* mvpMatrix, glm::mat4* modelMatrix);

private:
    AAssetManager* m_assetManager;

    Shader m_actorShader;
    Shader m_debugShader;

    void loadAndCompileShader(Shader* shader, const std::string& name);
    char* readAsset(const std::string& path);
};

#endif //AWA_RENDERER_H
