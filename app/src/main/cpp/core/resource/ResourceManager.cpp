#include "ResourceManager.h"
#include "../common.h"
#include "stdlib.h"

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager()
{
}

void ResourceManager::connect(AAssetManager *assetManager) {
    m_assetManager = assetManager;
}

char* ResourceManager::readAsset(const std::string& path) {
    AAsset* asset = AAssetManager_open(m_assetManager, path.c_str(), AASSET_MODE_BUFFER);

    if (asset == nullptr) {
        LOGE("Read actor shader failed!");
        return nullptr;
    }

    long length = AAsset_getLength(asset);
    char* buffer = (char*) malloc (length + 1); // +1 for string zero termination
    if (buffer == nullptr) {
        LOGE("Allocation error!");
        return nullptr;
    }
    auto bytesRead = AAsset_read(asset, buffer, length);
    if (bytesRead != length) {
        LOGE("Read asset bytes error!");
        return nullptr;
    }
    // fix the string to be zero-terminated
    buffer[length] = 0;
    AAsset_close(asset);
    return buffer;
}