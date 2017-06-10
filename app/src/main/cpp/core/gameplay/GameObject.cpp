#include "GameObject.h"
#include "../../glm/gtx/transform.hpp"

GameObject::GameObject(int id) :
    m_id(id),
    m_geometry(nullptr),
    m_position(0.0f, 0.0f, 0.0f),
    m_rotation(0.0f, 0.0f, 0.0f, 1.0f),
    m_moveDirection(0.0f, 0.0f),
    m_lookDirection(0.0f, 0.0f, 0.0f),
    m_modelMatrix(1.0f),
    m_rotationMatrix(1.0f)
{
}

int GameObject::getId() const{
    return m_id;
}

Geometry* GameObject::getGeometry() const{
    return m_geometry;
}

void GameObject::setGeometry(Geometry* geometry) {
    m_geometry = geometry;
}

const glm::vec3& GameObject::getPosition() const{
    return m_position;
};

void GameObject::setPosition(float x, float y, float z) {
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

const glm::vec4& GameObject::getRotation() const{
    return m_rotation;
}

void GameObject::setRotation(float x, float y, float z) {
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;

    m_rotationMatrix *=  m_rotation;
}

const glm::mat4& GameObject::getModelMatrix() const{
    return m_modelMatrix;
}

const glm::vec2& GameObject::getMoveDirection() const{
    return m_moveDirection;
}

void GameObject::setMoveDirection(float x, float y) {
    m_moveDirection.x = x;
    m_moveDirection.y = y;
}

const glm::vec3& GameObject::getLookDirection() const {
    return m_lookDirection;
}

void GameObject::setLookDirection(float x, float y, float z) {
    m_lookDirection.x = x;
    m_lookDirection.y = y;
    m_lookDirection.z = z;
}

void GameObject::rotate(const glm::vec3 &axis, float angle) {
    m_rotationMatrix = glm::rotate(angle, axis) * m_rotationMatrix;
}

void GameObject::move(float x, float y, float z) {
    m_position.x += x;
    m_position.y += y;
    m_position.z += z;
}

void GameObject::updateModelMatrix() {
    m_modelMatrix = glm::translate(m_position) * m_rotationMatrix;
}