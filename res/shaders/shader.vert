#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in uint tex;
layout(location = 3) in vec3 normal;
layout(location = 4) in mat4 model;

layout(location = 0) out vec2 fragPos;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out flat uint fragTex;
layout(location = 3) out vec3 fNormal;
layout(location = 4) out vec3 fSunDir;

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    vec3 sunDir;
} ubo;

void main() {
    gl_Position = ubo.proj * ubo.view * model * vec4(inPosition, 0.0, 1.0);
    fragPos = inPosition;
    fragTexCoord = texCoords;
    fragTex = tex;
    fNormal = normal;
    fSunDir = ubo.sunDir;
}