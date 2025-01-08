#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "forward_shader.h"
#include "deferred_renderer.h" 
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <random>
#include "LightSphere.h"

//cube
GLfloat vertices[] = {
    // Positions    // Normals        // Texture
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f
};
int debugView = 0;  
float mouseSensitivity = 0.05f;  


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); 
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool useForwardRendering = true;

//pour deferred rendering's lighting pass
float quadVertices[] = {
    // First triangle        // texCoords
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
     1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
     // Second triangle
     -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
      1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
      1.0f,  1.0f, 0.0f,     1.0f, 1.0f
};

int NUM_LIGHTS = 32;

struct PointLight {
    glm::vec4 position;  
    glm::vec4 color;     
    glm::vec4 velocity;  
};

std::vector<PointLight> lights;
LightSphere* lightSphere;
GLuint lightSSBO;
GLuint computeShader;
GLuint lightVolumeShader;

float RandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        useForwardRendering = !useForwardRendering;
        std::cout << "Switched to " << (useForwardRendering ? "Forward" : "Deferred") << " rendering" << std::endl;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;  
    static float lastX = 400, lastY = 300;  
    static float yaw = -90.0f;  
    static float pitch = 0.0f;  

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;  
    float yoffset = lastY - ypos;  
    lastX = xpos;  
    lastY = ypos;  

    const float mouseSensitivity = 0.05f;  

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window, float deltaTime, GLuint lightingShader) {
    const float cameraSpeed = 2.5f;  
    
    float velocity = cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += velocity * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= velocity * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    }

    if (!useForwardRendering) { 
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            debugView = 1;
            glUniform1i(glGetUniformLocation(lightingShader, "debugView"), 1);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            debugView = 2;
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glUniform1i(glGetUniformLocation(lightingShader, "debugView"), 2);

        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            debugView = 3;
            glUniform1i(glGetUniformLocation(lightingShader, "debugView"), 3);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            debugView = 4;
            glUniform1i(glGetUniformLocation(lightingShader, "debugView"), 4);
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            debugView = 5;
            glUniform1i(glGetUniformLocation(lightingShader, "debugView"), 5);
        }
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        {
            debugView = 0;
            glUniform1i(glGetUniformLocation(lightingShader, "debugView"), 0);
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //configuration glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Deferred Shading with GLEW", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //glew
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // viewport
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Enable depth testing pour rendering 3d
    glEnable(GL_DEPTH_TEST);

    GLuint forwardShader = LoadShader("shaders/forward_vertex.glsl", "shaders/forward_fragment.glsl");
    glfwSetKeyCallback(window, key_callback);

    GLuint geometryShader = LoadShader("shaders/geometry_vertex.glsl", "shaders/geometry_fragment.glsl");
    GLuint lightingShader = LoadShader("shaders/lighting_vertex.glsl", "shaders/lighting_fragment.glsl");

    lights.resize(NUM_LIGHTS);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        lights[i].position = glm::vec4(
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(0.5f, 1.0f)  //radius
        );
        lights[i].color = glm::vec4(
            RandomFloat(0.8f, 1.0f),
            RandomFloat(0.8f, 1.0f),
            RandomFloat(0.8f, 1.0f),
            10.0f  //intensity
        );
        lights[i].velocity = glm::vec4(
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(-1.0f, 1.0f),
            0.0f
        );
    }

    //SSBO
    glGenBuffers(1, &lightSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PointLight) * lights.size(), lights.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);

    // mesh sphere
    lightSphere = new LightSphere(16);

    computeShader = LoadComputeShader("shaders/lights_compute.glsl");
    lightVolumeShader = LoadShader("shaders/lights_volume_vertex.glsl", "shaders/lights_volume_fragment.glsl");

    DeferredRenderer deferredRenderer(800, 600);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //position 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // normal 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coordinate 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glfwSetCursorPosCallback(window, mouse_callback);
    glm::vec3 lightPositions[3] = {
        glm::vec3(0.0f, -0.5f, 1.0f),
        glm::vec3(0.5f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.5f, 1.0f)
    };

    glm::vec3 lightColors[3] = {
        glm::vec3(1.0f, 1.0f, 1.0f), //blanc
        glm::vec3(1.0f, 0.0f, -0.5f), //rouhe
        glm::vec3(-0.5f, 0.0f, 1.0f)  //bleu
    };

    float lastFrame = 0.0f;

    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime(); 
        float deltaTime = currentFrame - lastFrame; 
        lastFrame = currentFrame; 

        processInput(window, deltaTime, lightingShader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (useForwardRendering) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //background
            glUseProgram(computeShader);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);
            glUniform1f(glGetUniformLocation(computeShader, "deltaTime"), deltaTime);
            glUniform3f(glGetUniformLocation(computeShader, "boundingBox"), 1.0f, 1.0f, 1.0f);

            int numGroups = (NUM_LIGHTS + 255) / 256;
            glDispatchCompute(numGroups, 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glUseProgram(forwardShader);

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            glUniformMatrix4fv(glGetUniformLocation(forwardShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(forwardShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(forwardShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);

            glUniform3fv(glGetUniformLocation(forwardShader, "viewPos"), 1, glm::value_ptr(cameraPos));
            glUniform3f(glGetUniformLocation(forwardShader, "objectColor"), 0.3f, 0.3f, 0.3f);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
        else {  //mode deferred rendering 

            glUseProgram(0);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); 
            glBindVertexArray(0); 

            glUseProgram(computeShader);

            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);

            glUniform1f(glGetUniformLocation(computeShader, "deltaTime"), deltaTime);
            glUniform3f(glGetUniformLocation(computeShader, "boundingBox"), 1.0f, 1.0f, 1.0f);

            int numGroups = (NUM_LIGHTS + 255) / 256;
            glDispatchCompute(numGroups, 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            // 1. Geometry pass
            deferredRenderer.geometryPass(geometryShader, VAO, projection, view);
            GLuint error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "OpenGL Error after geometry pass: " << error << std::endl;
            }
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "Framebuffer is not complete!" << std::endl;
            }

            if (debugView > 0) {
                deferredRenderer.lightingPass(lightingShader, quadVAO, cameraPos);
            }
            else {
                // 2. Regular lighting pass
                deferredRenderer.lightingPass(lightingShader, quadVAO, cameraPos);

                // 3. Light volumes pass
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_GREATER);
                glDepthMask(GL_FALSE);

                glUseProgram(lightVolumeShader);

                GLint isLinked;
                glGetProgramiv(lightVolumeShader, GL_LINK_STATUS, &isLinked);
                if (!isLinked) {
                    std::cout << "Light volume shader not properly linked!" << std::endl;
                }

                glUniformMatrix4fv(glGetUniformLocation(lightVolumeShader, "projection"),
                    1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(lightVolumeShader, "view"),
                    1, GL_FALSE, glm::value_ptr(view));
                glUniform2f(glGetUniformLocation(lightVolumeShader, "screenSize"), 800.0f, 600.0f);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getAlbedoTexture());   
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gAlbedo"), 0);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getNormalTexture());   
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gNormal"), 1);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getPositionTexture());  
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gPosition"), 2);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getSpecularTexture()); 
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gSpecular"), 3);

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSSBO);
                PointLight* lightData = (PointLight*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

                if (lightData) {
                    for (int i = 0; i < NUM_LIGHTS; i++) {
                        glUniform3fv(glGetUniformLocation(lightVolumeShader, "lightPos"),
                            1, glm::value_ptr(lightData[i].position));
                        glUniform3fv(glGetUniformLocation(lightVolumeShader, "lightColor"),
                            1, glm::value_ptr(lightData[i].color));
                        glUniform1f(glGetUniformLocation(lightVolumeShader, "lightIntensity"),
                            lightData[i].color.w);
                        glUniform1f(glGetUniformLocation(lightVolumeShader, "lightRadius"),
                            lightData[i].position.w);

                        lightSphere->Draw();

                        GLenum error = glGetError();
                        if (error != GL_NO_ERROR) {
                            std::cout << "Error after drawing light " << i << ": " << error << std::endl;
                        }
                    }
                }

                glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

                glCullFace(GL_BACK);
                glDisable(GL_BLEND);
                glDepthFunc(GL_LESS);
                glDepthMask(GL_TRUE);
            }
        }
        // ImGui 
        ImGui::SetNextWindowPos(ImVec2(10, ImGui::GetIO().DisplaySize.y - 30));
        ImGui::Begin("Overlay", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings
        );

        std::string currentMode;
        if (useForwardRendering) {
            currentMode = "Forward Rendering";
        }
        else {
            switch (debugView) {
            case 0: currentMode = "Deferred Rendering"; break;
            case 1: currentMode = "Debug View: Position Buffer"; break;
            case 2: currentMode = "Debug View: Normal Buffer"; break;
            case 3: currentMode = "Debug View: Albedo Buffer"; break;
            case 4: currentMode = "Debug View: Specular Buffer"; break;
            case 5: currentMode = "Debug View: Depth Buffer"; break;
            }
        }
        ImGui::Text("%s", currentMode.c_str());

        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin("Performance Metrics", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings
        );
        ImGui::Text("Controls:");
        ImGui::Text("WASD - Move Camera");
        ImGui::Text("Mouse - Look Around");
        ImGui::Text("Space - Switch Rendering Mode");
        if (!useForwardRendering) {
            ImGui::Text("0-5 - Debug Views");
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(250, 10));
        ImGui::Begin("Light Controls", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings
        );

        static int newNumLights = NUM_LIGHTS;
        if (ImGui::SliderInt("Number of Lights", &newNumLights, 1, 150)) {
            if (newNumLights != NUM_LIGHTS) {
                lights.resize(newNumLights);

                for (int i = NUM_LIGHTS; i < newNumLights; i++) {
                    lights[i].position = glm::vec4(
                        RandomFloat(-1.0f, 1.0f),
                        RandomFloat(-1.0f, 1.0f),
                        RandomFloat(-1.0f, 1.0f),
                        RandomFloat(0.5f, 1.0f)  //radius
                    );
                    lights[i].color = glm::vec4(
                        RandomFloat(0.8f, 1.0f),
                        RandomFloat(0.8f, 1.0f),
                        RandomFloat(0.8f, 1.0f),
                        10.0f  // intensity
                    );
                    lights[i].velocity = glm::vec4(
                        RandomFloat(-1.0f, 1.0f),
                        RandomFloat(-1.0f, 1.0f),
                        RandomFloat(-1.0f, 1.0f),
                        0.0f
                    );
                }

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSSBO);
                glBufferData(GL_SHADER_STORAGE_BUFFER,
                    sizeof(PointLight) * lights.size(),
                    lights.data(),
                    GL_DYNAMIC_DRAW);

                NUM_LIGHTS = newNumLights;
            }
        }
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    delete lightSphere;
    glDeleteBuffers(1, &lightSSBO);
    glDeleteProgram(computeShader);
    glDeleteProgram(lightVolumeShader);
    glDeleteProgram(forwardShader);
    glDeleteProgram(geometryShader);
    glDeleteProgram(lightingShader);
    glfwTerminate();

    return 0;
}

