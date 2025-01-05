#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calculate view-space position
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    FragPos = viewPos.xyz;
    
    // Transform normal to view space
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    Normal = normalMatrix * aNormal;
    
    TexCoords = aTexCoords;
    
    // Final position
    gl_Position = projection * viewPos;
}