// light_volume_vertex.glsl
#version 450 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 lightPos;
uniform float lightRadius;

void main() {
    // Scale and position the sphere based on light's position and radius
    vec3 worldPos = aPos * lightRadius + lightPos;
    gl_Position = projection * view * vec4(worldPos, 1.0);
}
