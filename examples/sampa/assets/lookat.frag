#version 300 es

precision mediump float;

in vec4 fragColor;
in vec3 eyeSpacePos;

out vec4 outColor;

uniform sampler2D tex;
uniform vec2 viewportSize;

void main() {
  vec2 texel = 1.0 / viewportSize;

  float depth = texture(tex, eyeSpacePos.xy).r;

  float depthUp = texture(tex, eyeSpacePos.xy + vec2(0.0, texel.y)).r;
  float depthDown = texture(tex, eyeSpacePos.xy - vec2(0.0, texel.y)).r;
  float depthLeft = texture(tex, eyeSpacePos.xy - vec2(texel.x, 0.0)).r;
  float depthRight = texture(tex, eyeSpacePos.xy + vec2(texel.x, 0.0)).r;

  vec3 normal = normalize(cross(dFdx(eyeSpacePos), dFdy(eyeSpacePos)));
  float depthThreshold = 0.1;

  if (abs(depth - depthUp) > depthThreshold || abs(depth - depthDown) > depthThreshold || abs(depth - depthLeft) > depthThreshold || abs(depth - depthRight) > depthThreshold) {
    outColor = vec4(0.0, 0.0, 0.0, 1.0);
  } else {
    outColor = fragColor;
  }
}
