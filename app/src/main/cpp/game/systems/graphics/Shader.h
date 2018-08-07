#ifndef IRONBALLS_SHADER_H
#define IRONBALLS_SHADER_H

#include <GLES3/gl3.h>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <foundation/PxMat44.h>
#include "../../../core/common.h"
#include "../../geometry/Geometry.h"
#include "light/PointLight.h"

class Shader {
public:
    static const int MAX_POINT_LIGHTS = 4;

    Shader();
    ~Shader();

    bool compile(const char* vertexShader, const char* fragmentShader);
    bool init();
    void bind();
    void unbind();
    void beginRender(Geometry* renderTarget);
    void setPointLights(const std::vector<std::shared_ptr<PointLight>>& pointLights);
    void setEyeWorldPosition(float x, float y, float z);
    void setMatSpecularIntensity(float value);
    void setMatSpecularPower(float value);
    void render(const physx::PxMat44& mvpMatrix,
                const physx::PxMat44& modelMatrix,
                const physx::PxVec4& color);
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
    GLint m_eyeWorldPositionLocation;
    GLint m_matSpecularIntensityLocation;
    GLint m_matSpecularPowerLocation;
    GLint m_numPointLightsLocation;

    Geometry* m_renderTarget;

    struct {
        GLint ambientIntensity;
        GLint diffuseIntensity;
        GLint color;
        struct
        {
            GLint constantFactor;
            GLint linearFactor;
            GLint exponentialFactor;
        } attenuation;
        GLint position;
    } m_pointLightsLocation[MAX_POINT_LIGHTS];

    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLint getAttribLocation(const char* attribName);
    GLint getUniformLocation(const char* uniformName);
    bool checkLightLocationIsValid(int index);
    // debug
    void printUniforms();
};


#endif //IRONBALLS_SHADER_H
