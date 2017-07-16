#version 300 es
precision mediump float;

const int MAX_POINT_LIGHTS = 4;

struct BaseLight
{
    float ambientIntensity;
    float diffuseIntensity;
    vec3 color;
};

struct Attenuation
{
    float constantFactor;
    float linearFactor;
    float exponentialFactor;
};

struct PointLight
{
    BaseLight base;
    Attenuation attenuation;
    vec3 position;
};

in vec3 v_normal;
uniform vec4 u_color;
uniform int u_numPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
out vec4 fragColor;

void main() {
    fragColor = vec4(0.1, 0.1, 0.1, 1.0);
}