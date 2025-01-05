// debug_fragment.glsl
#version 450 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D gBuffer;

void main() {
    vec4 texColor = texture(gBuffer, TexCoords);
    FragColor = texColor;
}