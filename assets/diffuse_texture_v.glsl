#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec4 inColor;

uniform mat4 uTransform;

out vec2 vTexCoords;
out vec4 vColor;

void main() {
  gl_Position = uTransform * vec4(inPosition, 1.0);
  vTexCoords = inTexCoords;
  vColor = inColor;
}
