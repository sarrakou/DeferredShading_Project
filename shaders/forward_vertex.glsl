#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    // Transform to view space first
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    FragPos = viewPos.xyz;
    
    // Transform normals to view space
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    
    TexCoords = aTexCoord;
    
    gl_Position = projection * viewPos;
}