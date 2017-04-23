#ifndef IRONBALLS_SHADER_H
#define IRONBALLS_SHADER_H

#include <GLES3/gl3.h>
#include <stdlib.h>
#include "common.h"
#include "Geometry.h"
#include "light.h"

class Shader {
public:
    Shader();
    ~Shader();

    GLint getProgramId();

    virtual void compile(const char* vertexShader, const char* fragmentShader);
    void bind();
    void unbind();
    virtual void beginRender(Geometry* renderTarget);
    virtual void render(glm::mat4* mvpMatrix, glm::mat4* modelMatrix);
    virtual void endRender();
protected:
    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;
    GLuint m_programId;
    GLint m_mvpMatrixLocation;
    GLint m_modelMatrixLocation;
    GLint m_positionLocation;
    GLint m_normalLocation;

    Geometry* m_renderTarget;
    DirectionalLight m_light;
    DirectionalLightLocation m_lightLocation;

private:
    GLuint loadShader(GLenum shaderType, const char* pSource);
};


#endif //IRONBALLS_SHADER_H
