// lights_compute.glsl
#version 450

layout(local_size_x = 256) in;  // Add this line

struct PointLight {
    vec4 position;  // w is radius
    vec4 color;     // w is intensity
    vec4 velocity;
};

layout(std430, binding = 0) buffer LightBuffer {
    PointLight lights[];
};

uniform float deltaTime;
uniform vec3 boundingBox;  // Scene boundaries

void main() {
    uint index = gl_GlobalInvocationID.x;
    if (index >= lights.length()) return;

    // Update position
    lights[index].position.xyz += lights[index].velocity.xyz * deltaTime;

    // Bounce off boundaries
    vec3 pos = lights[index].position.xyz;
    vec3 vel = lights[index].velocity.xyz;
    
    if (pos.x < -boundingBox.x || pos.x > boundingBox.x) {
        lights[index].velocity.x = -vel.x;
    }
    if (pos.y < -boundingBox.y || pos.y > boundingBox.y) {
        lights[index].velocity.y = -vel.y;
    }
    if (pos.z < -boundingBox.z || pos.z > boundingBox.z) {
        lights[index].velocity.z = -vel.z;
    }

    // Vary color and intensity over time
    float time = deltaTime * index;
    lights[index].color.w = 0.5 + 0.5 * sin(time); // Intensity variation
    lights[index].position.w = 2.0 + sin(time) * 0.05; // Radius variation
}