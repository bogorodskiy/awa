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
in vec3 v_worldPosition;
uniform vec4 u_color;
uniform int u_numPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform vec3 u_eyeWorldPosition;
uniform float u_matSpecularIntensity;
uniform float u_specularPower;
out vec4 fragColor;

vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 normal)
{
    vec4 ambientColor = vec4(light.color * light.ambientIntensity, 1.0f);
    float diffuseFactor = dot(normal, -lightDirection);

    vec4 diffuseColor  = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f) {
        diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

        vec3 vertexToEye = normalize(u_eyeWorldPosition - v_worldPosition);
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, u_specularPower);
            specularColor = vec4(light.color * u_matSpecularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(int index, vec3 normal)
{
    vec3 lightDirection = v_worldPosition - u_pointLights[index].position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = calcLightInternal(u_pointLights[index].base, lightDirection, normal);
    float attenuation =  u_pointLights[index].attenuation.constantFactor +
                         u_pointLights[index].attenuation.linearFactor * distance +
                         u_pointLights[index].attenuation.exponentialFactor * distance * distance;

    return color / attenuation;
}

void main() {
    vec3 normal = normalize(v_normal);
    vec4 result = vec4(0, 0, 0, 0);
    for (int i = 0; i < u_numPointLights; ++i) {
        result += calcPointLight(i, normal);
    }
    fragColor = u_color * result;
}