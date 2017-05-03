#include "Renderer.h"

Renderer::Renderer(AAssetManager* assetManager):m_assetManager(assetManager) {

}

void Renderer::initShaders() {
    loadAndCompileShader(&m_actorShader, "actor");
    loadAndCompileShader(&m_debugShader, "debug");
}

void Renderer::renderActor(Geometry* renderTarget, glm::mat4* mvpMatrix, glm::mat4* modelMatrix) {

}

void Renderer::renderDebug(Geometry* renderTarget, glm::mat4* mvpMatrix, glm::mat4* modelMatrix) {
    renderTarget->bindVertexBuffer();

    renderTarget->unbindVertexBuffer();
}

void Renderer::loadAndCompileShader(Shader* shader, const std::string& name) {
    char* vertexShader = readAsset("shaders/" + name + ".vert");
    char* fragmentShader = readAsset("shaders/" + name + ".frag");
    auto loaded = true;

    if (vertexShader == nullptr) {
        LOGI("Load actor shader failed!");
        loaded = false;
    }
    if (fragmentShader == nullptr) {
        LOGI("Load actor shader failed!");
        loaded = false;

        free(vertexShader);
    }

    if (loaded) {
        shader->compile(vertexShader, fragmentShader);
        free(vertexShader);
        free(fragmentShader);
    }
}

char* Renderer::readAsset(const std::string& path) {
    AAsset* asset = AAssetManager_open(m_assetManager, path.c_str(), AASSET_MODE_BUFFER);

    if (asset == nullptr) {
        LOGI("Read actor shader failed!");
        return nullptr;
    }

    long length = AAsset_getLength(asset);
    char* buffer = (char*) malloc (length + 1); // +1 for string zero termination
    if (buffer == nullptr) {
        LOGI("Allocation error!");
        return nullptr;
    }
    auto bytesRead = AAsset_read (asset, buffer, length);
    if (bytesRead != length) {
        LOGI("Read asset bytes error!");
        return nullptr;
    }
    // fix the string to be zero-terminated
    buffer[length] = 0;
    AAsset_close(asset);
    return buffer;
}