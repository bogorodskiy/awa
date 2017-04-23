#include "Shader.h"

Shader::Shader():
        m_vertexShaderId(0),
        m_fragmentShaderId(0),
        m_programId(-1),
        m_mvpMatrixLocation(-1),
        m_modelMatrixLocation(-1),
        m_positionLocation(-1),
        m_renderTarget(nullptr) {

}

GLint Shader::getProgramId() {
    return m_programId;
}

void Shader::compile(const char* vertexShader, const char* fragmentShader) {
    LOGD("Compiling shader.");

    m_vertexShaderId = loadShader(GL_VERTEX_SHADER, vertexShader);
    m_fragmentShaderId = loadShader(GL_FRAGMENT_SHADER, fragmentShader);

    if (m_vertexShaderId == 0 || m_fragmentShaderId == 0) {
        LOGE("Shader creation error");
        // TODO abort game
        return;
    }

    m_programId = glCreateProgram();
    if (!m_programId) {
        LOGE("*** Failed to create program");
        // TODO abort game
        return;
    }

    GLint status = 0;

    glAttachShader(m_programId, m_vertexShaderId);
    glAttachShader(m_programId, m_fragmentShaderId);
    glLinkProgram(m_programId);
    glGetProgramiv(m_programId, GL_LINK_STATUS, &status);

    if (status == 0) {
        LOGE("*** Shader program link failed, %d", status);
        // TODO abort game
        return;
    }
    LOGD("Program linking succeeded.");

    bind();
    m_mvpMatrixLocation = glGetUniformLocation(m_programId, "u_mvpMatrix");
    if (m_mvpMatrixLocation < 0) {
        LOGE("Couldn't get shader's mvp matrix location");
        return;
    }
    m_modelMatrixLocation = glGetUniformLocation(m_programId, "u_worldMatrix");
    if (m_modelMatrixLocation < 0) {
        LOGE("Couldn't get shader's world matrix location");
        return;
    }
    m_positionLocation = glGetAttribLocation(m_programId, "a_position");
    if (m_positionLocation < 0) {
        LOGE("Couldn't get shader's position location");
        return;
    }
    m_normalLocation = glGetAttribLocation(m_programId, "a_normal");
    if (m_normalLocation < 0) {
        LOGE("Couldn't get shader's normal location");
        return;
    }

    m_lightLocation.color = glGetUniformLocation(m_programId, "u_directionalLight.color");
    m_lightLocation.direction = glGetUniformLocation(m_programId, "u_directionalLight.direction");
    m_lightLocation.diffuseIntensity = glGetUniformLocation(m_programId, "u_directionalLight.diffuseIntensity");

    if (m_lightLocation.color == 0xFFFFFFFF ||
        m_lightLocation.diffuseIntensity == 0xFFFFFFFF ||
        m_lightLocation.direction == 0xFFFFFFFF) {
        LOGE("Couldn't get light property location");
        return;
    }

    // TODO move
    m_light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_light.diffuseIntensity = 0.75f;
    m_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);

    LOGD("Shader compilation successfully finished");
    unbind();
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

    glUniform3f(m_lightLocation.color, m_light.color.x, m_light.color.y, m_light.color.z);
    glUniform3f(m_lightLocation.direction, m_light.direction.x, m_light.direction.y, m_light.direction.z);
    glUniform1f(m_lightLocation.diffuseIntensity, m_light.diffuseIntensity);
}

void Shader::render(glm::mat4* mvpMatrix, glm::mat4* modelMatrix) {
    if (m_renderTarget == nullptr) {
        LOGE("Unable to render in shader, target is null");
        return;
    }

    // TODO optimize
    glm::mat3 model3x3Matrix;
    model3x3Matrix[0][0] = (*modelMatrix)[0][0];
    model3x3Matrix[0][1] = (*modelMatrix)[0][1];
    model3x3Matrix[0][2] = (*modelMatrix)[0][2];

    model3x3Matrix[1][0] = (*modelMatrix)[1][0];
    model3x3Matrix[1][1] = (*modelMatrix)[1][1];
    model3x3Matrix[1][2] = (*modelMatrix)[1][2];

    model3x3Matrix[2][0] = (*modelMatrix)[2][0];
    model3x3Matrix[2][1] = (*modelMatrix)[2][1];
    model3x3Matrix[2][2] = (*modelMatrix)[2][2];

    model3x3Matrix = glm::transpose(glm::inverse(model3x3Matrix));
    glUniformMatrix4fv(m_mvpMatrixLocation, 1, GL_FALSE, glm::value_ptr(*mvpMatrix));
    glUniformMatrix3fv(m_modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model3x3Matrix));

    m_renderTarget->bindNormalBuffer();
    glVertexAttribPointer(m_normalLocation, 3, GL_FLOAT, GL_FALSE, m_renderTarget->getNormalsStride(), 0);
    glEnableVertexAttribArray(m_normalLocation);
    m_renderTarget->unbindNormalBuffer();

    m_renderTarget->bindIndexBuffer();
    glDrawElements(m_renderTarget->getPrimitive(), m_renderTarget->getNumIndices(), GL_UNSIGNED_SHORT, 0);
    m_renderTarget->unbindIndexBuffer();
}

void Shader::endRender() {
    if (m_renderTarget == nullptr){
        LOGE("Unable to end render. Render target is null");
        return;
    }
    m_renderTarget->unbindVertexBuffer();
    m_renderTarget = nullptr;
}

Shader::~Shader() {
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
}