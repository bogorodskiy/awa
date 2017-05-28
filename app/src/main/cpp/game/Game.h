#ifndef IRONBALLS_GAME_H
#define IRONBALLS_GAME_H

#include <memory>
#include "../core/input/InputSystem.h"
#include "../core/gameplay/GameObject.h"
#include "../core/Shader.h"
#include "../core/AndroidGame.h"
#include "../core/input/TouchLayer.h"
#include "geometry/Line.h"
#include "geometry/Plane.h"
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

    // debug
    Line m_xAxis;
    Line m_yAxis;
    Line m_zAxis;
    float m_cameraAngle;

    Plane m_floor;
    Plane m_frontWall;
    Plane m_backWall;
    Plane m_leftWall;
    Plane m_rightWall;
    Plane m_ceiling;

    InputSystem m_inputSystem;

    // PhysX
    physx::PxFoundation* m_pxFoundation;
    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;
    physx::PxRigidActor* m_pxGroundPlane;
    physx::PxRigidActor* m_pxBall;
    physx::PxReal m_pxTimestep;

    void initLevel();
    void initPhysX();
    physx::PxRigidActor* createPlane(glm::vec3 position, glm::vec4 quat, physx::PxMaterial* material);
    void finalizePhysX();
    void getColumnMajor(physx::PxMat33 m, physx::PxVec3 t, float* mat);
    void renderPxActor(physx::PxRigidActor* actor, Geometry* geometry);

    // TODO: create file manager
    char* readAsset(const std::string& path);
};


#endif //IRONBALLS_GAME_H
