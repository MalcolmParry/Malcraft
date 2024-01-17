#version 450

layout(location = 0) in vec2 fragPos;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat uint fragTex;
layout(location = 3) in vec3 fNormal;
layout(location = 4) in vec3 fSunDir;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSamplers[3];

float CalcLighting() {
     float lighting = 0.0f;

     // ambient
     lighting += 0.1f;

     // diffuse
     vec3 norm = normalize(fNormal);
     vec3 lightDir = normalize(fSunDir);
     float diff = max(dot(norm, lightDir), 0.0);
     lighting += diff;

     return lighting;
}

void main() {
     outColor = texture(texSamplers[fragTex], fragTexCoord) * CalcLighting();
}