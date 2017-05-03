#include "Shader.h"

Shader::Shader():
        m_vertexShaderId(0),
        m_fragmentShaderId(0),
        m_programId(-1),
        m_mvpMatrixLocation(-1),
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
    m_positionLocation = glGetAttribLocation(m_programId, "a_position");
    if (m_positionLocation < 0) {
        LOGE("Couldn't get shader's position location");
        return;
    }
    m_colorLocation = glGetUniformLocation(m_programId, "u_color");
    if (m_colorLocation < 0) {
        LOGE("Couldn't get shader's color location");
        return;
    }

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
}

void Shader::render(glm::mat4* mvpMatrix, glm::mat4* modelMatrix) {
    if (m_renderTarget == nullptr) {
        LOGE("Unable to render in shader, target is null");
        return;
    }

    // TODO optimize, разобраться зачем
    //glm::mat3 model3x3Matrix = glm::mat3(*modelMatrix);
    //model3x3Matrix = glm::transpose(glm::inverse(model3x3Matrix));
    glUniformMatrix4fv(m_mvpMatrixLocation, 1, GL_FALSE, glm::value_ptr(*mvpMatrix));
    // GLint location, GLsizei count, const GLfloat* v
    glUniform4fv(m_colorLocation, 1, glm::value_ptr(*m_renderTarget->getColor()));

    m_renderTarget->bindNormalBuffer();
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