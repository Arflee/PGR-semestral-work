#version 330 core

layout (location = 0) in vec3 position;           // vertex position in world space
layout (location = 0) in vec2 texCoord;           // incoming texture coordinates

uniform mat4 PVMmatrix;     // Projection * View * Model --> model to clip coordinates
uniform mat4 model;     // Projection * View * Model --> model to clip coordinates
smooth out vec2 texCoord_v; // outgoing vertex texture coordinates

void main()
{
  
  // vertex position after the projection (gl_Position is predefined output variable)
  gl_Position = PVMmatrix * model * vec4(position, 1.0);   // outgoing vertex in clip coordinates

  // outputs entering the fragment shader
  texCoord_v = texCoord;
}
