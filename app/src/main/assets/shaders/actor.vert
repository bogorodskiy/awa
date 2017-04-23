#version 300 es
uniform mat4 u_mvpMatrix;
uniform mat3 u_worldMatrix;

in vec4 a_position;
in vec3 a_normal;
out vec3 v_normal;

void main() {
    gl_Position = u_mvpMatrix * a_position;
    v_normal = normalize(u_worldMatrix * a_normal);
}