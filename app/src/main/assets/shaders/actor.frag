#version 300 es
precision mediump float;

struct DirectionalLight
{
    vec3 color;
    float diffuseIntensity;
    vec3 direction;
};

out vec4 fragColor;
in vec3 v_normal;
uniform DirectionalLight u_directionalLight;

void main() {
    vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1.0);
    float diffuseFactor = max(dot(v_normal, u_directionalLight.direction), 0.0);
    vec4 diffuseColor = vec4(u_directionalLight.color, 1.0) * u_directionalLight.diffuseIntensity * diffuseFactor;
    fragColor = ambientColor + diffuseColor;
}