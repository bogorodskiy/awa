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
#include "Room.h"
#include "Camera.h"

class Game : public AndroidGame, public physx::PxSimulationEventCallback {
public:
    Game(struct android_app *app);
    virtual ~Game();
    virtual void update(float dt) override;
    virtual void render() override;

    // PxSimulationEventCallback
    virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
    virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
    virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {};
    virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override {};
    virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override {};
    virtual void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {};

protected:
    virtual void onPause() override;
    virtual void onResume() override;
    virtual void startGraphics() override;
    virtual void killGraphics() override;
    virtual void onResize() override;
private:
    bool m_initialized;
    GameObject* m_ball;
    GameObject* m_ball2;
    int m_screenWidth;
    int m_screenHeight;

    // debug
    Geometry* m_xAxis;
    Geometry* m_yAxis;
    Geometry* m_zAxis;

    // PhysX
    physx::PxFoundation* m_pxFoundation;
    physx::PxPhysics* m_pxPhysics;
    physx::PxScene* m_pxScene;
    physx::PxReal m_pxTimestep;
    physx::PxShape** m_pxShapeBuffer;

    Shader m_shader;
    Room m_room;
    Camera m_camera;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    InputSystem m_inputSystem;

    void initLevel();
    void finalizeLevel();
    void initPhysX();
    void finalizePhysX();
    void getColumnMajor(physx::PxMat33 m, physx::PxVec3 t, float* mat);
    void renderPxActor(physx::PxRigidActor* actor, Geometry* geometry);
    GameObject* createBall(int id, float x, float y, float z);
    bool isPlayer(physx::PxRigidActor* actor);
    bool isPlane(physx::PxRigidActor* actor);
    // TODO: create file manager
    char* readAsset(const std::string& path);
};


#endif //IRONBALLS_GAME_H
