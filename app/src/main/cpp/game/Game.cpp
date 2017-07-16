#include "Game.h"
#include "geometry/GeometryCache.h"
#include "../core/resource/ResourceManager.h"

Game::Game(struct android_app *app) :
        AndroidGame(app),
        m_initialized(false),
        m_ball(nullptr),
        m_screenWidth(1920),
        m_screenHeight(1080),
        m_xAxis(nullptr),
        m_yAxis(nullptr),
        m_zAxis(nullptr),
        m_pxFoundation(nullptr),
        m_pxPhysics(nullptr),
        m_pxScene(nullptr),
        m_pxTimestep(1.0f/60.0f),
        m_pxShapeBuffer(nullptr),
        m_ball2(nullptr)
{
    initPhysX();
    initLevel();
}

Game::~Game()
{
    finalizeLevel();
    finalizePhysX();
    m_inputSystem.removeAllEntities();
}

// PHYSX CONTACT FILTER

physx::PxFilterFlags GameContactReportFilterShader(
        physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
        physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
        physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
    pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
                | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
                //| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
                | physx::PxPairFlag::eNOTIFY_TOUCH_LOST
                | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;

    return physx::PxFilterFlag::eDEFAULT;
}

void Game::initPhysX() {
    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    static physx::PxDefaultAllocator gDefaultAllocatorCallback;
    static physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;

    m_pxFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback,
                                        gDefaultErrorCallback);
    if (!m_pxFoundation) {
        LOGI("PxCreateFoundation failed!");
        return;
    }
    LOGI("PxCreateFoundation success!");

    m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, physx::PxTolerancesScale() );
    if (!m_pxPhysics) {
        LOGI("PxCreatePhysics failed!");
        return;
    }
    LOGI("PxCreatePhysics success!");

    physx::PxSceneDesc sceneDescription(m_pxPhysics->getTolerancesScale());
    if (!sceneDescription.cpuDispatcher) {
        physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
        if(!mCpuDispatcher) {
            LOGI("PxDefaultCpuDispatcherCreate failed!");
            return;
        }
        sceneDescription.cpuDispatcher = mCpuDispatcher;
    }
    if (!sceneDescription.filterShader) {
        sceneDescription.filterShader = gDefaultFilterShader;
    }
    sceneDescription.simulationEventCallback = this;
    sceneDescription.filterShader = GameContactReportFilterShader;
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_PCM;
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
    sceneDescription.flags |= physx::PxSceneFlag::eSUPPRESS_EAGER_SCENE_QUERY_REFIT;
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_KINEMATIC_PAIRS;

    m_pxScene = m_pxPhysics->createScene(sceneDescription);
    if (!m_pxScene) {
        LOGI("createScene failed!");
        return;
    }
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0);
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0);

    m_room.connect(m_pxPhysics, m_pxScene);

    m_pxShapeBuffer = new physx::PxShape*[1];
}

void Game::finalizePhysX() {
    m_pxPhysics->release();
    m_pxPhysics = nullptr;

    m_pxFoundation->release();
    m_pxFoundation = nullptr;

    delete [] m_pxShapeBuffer;
    m_pxShapeBuffer = nullptr;
}

void Game::initLevel()
{
    if (m_initialized) {
        return;
    }

    m_initialized = true;

    m_ball = createBall(0, 0.0f, 3.0f, 0.0f);

    // TODO: remove test
    m_ball2 = createBall(1, 3.0f, 4.0f, 3.0f);

    m_inputSystem.initialize(&inputTouchLayer, &m_camera);
    m_inputSystem.addEntity(m_ball);

    m_xAxis = GeometryCache::getInstance()->getLine(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f));
    m_xAxis->setColor(1.0f, 0.0f, 0.0f, 1.0f);

    m_yAxis = GeometryCache::getInstance()->getLine(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    m_yAxis->setColor(0.0f, 1.0f, 0.0f, 1.0f);

    m_zAxis = GeometryCache::getInstance()->getLine(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 10.0f));
    m_zAxis->setColor(0.0f, 0.0f, 1.0f, 1.0f);

    m_room.initialize(20.0f, 10.0f, 30.0f);

    m_camera.setPosition(0.0f, 5.0f, 20.0f);
    m_camera.setUpVector(0.0f, 1.0f, 0.0f);
    m_camera.setDirection(0.0f, -1.0f, -20.0f);
    m_camera.setTarget(m_ball);
    auto roomBounds = m_room.getBounds();
    m_camera.setRoomBounds(-roomBounds.x * 0.5f, roomBounds.x * 0.5f,
                           -roomBounds.y * 0.5f, roomBounds.y * 0.5f,
                           -roomBounds.z * 0.5f, roomBounds.z * 0.5f);
}

void Game::finalizeLevel() {
    delete m_ball;
    delete m_ball2;
}

GameObject* Game::createBall(int id, float x, float y, float z) {
    physx::PxMaterial* material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);

    // Create sphere
    physx::PxReal density = 1.0f;
    physx::PxQuat quat;
    quat.x = 0.0f;
    quat.y = 0.0f;
    quat.z = 0.0f;
    quat.w = 1.0f;
    physx::PxTransform transform(physx::PxVec3(x, y, z), quat);
    physx::PxSphereGeometry pxGeometry(0.5);
    physx::PxRigidDynamic* actor = PxCreateDynamic(*m_pxPhysics, transform, pxGeometry, *material, density);
    // damping - friction or resistance, angular = rotation/spinning, linear = moving
    actor->setAngularDamping(0.5);
    actor->setLinearDamping(0.5);
    actor->setLinearVelocity(physx::PxVec3(0,0,0));
    if (!actor) {
        LOGI("create actor failed!");
    }
    m_pxScene->addActor(*actor);

    auto geometry = GeometryCache::getInstance()->getSphere();
    geometry->setPrimitive(GL_LINES);
    geometry->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    auto result = new GameObject(id, geometry, actor, m_pxShapeBuffer);
    return result;
}

void Game::startGraphics() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto& resourceManager = ResourceManager::getInstance();

    char* vertexShader = resourceManager.readAsset("shaders/actor.vert");
    char* fragmentShader = resourceManager.readAsset("shaders/actor.frag");

    if (vertexShader == nullptr || fragmentShader == nullptr) {
        LOGI("Load shader failed!");
    }
    m_shader.reset();
    auto compileResult = m_shader.compile(vertexShader, fragmentShader);
    free(vertexShader);
    free(fragmentShader);
    if (!compileResult) {
        // TODO: abort game
    }
    auto initResult = m_shader.init();
    if (!initResult) {
        // TODO: abort game
    }

    GeometryCache::getInstance()->connect();
}

void Game::killGraphics() {
    GeometryCache::getInstance()->disconnect();
    m_shader.reset();
}

void Game::getColumnMajor(physx::PxMat33 m, physx::PxVec3 t, float* mat) {
    mat[0] = m.column0[0];
    mat[1] = m.column0[1];
    mat[2] = m.column0[2];
    mat[3] = 0;

    mat[4] = m.column1[0];
    mat[5] = m.column1[1];
    mat[6] = m.column1[2];
    mat[7] = 0;

    mat[8] = m.column2[0];
    mat[9] = m.column2[1];
    mat[10] = m.column2[2];
    mat[11] = 0;

    mat[12] = t[0];
    mat[13] = t[1];
    mat[14] = t[2];
    mat[15] = 1;
}

void Game::update(float dt) {
    m_inputSystem.update(dt);

    m_ball->applyGravity();
    m_ball2->applyGravity();

    m_pxScene->simulate(dt);

    // TODO: measure execution time
    // wait for simulation to finish(separate thread)
    while (!m_pxScene->fetchResults()) {
    }

    m_camera.update(dt);
}

void Game::render() {
    AndroidGame::render();

    const auto& camPositionVector = m_camera.getPosition();
    const auto& camUpVector = m_camera.getUpVector();
    const auto& camDirectionVector = m_camera.getDirection();

    m_viewMatrix = glm::lookAt(glm::vec3{camPositionVector.x, camPositionVector.y, camPositionVector.z},
                               glm::vec3{camDirectionVector.x, camDirectionVector.y, camDirectionVector.z},
                               glm::vec3{camUpVector.x, camUpVector.y, camUpVector.z});

    m_shader.bind();

    renderPxActor(m_ball->getActor(), m_ball->getGeometry());
    renderPxActor(m_ball2->getActor(), m_ball2->getGeometry());
    for (auto& elementPair : m_room.getElements()) {
        renderPxActor(elementPair.second, elementPair.first);
    }

    // debug
    auto identityMatrix = glm::mat4(1.0);
    auto mvpMatrix = m_projectionMatrix * m_viewMatrix;
    m_shader.beginRender(m_xAxis);
    m_shader.render(&mvpMatrix, &identityMatrix);
    m_shader.endRender();

    m_shader.beginRender(m_yAxis);
    m_shader.render(&mvpMatrix, &identityMatrix);
    m_shader.endRender();

    m_shader.beginRender(m_zAxis);
    m_shader.render(&mvpMatrix, &identityMatrix);
    m_shader.endRender();

    m_shader.unbind();
}

void Game::renderPxActor(physx::PxRigidActor* actor, Geometry* geometry) {
    physx::PxU32 nShapes = actor->getNbShapes();
    if (nShapes > 1) {
        LOGI("ERROR: Actor has more than one shape");
        return;
    }

    actor->getShapes(m_pxShapeBuffer, nShapes);
    auto shape = m_pxShapeBuffer[0];
    physx::PxTransform transform = physx::PxShapeExt::getGlobalPose(*shape, *actor);
    physx::PxMat33 mat3 = physx::PxMat33(transform.q);
    float mat4[16];
    getColumnMajor(mat3, transform.p, mat4);

    auto modelMatrix = glm::make_mat4(mat4);
    auto mvpMatrix = m_projectionMatrix * m_viewMatrix * modelMatrix;

    m_shader.beginRender(geometry);
    m_shader.render(&mvpMatrix, &modelMatrix);
    m_shader.endRender();
}

void Game::onResize() {
    AndroidGame::onResize();
    float aspectRatio = (float)m_surfaceWidth / m_surfaceHeight;
    m_projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.01f, 50.0f);
    m_screenHeight = 1080;
    m_screenWidth = m_screenHeight * m_surfaceWidth / m_surfaceHeight;
    inputTouchLayer.updateScreenSize(m_screenWidth, m_screenHeight, m_screenHeight / (float)m_surfaceHeight);
}

// called before actors moved(detected early on during the simulation)
void Game::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
/*
    const physx::PxU32 bufferSize = 64;
    physx::PxContactPairPoint contacts[bufferSize];

    for (physx::PxU32 i = 0; i < nbPairs; i++)
    {
        const physx::PxContactPair& contactPair = pairs[i];

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            auto actor1 = pairHeader.actors[0];
            auto actor2 = pairHeader.actors[1];
            GameObject* player = nullptr;
            if (isPlayer(actor1) && isPlane(actor2)) {
                player = static_cast<GameObject*>(actor1->userData);
            }
            else if (isPlayer(actor2) && isPlane(actor1)) {
                player = static_cast<GameObject*>(actor1->userData);
            }

            if (player) {
                auto numContacts = contactPair.extractContacts(contacts, bufferSize);
                for (physx::PxU32 j = 0; j < numContacts; j++) {
                    auto point = contacts[j].position;
                    auto playerPosition = player->getActor()->getGlobalPose().p;
                    auto contactVec = point - playerPosition;
                    contactVec.normalize();
                    player->setGravity(contactVec.x, contactVec.y, contactVec.z);
                    break;
                }
            }
        }
    }
    */
}

void Game::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
    // TODO
    // called from shapes that marked as triggers
}

bool Game::isPlayer(physx::PxRigidActor* actor) {
    return actor->getType() == physx::PxActorType::eRIGID_DYNAMIC;
}

bool Game::isPlane(physx::PxRigidActor* actor) {
    return actor->getType() == physx::PxActorType::eRIGID_STATIC;
}

void Game::onPause() {
    // TODO: handle
    LOGD("APP_CMD_PAUSE");
}
void Game::onResume() {
    // TODO: handle
    LOGD("APP_CMD_RESUME");
}
