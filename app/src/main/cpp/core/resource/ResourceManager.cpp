#include "ResourceManager.h"
#include "../common.h"
#include "stdlib.h"
#include "FreeImage.h"

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
        LOGI("Read actor shader failed!");
        return nullptr;
    }

    long length = AAsset_getLength(asset);
    char* buffer = (char*) malloc (length + 1); // +1 for string zero termination
    if (buffer == nullptr) {
        LOGI("Allocation error!");
        return nullptr;
    }
    auto bytesRead = AAsset_read(asset, buffer, length);
    if (bytesRead != length) {
        LOGI("Read asset bytes error!");
        return nullptr;
    }
    // fix the string to be zero-terminated
    buffer[length] = 0;
    AAsset_close(asset);
    return buffer;
}

void ResourceManager::loadImage() {
    AAsset* testAsset = AAssetManager_open(m_assetManager, "pig1.png", AASSET_MODE_UNKNOWN);
    if (testAsset != nullptr) {
        LOGI("ASSET PIG FOUND, TOTAL SIZE:%lli", AAsset_getLength64(testAsset));
        AAsset_close(testAsset);
    }
    else {
        LOGI("ASSET PIG NOT FOUND!");
    }

    FreeImage_Initialise(TRUE);

    //FIBITMAP* bitmap = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_PNG, "../../../assets/pig.png");

    FreeImage_DeInitialise();
}