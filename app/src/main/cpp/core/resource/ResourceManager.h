#ifndef AWA_RESOURCEMANAGER_H
#define AWA_RESOURCEMANAGER_H

#include <string>
#include <android/asset_manager.h>

class ResourceManager {
public:
    static ResourceManager& getInstance();

    ResourceManager();
    void connect(AAssetManager* assetManager);
    char* readAsset(const std::string& path);
private:
    AAssetManager* m_assetManager;
};


#endif //AWA_RESOURCEMANAGER_H
