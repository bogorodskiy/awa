
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

    const physx::PxMat44& getModelMatrix() const;
    void fillWith(GameObject* gameObject,
                  Geometry::Type geometryType,
                  physx::PxVec3 size,
                  physx::PxVec4 color);
    void fillWith(const GraphicsComponent& source);
    void preRender();
    void reset();
private:
    physx::PxMat44 m_modelMatrix;

    Geometry* createGeometry(Geometry::Type geometryType, physx::PxVec3 size);
};


#endif //AWA_GRAPHICSCOMPONENT_H
