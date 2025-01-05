#version 450 core
out vec4 FragColor;

in vec3 Normal;    // Normal at the fragment
in vec3 FragPos;   // Position of the fragment in world space
in vec2 TexCoords; // Texture coordinates (if used)

uniform vec3 lightPos[3];  // Array of light positions (3 lights)
uniform vec3 lightColor[3]; // Array of light colors (3 lights)
uniform vec3 viewPos;    // Position of the camera

uniform vec3 objectColor; // Color of the object

void main() {
    vec3 result = vec3(0.0);

    // Loop through each light
    for (int i = 0; i < 3; i++) {
        // Normalizing the normal vector
        vec3 norm = normalize(Normal);

        // Light direction and diffuse calculation
        vec3 lightDir = normalize(lightPos[i] - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor[i];

        // Specular reflection
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess exponent
        vec3 specular = spec * lightColor[i];

        // Accumulate results from all lights
        result += (diffuse + specular);
    }

    // Apply object color and output final color
    FragColor = vec4(result * objectColor, 1.0);
}
