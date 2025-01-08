#version 450 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRadius;
uniform vec2 screenSize;

void main() {
    vec2 texCoords = gl_FragCoord.xy / screenSize;
    
    vec3 FragPos = texture(gPosition, texCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, texCoords).rgb);
    vec3 Albedo = texture(gAlbedo, texCoords).rgb;
    float Specular = texture(gSpecular, texCoords).r;
    
    vec3 lightDir = lightPos - FragPos;
    float distance = length(lightDir);
    
    if(distance > lightRadius) {
        discard;
    }
    
    lightDir = normalize(lightDir);
    
    float attenuation = 1.0 - (distance / lightRadius);
    attenuation = pow(attenuation, 1.5); // Softer falloff
    
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * Albedo;
    
    vec3 viewDir = normalize(-FragPos); // In view space, camera is at origin
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * Specular;
    
    vec3 result = (diffuse + specular) * attenuation * lightIntensity;
    
    result *= 2.0; //more brightness
    
    FragColor = vec4(result, 1.0);
}