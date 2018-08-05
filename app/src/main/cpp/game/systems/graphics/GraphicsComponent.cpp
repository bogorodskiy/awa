
#include "GraphicsComponent.h"
#include <PxMat44.h>

GraphicsComponent::GraphicsComponent(GameObject* gameObject,
                                     GeometryFactory::GeometryType primitive,
                                     physx::PxVec3 size,
                                     physx::PxVec4 color) : GameObjectComponent(gameObject) {
    geometry = createGeometry(primitive, size);
    this->color = color;
}

physx::PxMat44 GraphicsComponent::getModelMatrix() {
    return physx::PxMat44(m_gameObject->transform);
}

Geometry* GraphicsComponent::createGeometry(GeometryFactory::GeometryType primitive, physx::PxVec3 size)
{
    Geometry* result = nullptr;
    switch(primitive)
    {
        case GeometryFactory::GeometryType::PLANE:
        {
            result = GeometryFactory::getInstance()->getPlane(size.x, size.y);
            break;
        }
        case GeometryFactory::GeometryType::SPHERE:
        {
            result = GeometryFactory::getInstance()->getSphere(size.x);
            break;
        }
    }
    return result;
}