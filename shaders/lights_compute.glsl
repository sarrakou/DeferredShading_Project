#version 450

layout(local_size_x = 256) in;  

struct PointLight {
    vec4 position; 
    vec4 color;  
    vec4 velocity;
};

layout(std430, binding = 0) buffer LightBuffer {
    PointLight lights[];
};

uniform float deltaTime;
uniform vec3 boundingBox; 

void main() {
    uint index = gl_GlobalInvocationID.x;
    if (index >= lights.length()) return;

    lights[index].position.xyz += lights[index].velocity.xyz * deltaTime;

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

    float time = deltaTime * index;
    lights[index].color.w = 0.5 + 0.5 * sin(time); // Intensity variation
    lights[index].position.w = 2.0 + sin(time) * 0.05; // Radius variation
}