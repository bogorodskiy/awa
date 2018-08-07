#include "PhysicsSystem.h"
#include "PxPhysicsAPI.h"
#include "../../geometry/GeometryFactory.h"
#include "../../GlobalSettings.h"

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

const physx::PxReal PhysicsSystem::STATIC_FRICTION = 0.5;
const physx::PxReal PhysicsSystem::DYNAMIC_FRICTION = 0.5;
const physx::PxReal PhysicsSystem::RESTITUTION = 0.5;

PhysicsSystem::~PhysicsSystem() {
    m_pxMaterial->release();
    m_pxPhysics->release();
    m_pxPhysics = nullptr;

    m_pxFoundation->release();
    m_pxFoundation = nullptr;
}

bool PhysicsSystem::initialize() {
    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    static physx::PxDefaultAllocator gDefaultAllocatorCallback;
    static physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;

    m_pxFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback,
                                        gDefaultErrorCallback);
    if (!m_pxFoundation) {
        LOGE("PxCreateFoundation failed!");
        return false;
    }
    LOGI("PxCreateFoundation success!");

    m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, physx::PxTolerancesScale() );
    if (!m_pxPhysics) {
        LOGE("PxCreatePhysics failed!");
        return false;
    }
    LOGI("PxCreatePhysics success!");

    physx::PxSceneDesc sceneDescription(m_pxPhysics->getTolerancesScale());
    if (!sceneDescription.cpuDispatcher) {
        physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
        if(!mCpuDispatcher) {
            LOGE("PxDefaultCpuDispatcherCreate failed!");
            return false;
        }
        sceneDescription.cpuDispatcher = mCpuDispatcher;
    }
    if (!sceneDescription.filterShader) {
        sceneDescription.filterShader = gDefaultFilterShader;
    }
    sceneDescription.simulationEventCallback = this;
    sceneDescription.filterShader = GameContactReportFilterShader;
    sceneDescription.gravity = physx::PxVec3(0.0f, GlobalSettings::DEFAULT_GRAVITY, 0.0f);
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_PCM;
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
    sceneDescription.flags |= physx::PxSceneFlag::eSUPPRESS_EAGER_SCENE_QUERY_REFIT;
    sceneDescription.flags |= physx::PxSceneFlag::eENABLE_KINEMATIC_PAIRS;

    m_pxScene = m_pxPhysics->createScene(sceneDescription);
    if (!m_pxScene) {
        LOGI("createScene failed!");
        return false;
    }
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0);
    m_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0);

    m_pxMaterial = m_pxPhysics->createMaterial(STATIC_FRICTION, DYNAMIC_FRICTION, RESTITUTION);
    return true;
}

std::shared_ptr<RigidDynamicComponent> PhysicsSystem::getDynamicComponent(GameObject* gameObject) {
    for (auto& component: m_dynamicComponents) {
        if (component->getGameObject() == gameObject) {
            return component;
        }
    }
    return nullptr;
}

void PhysicsSystem::addDynamicEntity(GameObject* gameObject,
                                     float radius) {
    if (m_numDynamicComponents == m_dynamicComponents.size()) {
        m_dynamicComponents.emplace_back(std::make_shared<RigidDynamicComponent>());
    }
    m_dynamicComponents[m_numDynamicComponents]->fillWith(m_pxPhysics,
                                                          m_pxScene,
                                                          m_pxMaterial,
                                                          physx::PxSphereGeometry(radius),
                                                          gameObject);
    ++m_numDynamicComponents;
}

void PhysicsSystem::addStaticEntity(GameObject* gameObject) {
    if (m_numStaticComponents == m_staticComponents.size()) {
        m_staticComponents.emplace_back(std::make_shared<RigidStaticComponent>());
    }
    m_staticComponents[m_numStaticComponents]->fillWith(m_pxPhysics,
                                                        m_pxScene,
                                                        m_pxMaterial,
                                                        physx::PxPlaneGeometry(),
                                                        gameObject);
    ++m_numStaticComponents;
}

void PhysicsSystem::removeDynamicEntity(GameObject* gameObject) {
    removeEntity<RigidDynamicComponent>(m_dynamicComponents, m_numDynamicComponents, gameObject);
}

void PhysicsSystem::removeStaticEntity(GameObject* gameObject) {
    removeEntity<RigidStaticComponent>(m_staticComponents, m_numStaticComponents, gameObject);
}

void PhysicsSystem::update(float dt) {
    for (auto i = 0; i < m_numDynamicComponents; ++i) {
        m_dynamicComponents[i]->update(dt);
    }

    m_pxScene->simulate(dt);

    // wait for simulation to finish(in separate thread)
    while (!m_pxScene->fetchResults()) {
    }
}

bool PhysicsSystem::isDynamic(physx::PxRigidActor* actor) {
    return actor->getType() == physx::PxActorType::eRIGID_DYNAMIC;
}

void PhysicsSystem::reset() {
    m_dynamicComponents.clear();
    m_staticComponents.clear();
}

// -----  PxSimulationEventCallback interface methods -----

// called before actors moved(detected early on during the simulation)
void PhysicsSystem::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
    static const physx::PxU32 bufferSize = 64;
    physx::PxContactPairPoint contacts[bufferSize];

    for (physx::PxU32 i = 0; i < nbPairs; i++)
    {
        const physx::PxContactPair& contactPair = pairs[i];

        if (contactPair.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            auto actorA = pairHeader.actors[0];
            auto actorB = pairHeader.actors[1];
            GameObject* gameObjectA = nullptr;
            GameObject* gameObjectB = nullptr;
            if (isDynamic(actorA) && isDynamic(actorB)) {
                gameObjectA = static_cast<GameObject*>(actorA->userData);
                gameObjectB = static_cast<GameObject*>(actorB->userData);
            }

            if (gameObjectA != nullptr) {
                auto numContacts = contactPair.extractContacts(contacts, bufferSize);
                for (physx::PxU32 j = 0; j < numContacts; ++j) {
                    auto contact = contacts[j];
                    LOGD("+++ COLLISION IMPULSE = %f", contact.impulse.magnitude());
                    auto point = contact.position;
                    auto aPosition = gameObjectA->transform.p;
                    auto bPosition = gameObjectB->transform.p;
                    auto contactVec = point - aPosition;
                    contactVec.normalize();
                    break;
                }
            }
        }
    }
}

void PhysicsSystem::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
    // TODO
    // called from shapes that marked as triggers
}