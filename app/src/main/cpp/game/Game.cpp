#include "Game.h"
#include "geometry/Sphere.h"
#include "geometry/GeometryCache.h"

Game::Game(struct android_app *app) :
        AndroidGame(app),
        m_initialized(false),
        m_ball(nullptr),
        m_screenWidth(1920),
        m_screenHeight(1080),
        m_cameraAngle(0.0f),
        m_xAxis(nullptr),
        m_yAxis(nullptr),
        m_zAxis(nullptr),
        m_pxFoundation(nullptr),
        m_pxPhysics(nullptr),
        m_pxScene(nullptr),
        m_pxBall(nullptr),
        m_pxTimestep(1.0f/60.0f)
{
    initPhysX();
    initLevel();
}

void Game::initLevel()
{
    if (m_initialized) {
        return;
    }

    m_initialized = true;
    m_ball = std::make_unique<GameObject>(0);
    m_ball->setPosition(0.0f, 0.0f, 0.0f);
    //m_inputSystem.initialize(&inputTouchLayer);
    //m_inputSystem.addEntity(m_ball.get());

    auto geometry = GeometryCache::getInstance()->getSphere();
    geometry->setPrimitive(GL_LINES);
    geometry->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_ball->setGeometry(geometry);
    // TODO sync with physx position(use physx active transforms)
    m_ball->setPosition(0.0f, 0.0f, 0.0f);

    m_xAxis = GeometryCache::getInstance()->getLine(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f));
    m_xAxis->setColor(1.0f, 0.0f, 0.0f, 1.0f);

    m_yAxis = GeometryCache::getInstance()->getLine(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    m_yAxis->setColor(0.0f, 1.0f, 0.0f, 1.0f);

    m_zAxis = GeometryCache::getInstance()->getLine(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 10.0f));
    m_zAxis->setColor(0.0f, 0.0f, 1.0f, 1.0f);

    m_room.initialize();
}

void Game::startGraphics() {
    static auto configured = false;
    if (configured) {
        // TODO:
        LOGI("REconfigre openGL");
    }
    else {
        configured = true;
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    char* vertexShader = readAsset("shaders/debug.vert");
    char* fragmentShader = readAsset("shaders/debug.frag");

    if (vertexShader == nullptr || fragmentShader == nullptr) {
        LOGI("Load shader failed!");
    }
    m_shader.reset();
    m_shader.compile(vertexShader, fragmentShader);

    free(vertexShader);
    free(fragmentShader);

    GeometryCache::getInstance()->connect();
}

void Game::killGraphics() {
    GeometryCache::getInstance()->disconnect();
    m_shader.reset();
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

    physx::PxTolerancesScale tolerance;

    m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, physx::PxTolerancesScale() );
    if (!m_pxPhysics) {
        LOGI("PxCreatePhysics failed!");
        return;
    }
    LOGI("PxCreatePhysics success!");

    physx::PxSceneDesc sceneDescription(tolerance);
    sceneDescription.gravity.x = 0.0f;
    sceneDescription.gravity.y = -9.8f;
    sceneDescription.gravity.z = 0.0f;
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

    m_pxScene = m_pxPhysics->createScene(sceneDescription);
    if (!m_pxScene) {
        LOGI("createScene failed!");
        return;
    }
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0);
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0);

    physx::PxMaterial* material = m_pxPhysics->createMaterial(0.5, 0.5, 0.5);

    m_room.connect(m_pxPhysics, m_pxScene);

    // Create sphere
    physx::PxReal density = 1.0f;
    physx::PxQuat quat;
    quat.x = 0.0f;
    quat.y = 0.0f;
    quat.z = 0.0f;
    quat.w = 1.0f;
    physx::PxTransform transform(physx::PxVec3(0.0f, 4.0f, 0.0f), quat);
    physx::PxSphereGeometry geometry(0.5);
    physx::PxRigidDynamic *actor = PxCreateDynamic(*m_pxPhysics, transform, geometry, *material, density);
    // damping - friction or resistance, angular = rotation/spinning, linear = moving
    //actor->setAngularDamping(0.75);
    //actor->setLinearDamping(0.1);
    actor->setLinearVelocity(physx::PxVec3(0,0,0));
    if (!actor) {
        LOGI("create actor failed!");
    }
    m_pxScene->addActor(*actor);
    m_pxBall = actor;
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

void Game::update(float deltaTime) {
    AndroidGame::update(deltaTime);

    m_cameraAngle += 0.5;
    if (m_cameraAngle >= 360.0f) {
        m_cameraAngle = 0.0f;
    }

    m_pxScene->simulate(deltaTime);
    // wait for simulation to finish(separate thread)
    while (!m_pxScene->fetchResults()) {

    }
}

void Game::render() {
    AndroidGame::render();

    // DIAGONAL
//    glm::vec3 camPositionVector(0.0f, 4.0f, 10.0f);
//    glm::vec3 camUpVector(0.0f, 1.0f, 0.0f);
//    glm::vec3 camDirectionVector(0.0f, -1.0f, -1.0f);

    // SIDE FROM X
    float distance = 4.0f;
    auto radians = glm::radians(m_cameraAngle);
    glm::vec3 camPositionVector(glm::sin(radians) * distance, 1.0f, glm::cos(radians) * distance);
    glm::vec3 camUpVector(0.0f, 1.0f, 0.0f);
    glm::vec3 camDirectionVector(camPositionVector.x * -1.0f, 0.0f, camPositionVector.z * -1.0f);

    m_viewMatrix = glm::lookAt(camPositionVector, camDirectionVector, camUpVector);

    m_shader.bind();

    renderPxActor(m_pxBall, m_ball->getGeometry());
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
    physx::PxShape** shapes = new physx::PxShape*[nShapes];

    actor->getShapes(shapes, nShapes);
    while (nShapes--)
    {
        auto shape = shapes[nShapes];
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
    delete [] shapes;
}

void Game::finalizePhysX() {
    m_pxPhysics->release();
    m_pxPhysics = nullptr;

    m_pxFoundation->release();
    m_pxFoundation = nullptr;
}

void Game::onResize() {
    AndroidGame::onResize();
    float aspectRatio = (float)m_surfaceWidth / m_surfaceHeight;
    // TODO choose optimal zoom for objects
    m_projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.01f, 50.0f);
    m_screenHeight = 1080;
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

char* Game::readAsset(const std::string& path) {
    auto assetManager = m_app->activity->assetManager;
    AAsset* asset = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);

    if (asset == nullptr) {
        LOGI("Read actor shader failed!");
        return nullptr;
    }

    long length = AAsset_getLength(asset);
    char* buffer = (char*) malloc (length + 1); // +1 for string zero termination
    if (buffer == nullptr) {
        LOGI("Allocation error!");
        return nullptr;
    }
    auto bytesRead = AAsset_read (asset, buffer, length);
    if (bytesRead != length) {
        LOGI("Read asset bytes error!");
        return nullptr;
    }
    // fix the string to be zero-terminated
    buffer[length] = 0;
    AAsset_close(asset);
    return buffer;
}

Game::~Game()
{
    finalizePhysX();
    m_inputSystem.removeAllEntities();
}