#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

smooth out vec2 TexCoords;

float decay = 0.05;

void main() {

  gl_Position = projection * view * model * vec4(aPos, 1.0);

  float localTime = time * decay;

  vec2 offset = vec2(0.0, (floor(localTime) - localTime) * 4 + 1.0);

  TexCoords = aTexCoords + offset;
}
