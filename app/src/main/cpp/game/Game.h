#ifndef IRONBALLS_GAME_H
#define IRONBALLS_GAME_H

#include <memory>
#include "../core/input/InputSystem.h"
#include "../core/gameplay/GameObject.h"
#include "../core/Shader.h"
#include "../core/AndroidGame.h"
#include "../core/input/TouchLayer.h"
#include "PxPhysicsAPI.h"

class Game : public AndroidGame {
public:
    Game(struct android_app *app);
    virtual ~Game();
    virtual void update(float deltaTime) override;
    virtual void render() override;

protected:
    virtual void configureOpenGL() override;
    virtual void onResize() override;
private:
    Shader m_shader;
    std::unique_ptr<GameObject> m_ball;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    int m_screenWidth;
    int m_screenHeight;
    bool m_initialized;

    InputSystem m_inputSystem;
    void drawAxis(int x, int y, int z);

    // PhysX
    physx::PxFoundation* m_pxFoundation;
    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;
    physx::PxActor* m_pxGroundPlane;
    physx::PxRigidActor* m_pxBox;
    physx::PxReal m_pxTimestep;

    void initLevel();
    void initPhysX();
    void finalizePhysX();
    void drawActor(physx::PxRigidActor* actor);
    void drawShape(physx::PxShape* shape, physx::PxRigidActor* actor);
    void drawBox(physx::PxShape* shape, physx::PxRigidActor* actor);

    // TODO: create file manager
    char* readAsset(const std::string& path);
};


#endif //IRONBALLS_GAME_H
