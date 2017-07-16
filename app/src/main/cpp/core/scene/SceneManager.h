#ifndef AWA_SCENEMANAGER_H
#define AWA_SCENEMANAGER_H

#include <string>

class SceneManager {
public:
    void activateScene(const std::string& sceneId);
    void pauseCurrentScene();
    void resumeCurrentScene();
    void removeScene(const std::string& sceneId);
};


#endif //AWA_SCENEMANAGER_H
