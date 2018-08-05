#ifndef AWA_GRAPHICSSYSTEM_H
#define AWA_GRAPHICSSYSTEM_H

#include <memory>
#include <vector>
#include "../../GameObject.h"
#include "../../Camera.h"
#include "Shader.h"
#include "GraphicsComponent.h"
#include "../../Level.h"

class GraphicsSystem {
public:
    GraphicsSystem();

    bool initialize(Camera* camera, Level* room);
    bool startGraphics();
    void killGraphics();
    void addEntity(GameObject* gameObject,
                   GeometryFactory::GeometryType primitive,
                   glm::vec3 size,
                   glm::vec4 color);
    void removeEntity(GameObject* gameObject);
    void render(const glm::mat4x4& viewProjectionMatrix);
private:
    int m_numComponents = 0;
    Level* m_level = nullptr;
    Camera* m_camera = nullptr;
    std::vector<std::shared_ptr<GraphicsComponent>> m_components;
    Shader m_shader;
};


#endif //AWA_GRAPHICSSYSTEM_H
