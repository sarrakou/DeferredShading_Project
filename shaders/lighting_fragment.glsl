
#version 450 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;
uniform sampler2D depthTexture; 

uniform int debugView;
uniform vec3 viewPos;

uniform vec3 lightPositions[3];  
uniform vec3 lightColors[3];    

void main() {
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    float Shininess = texture(gSpecular, TexCoords).a;

    if(debugView == 1) {
        vec3 pos = FragPos * 0.5 + 0.5;  //view space to [0,1] range
        FragColor = vec4(pos, 1.0);      //position buffer

    }
    else if(debugView == 2) {
        //normal buffer 
        vec3 normalColor = Normal * 0.5 + 0.5;  // normal to [0,1] range
        FragColor = vec4(normalColor, 1.0);

    }
    else if(debugView == 3) {
        //albedo 
        FragColor = vec4(Albedo, 1.0);
    }
    else if(debugView == 4) {
        //Specular 
        vec4 specularValue = texture(gSpecular, TexCoords);
        vec4 albedoAlpha = texture(gAlbedo, TexCoords);
        if(albedoAlpha.a < 0.1) {
            FragColor = vec4(0.0, 0.0, 0.0, 1.0);  //noir
        } else {
            FragColor = vec4(specularValue.rgb, 1.0);  //blanc
        }

    } else if(debugView == 5) {
        float depth = texture(depthTexture, TexCoords).r;
        FragColor = vec4(vec3(depth), 1.0);    
    
    }
    else {
        vec3 lighting = Albedo * 0.1; 
        vec3 viewDir = normalize(viewPos - FragPos);

        for(int i = 0; i < 3; i++) {
            vec3 lightDir = normalize(lightPositions[i] - FragPos);
            
            float diff = max(dot(Normal, lightDir), 0.0);
            vec3 diffuse = diff * lightColors[i] * Albedo;

            vec3 reflectDir = reflect(-lightDir, Normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = spec * lightColors[i] * Shininess;

            float distance = length(lightPositions[i] - FragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

            lighting += (diffuse + specular) * attenuation;
        }

        FragColor = vec4(lighting, 1.0);
    }
}