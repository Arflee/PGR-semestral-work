#version 330 core
layout (location = 0) in vec2 screenCoord;

out vec3 TexCoords;

uniform mat4 inversePVmatrix;

void main()
{
    vec4 farplaneCoord = vec4(screenCoord, 0.9999, 1.0);
    vec4 worldViewCoord = inversePVmatrix * farplaneCoord;
    TexCoords = worldViewCoord.xyz / worldViewCoord.w;
    gl_Position = farplaneCoord;
}