#version 330 core

smooth in vec2 texCoord_v;    // fragment texture coordinates

uniform float time;           // used for simulation of moving lights (such as sun) and to select proper animation frame
uniform sampler2D texSampler; // sampler for texture access
out vec4 color_f;             // outgoing fragment color

uniform ivec2 pattern = ivec2(12, 6);
uniform float frameDuration = 0.1f;


vec4 sampleTexture(int frame) {

  vec2 offset = vec2(1.0) / vec2(pattern);

  vec2 texCoordBase = texCoord_v / pattern / 2 + offset / 2;
  vec2 texCoord = texCoordBase + vec2(frame % pattern.x, (frame / pattern.x)) * offset;
  return texture(texSampler, texCoord);
}

void main() {
  // frame of the texture to be used for explosion drawing 
  int frame = int(time / frameDuration);

  vec4 texColor = sampleTexture(frame);
  color_f = texColor;
  if(color_f.a < 0.1)
        discard;
}
