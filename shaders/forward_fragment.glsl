#version 450 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;  // Now in view space
in vec2 TexCoords;

struct PointLight {
    vec4 position;  // w is radius
    vec4 color;     // w is intensity
    vec4 velocity;
};

layout(std430, binding = 0) buffer LightBuffer {
    PointLight lights[];
};

uniform mat4 view;  // Need view matrix to transform light positions
uniform vec3 viewPos;
uniform vec3 objectColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);  // In view space, camera is at (0,0,0)
    vec3 result = vec3(0.0);
    
    // Ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * objectColor;
    result += ambient;

    // Calculate contribution from each light
    for(int i = 0; i < lights.length(); i++) {
        // Transform light position to view space
        vec3 lightPos = vec3(view * vec4(lights[i].position.xyz, 1.0));
        float lightRadius = lights[i].position.w;
        vec3 lightColor = lights[i].color.xyz;
        float lightIntensity = lights[i].color.w;
        
        vec3 lightDir = normalize(lightPos - FragPos);
        float distance = length(lightPos - FragPos);
        
        // Skip if beyond light radius
        if(distance > lightRadius) continue;
        
        // Attenuation
        float attenuation = 1.0 - (distance / lightRadius);
        attenuation = max(0.0, attenuation);
        
        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * objectColor;
        
        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * lightColor;
        
        result += (diffuse + specular) * attenuation * lightIntensity;
    }
    
    FragColor = vec4(result, 1.0);
}