#include "Game.h"
#include "Sphere.h"
#include "Cube.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

Game::Game(struct android_app *app) :
        AndroidGame(app),
        m_initialized(false),
        m_ball(nullptr),
        m_screenWidth(1920),
        m_screenHeight(1080),
        m_pxFoundation(nullptr),
        m_pxPhysics(nullptr),
        m_pxScene(nullptr),
        m_pxGroundPlane(nullptr),
        m_pxBox(nullptr),
        m_pxTimestep(1.0f/60.0f)
{
    initLevel();
    initPhysX();
}

void Game::initLevel()
{
    if(m_initialized) {
        return;
    }

    m_initialized = true;
    //m_ball = std::make_unique<GameObject>(0);
    //m_ball->setPosition(0.0f, 0.0f, 0.0f);

    //m_inputSystem.initialize(&inputTouchLayer);
    //m_inputSystem.addEntity(m_ball.get());
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

    // create ground plane

    physx::PxReal d = 0.0f;
    physx::PxTransform pose = physx::PxTransform(physx::PxVec3(0.0f, 0, 0.0f),
                                                 physx::PxQuat(physx::PxHalfPi,
                                                               physx::PxVec3(0.0f, 0.0f, 1.0f)));
    physx::PxRigidStatic* plane = m_pxPhysics->createRigidStatic(pose);
    if (!plane) {
        LOGI("create plane failed!!");
    }
    physx::PxShape* shape = plane->createShape(physx::PxPlaneGeometry(), *material);
    if (!shape) {
        LOGI("create shape failed!");
    }
    m_pxScene->addActor(*plane);

    //2) Create cube
    physx::PxReal density = 1.0f;
    physx::PxQuat quat;
    quat.x = 0.0f;
    quat.y = 0.0f;
    quat.z = 0.0f;
    quat.w = 1.0f;
    physx::PxTransform transform(physx::PxVec3(0.0f, 10.0f, 0.0f), quat);
    physx::PxVec3 dimensions(0.5,0.5,0.5);
    physx::PxBoxGeometry geometry(dimensions);

    physx::PxRigidDynamic *actor = PxCreateDynamic(*m_pxPhysics, transform, geometry, *material, density);
    actor->setAngularDamping(0.75);
    actor->setLinearVelocity(physx::PxVec3(0,0,0));
    if (!actor) {
        LOGI("create actor failed!");
    }

    m_pxScene->addActor(*actor);

    m_pxBox = actor;
}

void Game::drawActor(physx::PxRigidActor* actor) {
    physx::PxU32 nShapes = actor->getNbShapes();
    physx::PxShape** shapes = new physx::PxShape*[nShapes];

    actor->getShapes(shapes, nShapes);
    while (nShapes--)
    {
        drawShape(shapes[nShapes], actor);
    }
    delete [] shapes;
}
void Game::drawShape(physx::PxShape* shape, physx::PxRigidActor* actor) {
    physx::PxGeometryType::Enum type = shape->getGeometryType();
    switch(type)
    {
        case physx::PxGeometryType::eBOX:
            drawBox(shape, actor);
            break;
    }
}
void Game::drawBox(physx::PxShape* shape, physx::PxRigidActor* actor) {
    physx::PxTransform transform = physx::PxShapeExt::getGlobalPose(*shape, *actor);
    physx::PxBoxGeometry bg;
    shape->getBoxGeometry(bg);
    physx::PxMat33 m = physx::PxMat33(transform.q );
    float mat[16];
    // TODO
//    getColumnMajor(m,pT.p, mat);
//    glPushMatrix();
//    glMultMatrixf(mat);
//    glutSolidCube(bg.halfExtents.x*2);
//    glPopMatrix();
}

void Game::update(float deltaTime) {
    AndroidGame::update(deltaTime);

    m_pxScene->simulate(deltaTime);
    while (!m_pxScene->fetchResults()) {

    }
    //m_inputSystem.update(deltaTime);
   // m_ball->rotate(glm::vec3(0.0f, 1.0f, 0.0f), objectRotation);
    //m_ball->setRotation(0.0, 0.0, objectRotation);
    //m_ball->updateModelMatrix();

    //glm::vec3 camPositionVector(0.0f, 0.0f, 2.0f);
    //glm::vec3 camUpVector(0.0f, 1.0f, 0.0f);
    //glm::vec3 camDirectionVector(0.0f, 0.0f, -1.0f);
    //m_viewMatrix = glm::lookAt(camPositionVector, camDirectionVector, camUpVector);
}

void Game::configureOpenGL() {
    AndroidGame::configureOpenGL();

    // FIXME: hardcoded path
    char* actorVertexShader = readAsset("shaders/actor.vert");
    char* actorFragmentShader = readAsset("shaders/actor.frag");

    if (actorVertexShader == nullptr || actorFragmentShader == nullptr) {
        LOGI("Load actor shader failed!");
    }
    m_shader.compile(actorVertexShader, actorFragmentShader);

    free(actorVertexShader);
    free(actorFragmentShader);

    // TODO make geometry independent from opengl or init opengl before gameloop
    //m_ball->setGeometry(std::make_unique<Cube>());
}

void Game::render() {
    AndroidGame::render();

    //auto modelMatrix = m_viewMatrix * m_ball->getModelMatrix();
    //auto modelMatrix = m_ball->getModelMatrix();
    //auto mvpMatrix = m_projectionMatrix * m_viewMatrix * modelMatrix;

    //m_shader.bind();
    //m_shader.beginRender(m_ball->getGeometry());
    //m_shader.render(&mvpMatrix, &modelMatrix);
    //m_shader.endRender();
    //m_shader.unbind();
}

void Game::drawAxis(int x, int y, int z) {

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
    m_projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.1f, 10.0f);
    m_screenHeight = 1080;
    // TODO removed static const to int, test
    m_screenWidth = m_screenHeight * m_surfaceWidth / m_surfaceHeight;
    inputTouchLayer.updateScreenSize(m_screenWidth, m_screenHeight, m_screenHeight / (float)m_surfaceHeight);
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
    AAsset_read (asset, buffer, length);
    // fix the string to be zero-terminated
    buffer[length] = 0;
    AAsset_close(asset);
    return buffer;
}

Game::~Game()
{
    finalizePhysX();
}