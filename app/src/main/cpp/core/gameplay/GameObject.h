#ifndef IRON_BALLS_GAMEOBJECT_H
#define IRON_BALLS_GAMEOBJECT_H

#include <memory>
#include "../Geometry.h"

class GameObject {
public:
    GameObject(int id);

    int getId() const;
    Geometry* getGeometry() const;
    void setGeometry(std::unique_ptr<Geometry> geometry);
    const glm::vec3& getPosition() const;
    void setPosition(float x, float y, float z);
    const glm::vec4& getRotation() const;
    void setRotation(float x, float y, float z);
    void rotate(const glm::vec3& axis, float angle);
    void move(float x, float y, float z);
    const glm::mat4& getModelMatrix() const;
    const glm::vec2& getMoveDirection() const;
    void setMoveDirection(float x, float y);
    const glm::vec3& getLookDirection() const;
    void setLookDirection(float x, float y, float z);
    void updateModelMatrix();
private:
    unsigned int m_id;
    std::unique_ptr<Geometry> m_geometry;
    glm::vec3 m_position;
    glm::vec4 m_rotation;
    glm::vec2 m_moveDirection;
    glm::vec3 m_lookDirection;
    glm::mat4 m_modelMatrix;
    glm::mat4 m_rotationMatrix;
};


#endif //IRON_BALLS_GAMEOBJECT_H
