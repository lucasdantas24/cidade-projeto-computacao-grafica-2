#version 300 es

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightPosWorldSpace; // Alterado de lightDirWorldSpace para lightPosWorldSpace

out vec3 fragV;
out vec3 fragL;
out vec3 fragN;
out vec2 fragTexCoord;
out vec3 fragPObj;
out vec3 fragNObj;

void main() {
  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;

  // Calcula a direção da luz no espaço da visão
  vec3 lightPosViewSpace = (viewMatrix * lightPosWorldSpace).xyz; // Transforma a posição da luz para o espaço da visão
  vec3 L = lightPosViewSpace - P; // Direção da luz = posição da luz - posição do vértice

  fragL = L;
  fragV = -P;
  fragN = N;
  fragTexCoord = inTexCoord;
  fragPObj = inPosition;
  fragNObj = inNormal;

  gl_Position = projMatrix * vec4(P, 1.0);
}
