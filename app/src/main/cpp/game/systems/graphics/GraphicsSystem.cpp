#include "GraphicsSystem.h"
#include "../../Camera.h"
#include "../../../core/resource/ResourceManager.h"
#include "../../GlobalSettings.h"

GraphicsSystem::GraphicsSystem() {
}

bool GraphicsSystem::initialize(Camera* camera, Level* room) {
    m_camera = camera;
    m_level = room;
    // TODO:
    m_components.reserve(0);
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
                               GeometryFactory::GeometryType primitive,
                               glm::vec3 size,
                               glm::vec4 color) {

    auto component = std::make_shared<GraphicsComponent>(gameObject, primitive, size, color);
    if (m_numComponents == m_components.size()) {
        m_components.emplace_back(component);
    }
    else{
        m_components[m_numComponents] = component;
    }
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
        LOGE("Unable to find gameObject with id %i in GraphicsSystem components", gameObject->getId());
        return;
    }

    if (m_numComponents > 1)
    {
        m_components[position] = m_components[m_numComponents - 1];
        m_components[m_numComponents - 1] = nullptr;
    }
    else {
        m_components[position] = nullptr;
    }

    --m_numComponents;
}

void GraphicsSystem::render(const glm::mat4x4& viewProjectionMatrix) {
    // TODO:
    const auto& pointLights = m_level->getPointLights();
    m_shader.bind();
    m_shader.setPointLights(pointLights);
    auto position = m_camera->getPosition();
    m_shader.setEyeWorldPosition(position.x, position.y, position.z);

    for (auto& component : m_components) {
        auto modelMatrix = component->getModelMatrix();
        auto mvpMatrix = viewProjectionMatrix * modelMatrix;
        m_shader.beginRender(component->geometry);
        m_shader.render(&mvpMatrix, &modelMatrix, component->color);
        m_shader.endRender();
    }
    m_shader.unbind();
}
