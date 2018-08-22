#ifndef IRONBALLS_GAME_H
#define IRONBALLS_GAME_H

#include <memory>
#include "systems/graphics/GraphicsSystem.h"
#include "GameObject.h"
#include "../core/AndroidGame.h"
#include "systems/input/TouchLayer.h"
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
    virtual int onInputEvent(AInputEvent* event) override;
protected:
    virtual void onPause() override;
    virtual void onResume() override;
    virtual void startGraphics() override;
    virtual void killGraphics() override;
    virtual void onResize() override;
private:
    static const float BALL_RADIUS;

    bool m_initialized = false;
    std::vector<GameObject*> m_balls;
    int m_screenWidth = 0;
    int m_screenHeight = 0;
    int m_lastFreeId = 0;
    int m_numActiveBalls = 0;

    Level m_level;
    Camera m_camera;

    TouchLayer inputTouchLayer;
    PlayerController m_playerController;
    GraphicsSystem m_graphicsSystem;
    PhysicsSystem m_physicsSystem;

    void initLevel();
};


#endif //IRONBALLS_GAME_H
