#ifndef AWA_GRAPHICSSYSTEM_H
#define AWA_GRAPHICSSYSTEM_H

class Level;

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

    bool initialize(Camera* camera, Level* level);
    bool startGraphics();
    void killGraphics();
    void addEntity(GameObject* gameObject,
                   Geometry::Type geometryType,
                   physx::PxVec3 size,
                   physx::PxVec4 color);
    void removeEntity(GameObject* gameObject);
    void render(const physx::PxMat44& viewProjectionMatrix);
    void reset();
private:
    int m_numComponents = 0;
    Geometry::Type m_lastGeometryType = Geometry::Type::NONE;
    Level* m_level = nullptr;
    Camera* m_camera = nullptr;
    std::vector<std::shared_ptr<GraphicsComponent>> m_components;
    Shader m_shader;
};


#endif //AWA_GRAPHICSSYSTEM_H
