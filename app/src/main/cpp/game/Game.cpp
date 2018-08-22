#include <cstdlib>
#include <exception>
#include "Game.h"
#include "GlobalSettings.h"
#include "geometry/GeometryFactory.h"
#include "../core/resource/ResourceManager.h"

const float Game::BALL_RADIUS = 0.5f;

Game::Game(struct android_app *app) :
        AndroidGame(app),
        m_initialized(false)
{
    m_screenWidth = GlobalSettings::DEFAULT_SCREEN_WIDTH;
    m_screenHeight = GlobalSettings::DEFAULT_SCREEN_WIDTH;

    auto physicsInitialized = m_physicsSystem.initialize(BALL_RADIUS);
    if (!physicsInitialized)
    {
        std::terminate();
    }
    initLevel();
}

Game::~Game()
{
    m_graphicsSystem.reset();
    m_physicsSystem.reset();
    for (auto ball : m_balls) {
        delete ball;
    }
}

void Game::initLevel()
{
    if (m_initialized) {
        return;
    }
    m_initialized = true;

    m_playerController.initialize(&inputTouchLayer, &m_camera);
    m_graphicsSystem.initialize(&m_camera, &m_level);

    // spawn balls in the center of the level
    const auto numBalls = 100;
    const auto ballSize = physx::PxVec3(0.5f, 0.5f, 0.5f);
    for (auto i = 0; i < numBalls; ++i) {
        auto positionX = 0.0f;
        auto positionY = 3.0f + i;
        auto positionZ = (i == 0) ? 3.0f : 0.0f;
        auto ball = new GameObject(i, GlobalSettings::BASE_HP);
        ball->transform.p = physx::PxVec3(positionX, positionY, positionZ);
        ball->transform.q = physx::PxQuat(0.0f, 0.0f, 0.0f, 1.0f);
        m_balls.emplace_back(ball);

        auto randomDelta = (1 + std::rand() % 5) / 10.0f;
        // purple
        auto randomColor = physx::PxVec4(0.47f, 0.26f, 0.5f + randomDelta, 1.0f);

        m_graphicsSystem.addEntity(ball,
                                   Geometry::Type::SPHERE,
                                   ballSize,
                                   randomColor);
        m_physicsSystem.addDynamicEntity(ball);
    }
    auto firstBallComponent = m_physicsSystem.getDynamicComponent(m_balls.front());
    m_playerController.setPawn(firstBallComponent);
    m_physicsSystem.setPlayerId(firstBallComponent->getGameObject()->getId());
    m_lastFreeId = m_level.initializeLevel(m_graphicsSystem, m_physicsSystem, numBalls);
    m_numActiveBalls = numBalls;

    m_camera.setTarget(m_balls.front());
    m_camera.update();
}

void Game::startGraphics() {
    auto graphicsStarted = m_graphicsSystem.startGraphics();
    if (!graphicsStarted)
    {
        LOGE("startGraphics ERROR");
        std::terminate();
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
    m_camera.update();

    for (int i = 0; i < m_numActiveBalls; ++i) {
        auto ball = m_balls[i];
        if (ball->getHealth() == 0) {
            m_graphicsSystem.removeEntity(ball);
            m_physicsSystem.removeDynamicEntity(ball);
            ball->reset();

            std::swap(m_balls[i], m_balls[m_numActiveBalls - 1]);
            --i;
            --m_numActiveBalls;
        }
    }
}

void Game::render() {
    AndroidGame::render();

    const auto& matrix = m_camera.getProjectionViewMatrix();
    m_graphicsSystem.render(matrix);

    for (int i = 0; i < m_numActiveBalls; ++i) {
        m_balls[i]->transformChanged = false;
    }
}

void Game::onResize() {
    AndroidGame::onResize();
    m_camera.setAspectRatio((float)m_surfaceWidth / m_surfaceHeight);
    m_screenHeight = GlobalSettings::DEFAULT_SCREEN_HEIGHT;
    m_screenWidth = m_screenHeight * m_surfaceWidth / m_surfaceHeight;
    inputTouchLayer.updateScreenSize(m_screenWidth, m_screenHeight, m_screenHeight / (float)m_surfaceHeight);
}

int Game::onInputEvent(AInputEvent* event) {
    return inputTouchLayer.processInputEvent(event) ? 1 : 0;
}

void Game::onPause() {
    LOGD("APP_CMD_PAUSE");
}
void Game::onResume() {
    LOGD("APP_CMD_RESUME");
}
