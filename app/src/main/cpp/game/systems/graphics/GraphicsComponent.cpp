
#include "GraphicsComponent.h"
#include <PxMat44.h>

GraphicsComponent::GraphicsComponent(GameObject* gameObject,
                                     GeometryFactory::GeometryType primitive,
                                     glm::vec3 size,
                                     glm::vec4 color) : GameObjectComponent(gameObject) {
    geometry = createGeometry(primitive, size);
    this->color = color;
}

glm::mat4 GraphicsComponent::getModelMatrix() {
    auto transform = m_gameObject->transform;
    glm::mat4 modelMatrix = glm::make_mat4(physx::PxMat44(transform.q).front());
    modelMatrix[3][0] = transform.p[0];
    modelMatrix[3][1] = transform.p[1];
    modelMatrix[3][2] = transform.p[2];
    modelMatrix[3][3] = 1;
    return modelMatrix;
}

Geometry* GraphicsComponent::createGeometry(GeometryFactory::GeometryType primitive, glm::vec3 size)
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