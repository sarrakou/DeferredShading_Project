#version 450 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;  
in vec2 TexCoords;

struct PointLight {
    vec4 position;  
    vec4 color;     
    vec4 velocity;
};

layout(std430, binding = 0) buffer LightBuffer {
    PointLight lights[];
};

uniform mat4 view;  
uniform vec3 viewPos;
uniform vec3 objectColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos); 
    vec3 result = vec3(0.0);
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * objectColor;
    result += ambient;

    for(int i = 0; i < lights.length(); i++) {
        //transformation to view space
        vec3 lightPos = vec3(view * vec4(lights[i].position.xyz, 1.0));
        float lightRadius = lights[i].position.w;
        vec3 lightColor = lights[i].color.xyz;
        float lightIntensity = lights[i].color.w;
        
        vec3 lightDir = normalize(lightPos - FragPos);
        float distance = length(lightPos - FragPos);
       
        if(distance > lightRadius) continue;
        
        float attenuation = 1.0 - (distance / lightRadius);
        attenuation = max(0.0, attenuation);
        
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * objectColor;
        
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * lightColor;
        
        result += (diffuse + specular) * attenuation * lightIntensity;
    }
    
    FragColor = vec4(result, 1.0);
}