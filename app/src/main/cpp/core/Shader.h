#ifndef IRONBALLS_SHADER_H
#define IRONBALLS_SHADER_H

#include <GLES3/gl3.h>
#include <stdlib.h>
#include "common.h"
#include "../game/geometry/Geometry.h"
#include "light.h"

class Shader {
public:
    Shader();
    ~Shader();

    GLuint getProgramId();

    virtual void compile(const char* vertexShader, const char* fragmentShader);
    void bind();
    void unbind();
    virtual void beginRender(Geometry* renderTarget);
    virtual void render(glm::mat4* mvpMatrix, glm::mat4* modelMatrix);
    virtual void endRender();
    virtual void reset();
protected:
    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;
    GLuint m_programId;
    GLint m_mvpMatrixLocation;
    GLint m_positionLocation;
    GLint m_colorLocation;

    Geometry* m_renderTarget;
private:
    GLuint loadShader(GLenum shaderType, const char* pSource);
};


#endif //IRONBALLS_SHADER_H
