
#ifndef AWA_GRAPHICSCOMPONENT_H
#define AWA_GRAPHICSCOMPONENT_H

#include "../GameObjectComponent.h"
#include "../../geometry/GeometryFactory.h"

class GraphicsComponent: public GameObjectComponent {
public:
    Geometry* geometry;
    glm::vec4 color;

    GraphicsComponent(GameObject* gameObject,
                      GeometryFactory::GeometryType primitive,
                      glm::vec3 size,
                      glm::vec4 color);
    glm::mat4 getModelMatrix();
private:
    Geometry* createGeometry(GeometryFactory::GeometryType primitive, glm::vec3 size);
};


#endif //AWA_GRAPHICSCOMPONENT_H
