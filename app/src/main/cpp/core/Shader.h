#ifndef IRONBALLS_SHADER_H
#define IRONBALLS_SHADER_H

#include <GLES3/gl3.h>
#include <stdlib.h>
#include "common.h"
#include "../game/geometry/Geometry.h"
#include "light.h"
#include "../game/light/PointLight.h"

class Shader {
public:
    static const int MAX_POINT_LIGHTS = 4;

    Shader();
    ~Shader();

    GLuint getProgramId();

    bool compile(const char* vertexShader, const char* fragmentShader);
    bool init();
    void bind();
    void unbind();
    void beginRender(Geometry* renderTarget);
    void render(glm::mat4* mvpMatrix, glm::mat4* modelMatrix);
    void endRender();
    void reset();
private:
    bool m_initSuccessful;
    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;
    GLuint m_programId;
    GLint m_positionLocation;
    GLint m_normalLocation;
    GLint m_mvpMatrixLocation;
    GLint m_worldMatrixLocation;
    GLint m_colorLocation;
    GLint m_numPointLightsLocation;

    Geometry* m_renderTarget;

    struct {
        GLint color;
        GLint ambientIntensity;
        GLint diffuseIntensity;
        GLint position;
        struct
        {
            GLint constantFactor;
            GLint linearFactor;
            GLint exponentialFactor;
        } attenuation;
    } m_pointLightsLocation[MAX_POINT_LIGHTS];

    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLint getAttribLocation(const char* attribName);
    GLint getUniformLocation(const char* uniformName);
    void setPointLights(int numLights, PointLight* pointLights);
};


#endif //IRONBALLS_SHADER_H
