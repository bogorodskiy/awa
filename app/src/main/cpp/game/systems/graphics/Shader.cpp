#include "Shader.h"

// TODO: init in header
Shader::Shader():
        m_initSuccessful(false),
        m_vertexShaderId(0),
        m_fragmentShaderId(0),
        m_programId(0),
        m_positionLocation(-1),
        m_normalLocation(-1),
        m_mvpMatrixLocation(-1),
        m_worldMatrixLocation(-1),
        m_colorLocation(-1),
        m_eyeWorldPositionLocation(-1),
        m_matSpecularIntensityLocation(-1),
        m_matSpecularPowerLocation(-1),
        m_numPointLightsLocation(-1),
        m_renderTarget(nullptr) {

}

Shader::~Shader() {
    reset();
}

bool Shader::compile(const char* vertexShader, const char* fragmentShader) {
    LOGD("Compiling shader.");

    m_vertexShaderId = loadShader(GL_VERTEX_SHADER, vertexShader);
    m_fragmentShaderId = loadShader(GL_FRAGMENT_SHADER, fragmentShader);

    if (m_vertexShaderId == 0 || m_fragmentShaderId == 0) {
        LOGE("Shader creation error");
        return false;
    }

    m_programId = glCreateProgram();
    if (!m_programId) {
        LOGE("*** Failed to create program");
        return false;
    }

    GLint status = 0;

    glAttachShader(m_programId, m_vertexShaderId);
    glAttachShader(m_programId, m_fragmentShaderId);
    glLinkProgram(m_programId);
    glGetProgramiv(m_programId, GL_LINK_STATUS, &status);

    if (status == 0) {
        LOGE("Shader program link failed, %d", status);
        return false;
    }
    LOGD("Program linking succeeded.");
    LOGD("Shader compilation successfully finished");

    return true;
}

bool Shader::init() {
    m_initSuccessful = true;
    bind();

    m_positionLocation = getAttribLocation("a_position");
    m_normalLocation = getAttribLocation("a_normal");
    m_mvpMatrixLocation = getUniformLocation("u_mvpMatrix");
    m_worldMatrixLocation = getUniformLocation("u_worldMatrix");
    m_colorLocation = getUniformLocation("u_color");
    m_eyeWorldPositionLocation = getUniformLocation("u_eyeWorldPosition");
    m_matSpecularIntensityLocation = getUniformLocation("u_matSpecularIntensity");
    m_matSpecularPowerLocation = getUniformLocation("u_specularPower");
    m_numPointLightsLocation = getUniformLocation("u_numPointLights");

    const auto nameBufferSize = 128;
    for (auto i = 0; i < MAX_POINT_LIGHTS; ++i) {
        auto& location = m_pointLightsLocation[i];
        char name[nameBufferSize];
        memset(name, 0, sizeof(name));

        sprintf(name, "u_pointLights[%d].base.color", i);
        location.color = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].base.ambientIntensity", i);
        location.ambientIntensity = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].base.diffuseIntensity", i);
        location.diffuseIntensity = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].attenuation.constantFactor", i);
        location.attenuation.constantFactor = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].attenuation.linearFactor", i);
        location.attenuation.linearFactor = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].attenuation.exponentialFactor", i);
        location.attenuation.exponentialFactor = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].position", i);
        location.position = getUniformLocation(name);

        if (!checkLightLocationIsValid(i)) {
            m_initSuccessful = false;
            break;
        }
    }



    unbind();
    if (m_initSuccessful) {
        LOGD("Shader init succeeded.");
    }
    else {
        LOGE("Shader init error.");
    }
    return m_initSuccessful;
}

GLuint Shader::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(static_cast<size_t>(infoLen));
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                         shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }

    return shader;
}

void Shader::bind() {
    glUseProgram(m_programId);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::beginRender(Geometry* renderTarget) {
    m_renderTarget = renderTarget;
    if (m_renderTarget == nullptr){
        LOGE("Unable to begin render. Render target is null");
        return;
    }
    m_renderTarget->bindVertexBuffer();
    const auto positionSize = static_cast<GLint>(3);
    const auto normalized = static_cast<GLboolean>(GL_FALSE);
    glVertexAttribPointer(static_cast<GLuint>(m_positionLocation), positionSize, GL_FLOAT, normalized, renderTarget->getVerticesStride(), 0);
    glEnableVertexAttribArray(static_cast<GLuint>(m_positionLocation));
}

void Shader::render(const physx::PxMat44& mvpMatrix, const physx::PxMat44& modelMatrix, const physx::PxVec4& color) {
    static const GLsizei COUNT_ONE = 1;
    static const GLboolean NO_TRANSPOSE = GL_FALSE;
    static const GLint NORMAL_SIZE_THREE = 3;
    static const GLboolean NOT_NORMALIZED = GL_FALSE;

    glUniformMatrix4fv(m_mvpMatrixLocation, COUNT_ONE, NO_TRANSPOSE, mvpMatrix.front());
    glUniformMatrix4fv(m_worldMatrixLocation, COUNT_ONE, NO_TRANSPOSE, modelMatrix.front());
    float rawColor[4] = {color.x, color.y, color.z, color.w};
    glUniform4fv(m_colorLocation, COUNT_ONE, rawColor);
    setMatSpecularIntensity(m_renderTarget->getSpecularIntensity());
    setMatSpecularPower(m_renderTarget->getSpecularPower());

    m_renderTarget->bindNormalBuffer();
    glVertexAttribPointer(static_cast<GLuint>(m_normalLocation), NORMAL_SIZE_THREE, GL_FLOAT, NOT_NORMALIZED, m_renderTarget->getNormalsStride(), 0);
    glEnableVertexAttribArray(static_cast<GLuint>(m_normalLocation));
    m_renderTarget->unbindNormalBuffer();

    if (m_renderTarget->getNumIndices() > 0) {
        m_renderTarget->bindIndexBuffer();
        glDrawElements(m_renderTarget->getPrimitive(), m_renderTarget->getNumIndices(), GL_UNSIGNED_SHORT, 0);
        m_renderTarget->unbindIndexBuffer();
    }
    else {
       glDrawArrays(m_renderTarget->getPrimitive(), 0, m_renderTarget->getNumVertices());
    }
}

void Shader::endRender() {
    m_renderTarget->unbindVertexBuffer();
    m_renderTarget = nullptr;
}

GLint Shader::getAttribLocation(const char* attribName) {
    auto result = glGetAttribLocation(m_programId, attribName);
    if (result < 0) {
        LOGE("Couldn't get location of attribute %s", attribName);
        m_initSuccessful = false;
    }
    return result;
}

GLint Shader::getUniformLocation(const char* uniformName) {
    auto result = glGetUniformLocation(m_programId, uniformName);
    if (result < 0) {
        LOGE("Couldn't get location of uniform %s", uniformName);
        m_initSuccessful = false;
    }
    return result;
}

void Shader::setPointLights(const std::vector<std::shared_ptr<PointLight>>& pointLights) {
    const auto numLights = pointLights.size();
    if (numLights > MAX_POINT_LIGHTS) {
        LOGE("Num point lights > MAX (%d)", numLights);
        return;
    }
    if (!m_initSuccessful) {
        LOGE("Unable to set point lights, shader is not initialized");
        return;
    }

    for (auto i = 0u; i < numLights; ++i) {
        const auto& pointLight = pointLights.at(i);
        const auto& location = m_pointLightsLocation[i];
        glUniform3f(location.color, pointLight->color.x, pointLight->color.y, pointLight->color.z);
        glUniform1f(location.ambientIntensity, pointLight->ambientIntensity);
        glUniform1f(location.diffuseIntensity, pointLight->diffuseIntensity);
        glUniform1f(location.attenuation.constantFactor, pointLight->attenuation.constantFactor);
        glUniform1f(location.attenuation.linearFactor, pointLight->attenuation.linearFactor);
        glUniform1f(location.attenuation.exponentialFactor, pointLight->attenuation.exponentialFactor);
        glUniform3f(location.position, pointLight->position.x, pointLight->position.y, pointLight->position.z);
    }

    glUniform1i(m_numPointLightsLocation, numLights);
}

void Shader::setEyeWorldPosition(float x, float y, float z) {
    glUniform3f(m_eyeWorldPositionLocation, x, y, z);
}

void Shader::setMatSpecularIntensity(float value) {
    glUniform1f(m_matSpecularIntensityLocation, value);
}

void Shader::setMatSpecularPower(float value) {
    glUniform1f(m_matSpecularPowerLocation, value);
}

bool Shader::checkLightLocationIsValid(int index) {
    auto& location = m_pointLightsLocation[index];
    if (location.color < 0 ||
        location.ambientIntensity < 0 ||
        location.position < 0 ||
        location.diffuseIntensity < 0 ||
        location.attenuation.constantFactor < 0 ||
        location.attenuation.linearFactor < 0 ||
        location.attenuation.exponentialFactor < 0) {
        return false;
    }
    return true;
}

void Shader::reset() {
    if (m_vertexShaderId) {
        glDeleteShader(m_vertexShaderId);
        m_vertexShaderId = 0;
    }
    if (m_fragmentShaderId) {
        glDeleteShader(m_fragmentShaderId);
        m_fragmentShaderId = 0;
    }
    if (m_programId) {
        glDeleteProgram(m_programId);
        m_programId = 0;
    }
    m_mvpMatrixLocation = -1;
    m_positionLocation = -1;
    m_colorLocation = -1;
    m_numPointLightsLocation = -1;
    m_initSuccessful = false;
    m_renderTarget = nullptr;
}

void Shader::printUniforms() {
    GLint count;
    GLint size;
    GLenum type;
    const GLsizei bufSize = 128;
    GLchar name[bufSize];
    GLsizei length;
    glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &count);
    printf("+++ Active Uniforms: %d\n", count);
    for (auto i = 0; i < count; i++)
    {
        glGetActiveUniform(m_programId, (GLuint)i, bufSize, &length, &size, &type, name);
        LOGD("+++ Uniform #%d Type: %u Name: %s\n", i, type, name);
    }
    LOGD("+++ --------------------");
}