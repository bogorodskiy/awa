#ifndef IRON_BALLS_INPUTSYSTEM_H
#define IRON_BALLS_INPUTSYSTEM_H

#include <memory>
#include "TouchLayer.h"
#include "InputComponent.h"

class InputSystem {
public:
    InputSystem();
    void initialize(TouchLayer *touchLayer);
    void update(float deltaTime);
    void addEntity(GameObject* entity);
    void clearComponent(int index);
    void removeAllEntities();
private:
    static const int MAX_ENTITIES = 100;

    std::unique_ptr<InputComponent[]> m_components;
    int m_freeIndices[MAX_ENTITIES];
    int m_lastFreeIndex;
    int m_numAddedComponents;
    TouchLayer* m_touchLayer;
};


#endif //IRON_BALLS_INPUTSYSTEM_H
