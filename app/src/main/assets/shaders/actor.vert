#version 300 es

in vec3 a_position;
in vec3 a_normal;

uniform mat4 u_mvpMatrix;
uniform mat4 u_worldMatrix;

out vec3 v_normal;
out vec3 v_worldPosition;

void main() {
    gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
    v_normal = (u_worldMatrix * vec4(a_normal, 1.0)).xyz;
    v_worldPosition = (u_worldMatrix * vec4(a_position, 1.0)).xyz;
}