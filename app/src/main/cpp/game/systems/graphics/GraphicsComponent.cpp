
#include "GraphicsComponent.h"
#include "PxMat44.h"

const physx::PxMat44& GraphicsComponent::getModelMatrix() const {
    return m_modelMatrix;
}

void GraphicsComponent::fillWith(GameObject* gameObject,
                                 Geometry::Type geometryType,
                                 physx::PxVec3 size,
                                 physx::PxVec4 color) {
    m_gameObject = gameObject;
    geometry = createGeometry(geometryType, size);
    this->color = color;
}

void GraphicsComponent::fillWith(const GraphicsComponent& source) {
    geometry = source.geometry;
    m_gameObject = source.m_gameObject;
}

void GraphicsComponent::preRender() {
    if (m_gameObject->transformChanged) {
        m_modelMatrix = physx::PxMat44(m_gameObject->transform);
    }
}

Geometry* GraphicsComponent::createGeometry(Geometry::Type geometryType, physx::PxVec3 size)
{
    Geometry* result = nullptr;
    switch(geometryType)
    {
        case Geometry::Type::PLANE:
        {
            result = GeometryFactory::getInstance()->getPlane(size.x, size.y);
            break;
        }
        case Geometry::Type::SPHERE:
        {
            result = GeometryFactory::getInstance()->getSphere(size.x);
            break;
        }
    }
    return result;
}

void GraphicsComponent::reset() {
    geometry = nullptr;
    m_gameObject = nullptr;
}