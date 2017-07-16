#include "Shader.h"

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
        m_numPointLightsLocation(-1),
        m_renderTarget(nullptr) {

}

Shader::~Shader() {
    reset();
}

GLuint Shader::getProgramId() {
    return m_programId;
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
        LOGE("*** Shader program link failed, %d", status);
        return false;
    }
    LOGD("Program linking succeeded.");
    LOGD("Shader compilation successfully finished");

    return true;
}

bool Shader::init() {
    m_initSuccessful = false;
    bind();

    m_positionLocation = getAttribLocation("a_position");
    m_normalLocation = getAttribLocation("a_normal");
    m_mvpMatrixLocation = getUniformLocation("u_mvpMatrix");
    m_worldMatrixLocation = getUniformLocation("u_worldMatrix");
    m_colorLocation = getUniformLocation("u_color");

    const auto nameBufferSize = 128;
    for (auto i = 0; i < MAX_POINT_LIGHTS; ++i) {
        char name[nameBufferSize];
        memset(name, 0, sizeof(name));

        // TODO: test sprintf
        sprintf(name, "u_pointLights[%d].base.color", i);
        m_pointLightsLocation[i].color = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].base.ambientIntensity", i);
        m_pointLightsLocation[i].ambientIntensity = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].base.diffuseIntensity", i);
        m_pointLightsLocation[i].diffuseIntensity = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].attenuation.constantFactor", i);
        m_pointLightsLocation[i].attenuation.constantFactor = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].attenuation.linearFactor", i);
        m_pointLightsLocation[i].attenuation.linearFactor = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].attenuation.exponentialFactor", i);
        m_pointLightsLocation[i].attenuation.exponentialFactor = getUniformLocation(name);

        sprintf(name, "u_pointLights[%d].position", i);
        m_pointLightsLocation[i].position = getUniformLocation(name);
    }

    unbind();
    if (m_initSuccessful) {
        LOGD("Shader init succeeded.");
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
                char* buf = (char*) malloc(infoLen);
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
    glVertexAttribPointer(m_positionLocation, 3, GL_FLOAT, GL_FALSE, renderTarget->getVerticesStride(), 0);
    glEnableVertexAttribArray(m_positionLocation);

    // TODO set uniform light properties
}

void Shader::render(glm::mat4* mvpMatrix, glm::mat4* modelMatrix) {
    if (m_renderTarget == nullptr) {
        LOGE("Unable to render in shader, target is null");
        return;
    }

    glUniformMatrix4fv(m_mvpMatrixLocation, 1, GL_FALSE, glm::value_ptr(*mvpMatrix));
    // TODO: pass model 3x3 matrix?
    // GLint location, GLsizei count, const GLfloat* v
    glUniform4fv(m_colorLocation, 1, glm::value_ptr(*m_renderTarget->getColor()));

    m_renderTarget->bindNormalBuffer();
    glVertexAttribPointer(m_normalLocation, 3, GL_FLOAT, GL_FALSE, m_renderTarget->getNormalsStride(), 0);
    glEnableVertexAttribArray(m_normalLocation);
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
    if (m_renderTarget == nullptr){
        LOGE("Unable to end render. Render target is null");
        return;
    }
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

void Shader::setPointLights(int numLights, PointLight* pointLights) {
    if (numLights > MAX_POINT_LIGHTS) {
        LOGE("Num point lights > MAX (%d)", numLights);
        return;
    }
    if (!m_initSuccessful) {
        LOGE("Unable to set point lights, shader is not initialized", numLights);
        return;
    }

    for (auto i = 0; i < numLights; ++i) {
        glUniform3f(m_pointLightsLocation[i].color, pointLights[i].color.x, pointLights[i].color.y, pointLights[i].color.z);
        glUniform1f(m_pointLightsLocation[i].ambientIntensity, pointLights[i].ambientIntensity);
        glUniform1f(m_pointLightsLocation[i].diffuseIntensity, pointLights[i].diffuseIntensity);
        glUniform1f(m_pointLightsLocation[i].attenuation.constantFactor, pointLights[i].attenuation.constantFactor);
        glUniform1f(m_pointLightsLocation[i].attenuation.linearFactor, pointLights[i].attenuation.linearFactor);
        glUniform1f(m_pointLightsLocation[i].attenuation.exponentialFactor, pointLights[i].attenuation.exponentialFactor);
        glUniform3f(m_pointLightsLocation[i].position, pointLights[i].position.x, pointLights[i].position.y, pointLights[i].position.z);
    }
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