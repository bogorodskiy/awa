
#ifndef AWA_GRAPHICSCOMPONENT_H
#define AWA_GRAPHICSCOMPONENT_H

#include "foundation/PxVec4.h"
#include "foundation/PxMat44.h"
#include "../GameObjectComponent.h"
#include "../../geometry/GeometryFactory.h"

class GraphicsComponent: public GameObjectComponent {
public:
    Geometry* geometry;
    physx::PxVec4 color;

    GraphicsComponent(GameObject* gameObject,
                      GeometryFactory::GeometryType primitive,
                      physx::PxVec3 size,
                      physx::PxVec4 color);
    physx::PxMat44 getModelMatrix();
private:
    Geometry* createGeometry(GeometryFactory::GeometryType primitive, physx::PxVec3 size);
};


#endif //AWA_GRAPHICSCOMPONENT_H
