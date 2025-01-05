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

// Cube vertex data
GLfloat vertices[] = {
    // Positions        // Normals         // Texture Coords
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
int debugView = 0;  // Add this where your other globals are

float yaw = -90.0f;  // Yaw (degrees)
float pitch = 0.0f;  // Pitch (degrees)
float lastX = 400, lastY = 300;  // Initial mouse position (center)
bool firstMouse = true;  // Flag to check first mouse movement

// Movement speed and sensitivity
float cameraSpeed = 2.5f;  // Camera movement speed
float mouseSensitivity = 0.05f;  // Mouse sensitivity


// Camera parameters
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Camera position
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Camera front direction
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Camera up direction

bool useForwardRendering = true;

// Add quad vertices for deferred rendering's lighting pass
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

const int NUM_LIGHTS = 32;
// Add to your header file or main.cpp
struct PointLight {
    glm::vec4 position;  // w component can store radius
    glm::vec4 color;     // w component can store intensity
    glm::vec4 velocity;  // For movement
};

std::vector<PointLight> lights;
LightSphere* lightSphere;
GLuint lightSSBO;
GLuint computeShader;
GLuint lightVolumeShader;

// Add this helper function for random numbers
float RandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

// Add key callback for switching between rendering modes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        useForwardRendering = !useForwardRendering;
        std::cout << "Switched to " << (useForwardRendering ? "Forward" : "Deferred") << " rendering" << std::endl;
    }
}

// Mouse callback function (to adjust the camera's rotation based on mouse movement)
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;  // To prevent sudden jump of the cursor
    static float lastX = 400, lastY = 300;  // Initial mouse position
    static float yaw = -90.0f;  // Initial yaw (degrees)
    static float pitch = 0.0f;  // Initial pitch (degrees)

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;  // Calculate offset of the mouse's X position
    float yoffset = lastY - ypos;  // Calculate offset of the mouse's Y position (inverted)
    lastX = xpos;  // Update last mouse X position
    lastY = ypos;  // Update last mouse Y position

    const float mouseSensitivity = 0.05f;  // Mouse sensitivity

    // Adjust the offsets based on the sensitivity
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    // Update the yaw and pitch based on the mouse movement
    yaw += xoffset;
    pitch += yoffset;

    // Constrain the pitch to avoid flipping the camera upside down
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate the new front vector based on the yaw and pitch
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // Normalize the front vector and update the global camera front direction
    cameraFront = glm::normalize(front);
}

// Function to handle camera movement via keyboard input (WASD keys)
void processInput(GLFWwindow* window, float deltaTime, GLuint lightingShader) {
    const float cameraSpeed = 2.5f;  // Camera speed (adjust as needed)

    // Calculate the velocity based on deltaTime (time between frames)
    float velocity = cameraSpeed * deltaTime;

    // Move camera forward (W key)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += velocity * cameraFront;
    }
    // Move camera backward (S key)
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= velocity * cameraFront;
    }
    // Move camera left (A key)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    }
    // Move camera right (D key)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    }

    // Add debug view controls
    if (!useForwardRendering) {  // Only check these keys when in deferred mode
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            debugView = 1;
            glUniform1i(glGetUniformLocation(lightingShader, "debugView"), 1);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            debugView = 2;
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

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Deferred Shading with GLEW", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL functions using GLEW
    glewExperimental = GL_TRUE; // Enable modern OpenGL features
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set viewport
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);

    GLuint forwardShader = LoadShader("shaders/forward_vertex.glsl", "shaders/forward_fragment.glsl");
    // Add key callback
    glfwSetKeyCallback(window, key_callback);

    // Load both forward and deferred shaders
    GLuint geometryShader = LoadShader("shaders/geometry_vertex.glsl", "shaders/geometry_fragment.glsl");
    GLuint lightingShader = LoadShader("shaders/lighting_vertex.glsl", "shaders/lighting_fragment.glsl");
    GLuint debugShader = LoadShader("shaders/debug_vertex.glsl", "shaders/debug_fragment.glsl");


    // Initialize lights
    lights.resize(NUM_LIGHTS);
    for (int i = 0; i < NUM_LIGHTS; i++) {
        lights[i].position = glm::vec4(
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(-1.0f, 1.0f),
            RandomFloat(0.5f, 1.0f)  // radius
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

    // Create SSBO for lights
    glGenBuffers(1, &lightSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PointLight) * lights.size(), lights.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);

    // Create sphere mesh for light volumes
    lightSphere = new LightSphere(16);

    // Load compute and light volume shaders
    computeShader = LoadComputeShader("shaders/lights_compute.glsl");
    lightVolumeShader = LoadShader("shaders/lights_volume_vertex.glsl", "shaders/lights_volume_fragment.glsl");

    // Create deferred renderer
    DeferredRenderer deferredRenderer(800, 600);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Inside main(), set the mouse callback function
    glfwSetCursorPosCallback(window, mouse_callback);
    // Set light positions and colors
    glm::vec3 lightPositions[3] = {
        glm::vec3(0.0f, -0.5f, 1.0f),
        glm::vec3(0.5f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.5f, 1.0f)
    };

    glm::vec3 lightColors[3] = {
        glm::vec3(1.0f, 1.0f, 1.0f), // White light
        glm::vec3(1.0f, 0.0f, -0.5f), // Red light
        glm::vec3(-0.5f, 0.0f, 1.0f)  // Blue light
    };

    float lastFrame = 0.0f;

    // In main, when setting up quad VAO
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Verify these attribute locations match your lighting vertex shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


    // After your GLFW window creation and before the main loop
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Inside the render loop
        float currentFrame = glfwGetTime();  // Get current frame time
        float deltaTime = currentFrame - lastFrame;  // Calculate time between frames
        lastFrame = currentFrame;  // Update last frame time

        // Call the processInput function to update the camera position
        processInput(window, deltaTime, lightingShader);

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (useForwardRendering) {
            // Clear screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use the shader program
            glUseProgram(forwardShader);

            // Set uniform variables for your shader (e.g., transformation matrices, light positions, etc.)
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            glUniformMatrix4fv(glGetUniformLocation(forwardShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(forwardShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(forwardShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Set the light positions and colors
            for (int i = 0; i < 3; i++) {
                std::string posName = "lightPos[" + std::to_string(i) + "]";
                std::string colorName = "lightColor[" + std::to_string(i) + "]";
                glUniform3fv(glGetUniformLocation(forwardShader, posName.c_str()), 1, glm::value_ptr(lightPositions[i]));
                glUniform3fv(glGetUniformLocation(forwardShader, colorName.c_str()), 1, glm::value_ptr(lightColors[i]));
            }

            // Set view position (camera position)
            glUniform3f(glGetUniformLocation(forwardShader, "viewPos"), 0.0f, 0.0f, 3.0f);

            // Set object color (can change this to other colors)
            glUniform3f(glGetUniformLocation(forwardShader, "objectColor"), 0.3f, 0.3f, 0.3f);

            // Draw the cube
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);  // Draw the cube with 36 vertices (6 faces, 2 triangles per face)
            glBindVertexArray(0);
        }
        else {  // Deferred rendering mode

            glUseProgram(0);  // Unbind any shader program
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);  // Unbind any SSBO
            glBindVertexArray(0);  // Unbind any VAO

            // Update lights using compute shader
            glUseProgram(computeShader);

            // Ensure SSBO is bound
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightSSBO);

            glUniform1f(glGetUniformLocation(computeShader, "deltaTime"), deltaTime);
            glUniform3f(glGetUniformLocation(computeShader, "boundingBox"), 1.0f, 1.0f, 1.0f);

            // Calculate number of work groups needed
            int numGroups = (NUM_LIGHTS + 255) / 256;
            glDispatchCompute(numGroups, 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            // Setup matrices
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            // 1. Geometry pass
            deferredRenderer.geometryPass(geometryShader, VAO, projection, view);
            // After geometry pass
            GLuint error = glGetError();
            if (error != GL_NO_ERROR) {
                std::cout << "OpenGL Error after geometry pass: " << error << std::endl;
            }
            // Check framebuffer status
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "Framebuffer is not complete!" << std::endl;
            }

            // 2. If in debug mode, only render debug view
            if (debugView > 0) {
                deferredRenderer.lightingPass(lightingShader, quadVAO, cameraPos);
            }
            else {
                // 3. Regular lighting pass
                deferredRenderer.lightingPass(lightingShader, quadVAO, cameraPos);

                // 4. Light volumes pass
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_GREATER);
                glDepthMask(GL_FALSE);

                glUseProgram(lightVolumeShader);

                // Verify shader program
                GLint isLinked;
                glGetProgramiv(lightVolumeShader, GL_LINK_STATUS, &isLinked);
                if (!isLinked) {
                    std::cout << "Light volume shader not properly linked!" << std::endl;
                }

                // Set uniforms for light volume shader
                glUniformMatrix4fv(glGetUniformLocation(lightVolumeShader, "projection"),
                    1, GL_FALSE, glm::value_ptr(projection));
                glUniformMatrix4fv(glGetUniformLocation(lightVolumeShader, "view"),
                    1, GL_FALSE, glm::value_ptr(view));
                glUniform2f(glGetUniformLocation(lightVolumeShader, "screenSize"), 800.0f, 600.0f);

                // Bind G-Buffer textures matching uniform locations
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getAlbedoTexture());    // gAlbedo is location 0
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gAlbedo"), 0);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getNormalTexture());    // gNormal is location 1
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gNormal"), 1);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getPositionTexture());  // gPosition is location 2
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gPosition"), 2);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, deferredRenderer.getSpecularTexture());  // gSpecular is location 3
                glUniform1i(glGetUniformLocation(lightVolumeShader, "gSpecular"), 3);

                // Get updated light data and render each light
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

                // Restore OpenGL state
                glCullFace(GL_BACK);
                glDisable(GL_BLEND);
                glDepthFunc(GL_LESS);
                glDepthMask(GL_TRUE);
            }
        }
        // ImGui overlay
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

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup
    glfwDestroyWindow(window);
    // Cleanup
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

