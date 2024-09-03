#version 140

uniform sampler2D texture1;

out vec4 FragColor;

in vec2 TexCoords;

void main() {

  FragColor = texture(texture1, TexCoords);
}
