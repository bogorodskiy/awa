#include "GraphicsSystem.h"
#include "../../Camera.h"
#include "../../../core/resource/ResourceManager.h"
#include "../../GlobalSettings.h"

GraphicsSystem::GraphicsSystem() {
}

bool GraphicsSystem::initialize(Camera* camera, Level* level) {
    m_camera = camera;
    m_level = level;
    return true;
}

bool GraphicsSystem::startGraphics()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto& resourceManager = ResourceManager::getInstance();

    char* vertexShader = resourceManager.readAsset(GlobalSettings::VERTEX_SHADER_PATH);
    char* fragmentShader = resourceManager.readAsset(GlobalSettings::FRAGMENT_SHADER_PATH);

    if (vertexShader == nullptr) {
        LOGE("Load vertex shader failed!");
        return false;
    }
    if (fragmentShader == nullptr)
    {
        free(vertexShader);
        LOGE("Load fragment shader failed!");
        return false;
    }

    m_shader.reset();
    auto compileResult = m_shader.compile(vertexShader, fragmentShader);
    free(vertexShader);
    free(fragmentShader);
    if (!compileResult) {
        LOGE("Shader compile failed");
        return false;
    }
    auto initResult = m_shader.init();
    if (!initResult) {
        LOGE("Unable to init shader");
        return false;
    }
    return true;
}

void GraphicsSystem::killGraphics()
{
    m_shader.reset();
}

void GraphicsSystem::addEntity(GameObject* gameObject,
                               Geometry::Type geometryType,
                               physx::PxVec3 size,
                               physx::PxVec4 color) {
    if (m_numComponents == m_components.size()) {
        m_components.emplace_back(std::make_shared<GraphicsComponent>());
    }
    m_components[m_numComponents]->fillWith(gameObject, geometryType, size, color);
    ++m_numComponents;
}

void GraphicsSystem::removeEntity(GameObject* gameObject) {
    auto position = 0u;
    for (; position < m_numComponents; ++position)
    {
        if (m_components[position]->getGameObject() == gameObject)
        {
            break;
        }
    }
    if (position == m_numComponents)
    {
        LOGE("+++ Unable to find gameObject with id %i in GraphicsSystem components", gameObject->getId());
        return;
    }

    m_components[position]->reset();
    if (m_numComponents > 1)
    {
        m_components[position]->fillWith(*m_components[m_numComponents - 1]);
        m_components[m_numComponents - 1]->reset();
    }

    --m_numComponents;
}

void GraphicsSystem::render(const physx::PxMat44& viewProjectionMatrix) {
    m_shader.bind();
    m_shader.setPointLights(m_level->getPointLights());
    const auto& position = m_camera->getPosition();
    m_shader.setEyeWorldPosition(position.x, position.y, position.z);
    Geometry* lastGeometry = nullptr;

    for (auto i = 0; i < m_numComponents; ++i) {
        auto& component = m_components[i];
        component->preRender();

        const auto& modelMatrix = component->getModelMatrix();
        auto mvpMatrix = viewProjectionMatrix * modelMatrix;

        if (lastGeometry != component->geometry) {
            lastGeometry = component->geometry;

            m_shader.endRender();
            m_shader.beginRender(component->geometry);
            m_shader.render(mvpMatrix, modelMatrix, component->color);
        }
        else {
            m_shader.render(mvpMatrix, modelMatrix, component->color);
        }
    }
    m_shader.endRender();
    m_shader.unbind();
}

void GraphicsSystem::reset() {
    m_components.clear();
}