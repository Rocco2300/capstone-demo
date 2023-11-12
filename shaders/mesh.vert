#version 330 core

layout (location = 0) in vec4 position;

void main() {
    float value = 0.0;

    for (float i = 0.0; i < 8196.0; i += 1.0) {
        value = sin(dot(position.xz, position.yw) * i);
    }

    gl_Position = value * position / value;
}