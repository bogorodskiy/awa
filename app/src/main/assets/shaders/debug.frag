#version 300 es
precision mediump float;

uniform vec4 u_color;
out vec4 fragment_color;

void main() {
    fragment_color = u_color;
}