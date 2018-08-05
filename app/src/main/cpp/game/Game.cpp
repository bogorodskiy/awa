#include <cstdlib>
#include "Game.h"
#include "GlobalSettings.h"
#include "geometry/GeometryFactory.h"
#include "../core/resource/ResourceManager.h"

Game::Game(struct android_app *app) :
        AndroidGame(app),
        m_initialized(false)
{
    m_screenWidth = GlobalSettings::DEFAULT_SCREEN_WIDTH;
    m_screenHeight = GlobalSettings::DEFAULT_SCREEN_WIDTH;

    auto physicsInitialized = m_physicsSystem.initialize();
    if (!physicsInitialized)
    {
        // TODO: abort game
    }
    initLevel();
}

Game::~Game()
{
    finalizeLevel();
    // TODO: clear systems
}

void Game::initLevel()
{
    if (m_initialized) {
        return;
    }

    m_initialized = true;

    m_playerController.initialize(&inputTouchLayer, &m_camera);
    m_graphicsSystem.initialize(&m_camera, &m_level);

    // TODO from level config
    auto numBalls = 11;
    for (int i = 0; i < numBalls; ++i) {
        auto positionX = 0.0f;
        auto positionY = 3.0f + i;
        auto positionZ = 2.0f;
        auto ball = new GameObject(i);
        ball->transform.p = physx::PxVec3(positionX, positionY, positionZ);
        ball->transform.q = physx::PxQuat(0.0f,0.0f, 0.0f, 1.0f);
        m_balls.emplace_back(ball);

        auto size = physx::PxVec3(0.5f, 0.5f, 0.5f);
        auto randomDelta = (1 + std::rand() % 5) / 10.0f;
        // purple
        auto randomColor = physx::PxVec4(0.47f, 0.26f, 0.5f + randomDelta, 1.0f);

        m_graphicsSystem.addEntity(ball,
                                   GeometryFactory::GeometryType::SPHERE,
                                   size,
                                   randomColor);
        m_physicsSystem.addDynamicEntity(ball,
                                         GeometryFactory::GeometryType::SPHERE,
                                         size);
    }
    auto firstBallComponent = m_physicsSystem.getDynamicComponent(m_balls.front());
    m_playerController.setPawn(firstBallComponent);

    const auto levelWidth = 20.0f;
    const auto levelHeight = 10.0f;
    const auto levelDepth = 30.0f;
    m_level.initialize(levelWidth, levelHeight, levelDepth);
    auto halfWidth = levelWidth * 0.5f;
    auto halfHeight = levelHeight * 0.5f;
    auto halfDepth = levelDepth * 0.5f;
    auto lastFreeId = numBalls;
    auto whiteColor = physx::PxVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // floor
    auto plane = new GameObject(lastFreeId++);
    auto size = physx::PxVec3(levelDepth, levelWidth, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, 0.0f, 0.0f);
    plane->transform.q = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    m_graphicsSystem.addEntity(plane,
                               GeometryFactory::GeometryType::PLANE,
                               size,
                               whiteColor);
    m_physicsSystem.addStaticEntity(plane,
                                    GeometryFactory::GeometryType::PLANE,
                                    size);

    // frontWall
    plane = new GameObject(lastFreeId++);
    size = physx::PxVec3(levelWidth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, halfHeight, halfDepth);
    plane->transform.q = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    m_graphicsSystem.addEntity(plane,
                               GeometryFactory::GeometryType::PLANE,
                               size,
                               whiteColor);
    m_physicsSystem.addStaticEntity(plane,
                                    GeometryFactory::GeometryType::PLANE,
                                    size);

    // backWall
    plane = new GameObject(lastFreeId++);
    size = physx::PxVec3(levelWidth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, halfHeight, -halfDepth);
    plane->transform.q = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    m_graphicsSystem.addEntity(plane,
                               GeometryFactory::GeometryType::PLANE,
                               size,
                               whiteColor);
    m_physicsSystem.addStaticEntity(plane,
                                    GeometryFactory::GeometryType::PLANE,
                                    size);

    // leftWall
    plane = new GameObject(lastFreeId++);
    size = physx::PxVec3(levelDepth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(-halfWidth, halfHeight, 0.0f);
    plane->transform.q = physx::PxQuat(0, physx::PxVec3(0.0f, 0.0f, 0.0f));
    m_graphicsSystem.addEntity(plane,
                               GeometryFactory::GeometryType::PLANE,
                               size,
                               whiteColor);
    m_physicsSystem.addStaticEntity(plane,
                                    GeometryFactory::GeometryType::PLANE,
                                    size);

    // rightWall
    plane = new GameObject(lastFreeId++);
    size = physx::PxVec3(levelDepth, levelHeight, 0.0f);
    plane->transform.p = physx::PxVec3(halfWidth, halfHeight, 0.0f);
    plane->transform.q = physx::PxQuat(physx::PxPi, physx::PxVec3(0.0f, 1.0f, 0.0f));
    m_graphicsSystem.addEntity(plane,
                               GeometryFactory::GeometryType::PLANE,
                               size,
                               whiteColor);
    m_physicsSystem.addStaticEntity(plane,
                                    GeometryFactory::GeometryType::PLANE,
                                    size);

    // ceiling
    plane = new GameObject(lastFreeId);
    size = physx::PxVec3(levelDepth, levelWidth, 0.0f);
    plane->transform.p = physx::PxVec3(0.0f, levelHeight, 0.0f);
    plane->transform.q = physx::PxQuat(physx::PxPi + physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
    m_graphicsSystem.addEntity(plane,
                               GeometryFactory::GeometryType::PLANE,
                               size,
                               whiteColor);
    m_physicsSystem.addStaticEntity(plane,
                                    GeometryFactory::GeometryType::PLANE,
                                    size);

    m_camera.setPosition(0.0f, 5.0f, 0.0f);
    m_camera.setUpVector(0.0f, 1.0f, 0.0f);
    m_camera.setDirection(0.0f, -1.0f, -20.0f);
    m_camera.setTarget(m_balls.front());
    auto roomBounds = m_level.getBounds();
    m_camera.setRoomBounds(-roomBounds.x * 0.5f, roomBounds.x * 0.5f,
                           -roomBounds.y * 0.5f, roomBounds.y * 0.5f,
                           -roomBounds.z * 0.5f, roomBounds.z * 0.5f);
}

void Game::finalizeLevel() {
    for (auto ball : m_balls) {
        delete ball;
    }
}

void Game::startGraphics() {
    auto graphicsStarted = m_graphicsSystem.startGraphics();
    if (!graphicsStarted)
    {
        LOGE("startGraphics ERROR");
        // TODO: abort game
    }
    GeometryFactory::getInstance()->connect();
}

void Game::killGraphics() {
    GeometryFactory::getInstance()->disconnect();
    m_graphicsSystem.killGraphics();
}

void Game::update(float dt) {
    m_playerController.update();
    m_physicsSystem.update(dt);
    m_camera.update(dt);
}

void Game::render() {
    AndroidGame::render();

    const auto matrix = m_camera.getProjectionViewMatrix();
    m_graphicsSystem.render(matrix);
}

void Game::onResize() {
    AndroidGame::onResize();
    m_camera.setAspectRatio((float)m_surfaceWidth / m_surfaceHeight);
    m_screenHeight = GlobalSettings::DEFAULT_SCREEN_HEIGHT;
    m_screenWidth = m_screenHeight * m_surfaceWidth / m_surfaceHeight;
    inputTouchLayer.updateScreenSize(m_screenWidth, m_screenHeight, m_screenHeight / (float)m_surfaceHeight);
}

void Game::onPause() {
    // TODO: handle
    LOGD("APP_CMD_PAUSE");
}
void Game::onResume() {
    // TODO: handle
    LOGD("APP_CMD_RESUME");
}
