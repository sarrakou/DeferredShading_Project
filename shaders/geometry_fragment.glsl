#version 450 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpecular;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main() {
    // Store the view-space position
    float depth = gl_FragCoord.z;
    gPosition = vec4(FragPos, depth);  // Store depth in w component
    gNormal = normalize(Normal);
    gAlbedo = vec4(0.3, 0.3, 0.3, 1.0);
    gSpecular = vec4(1.0, 1.0, 1.0, 32.0);
}