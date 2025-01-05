// In your light_volume_fragment.glsl
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
    
    // Calculate distance to light
    vec3 lightDir = lightPos - FragPos;
    float distance = length(lightDir);
    
    // Check if fragment is within light radius
    if(distance > lightRadius) {
        discard;
    }
    
    lightDir = normalize(lightDir);
    
    // Calculate attenuation (make it fall off less aggressively)
    float attenuation = 1.0 - (distance / lightRadius);
    attenuation = pow(attenuation, 1.5); // Softer falloff
    
    // Diffuse lighting
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * Albedo;
    
    // Specular lighting
    vec3 viewDir = normalize(-FragPos); // In view space, camera is at origin
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * Specular;
    
    // Combine and apply intensity
    vec3 result = (diffuse + specular) * attenuation * lightIntensity;
    
    // Boost the final color to make it more visible
    result *= 2.0; // Increase overall brightness
    
    FragColor = vec4(result, 1.0);
}