#ifndef IRONBALLS_GAME_H
#define IRONBALLS_GAME_H

#include <memory>
#include "systems/graphics/GraphicsSystem.h"
#include "GameObject.h"
#include "systems/graphics/Shader.h"
#include "../core/AndroidGame.h"
#include "systems/input/TouchLayer.h"
#include "geometry/Line.h"
#include "geometry/Plane.h"
#include "PxPhysicsAPI.h"
#include "Level.h"
#include "Camera.h"
#include "systems/physics/PhysicsSystem.h"
#include "PlayerController.h"

class Game : public AndroidGame {
public:
    Game(struct android_app *app);
    virtual ~Game();
    virtual void update(float dt) override;
    virtual void render() override;

protected:
    virtual void onPause() override;
    virtual void onResume() override;
    virtual void startGraphics() override;
    virtual void killGraphics() override;
    virtual void onResize() override;
private:
    bool m_initialized = false;
    std::vector<GameObject*> m_balls;
    int m_screenWidth;
    int m_screenHeight;

    Level m_level;
    Camera m_camera;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    PlayerController m_playerController;
    GraphicsSystem m_graphicsSystem;
    PhysicsSystem m_physicsSystem;

    void initLevel();
    void finalizeLevel();
};


#endif //IRONBALLS_GAME_H
