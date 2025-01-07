#include "deferred_renderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <string>

DeferredRenderer::DeferredRenderer(unsigned int width, unsigned int height)
    : SCR_WIDTH(width), SCR_HEIGHT(height) {
    // Create framebuffer
    glGenFramebuffers(1, &gBuffer.gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.gBuffer);

    // Position buffer
    glGenTextures(1, &gBuffer.gPosition);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBuffer.gPosition, 0);

    // Normal buffer
    glGenTextures(1, &gBuffer.gNormal);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBuffer.gNormal, 0);

    // Albedo buffer
    glGenTextures(1, &gBuffer.gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBuffer.gAlbedo, 0);

    // Specular buffer
    glGenTextures(1, &gBuffer.gSpecular);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gSpecular);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gBuffer.gSpecular, 0);

    // Tell OpenGL which attachments we'll use
    GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                             GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);

    // Create and attach depth buffer
    glGenRenderbuffers(1, &gBuffer.rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, gBuffer.rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, gBuffer.rboDepth);

    // Check framebuffer completion
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::geometryPass(GLuint geometryShader, GLuint VAO,
    const glm::mat4& projection, const glm::mat4& view) {

    glUseProgram(geometryShader);

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Check uniforms before setting them
    GLint projLoc = glGetUniformLocation(geometryShader, "projection");
    GLint viewLoc = glGetUniformLocation(geometryShader, "view");
    GLint modelLoc = glGetUniformLocation(geometryShader, "model");

    if (projLoc == -1 || viewLoc == -1 || modelLoc == -1) {
        std::cout << "Failed to find uniform locations in geometry shader" << std::endl;
    }

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

void DeferredRenderer::lightingPass(GLuint lightingShader, GLuint quadVAO, const glm::vec3& cameraPos) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(lightingShader);

    // Check debugView value and uniform location
    GLint debugLoc = glGetUniformLocation(lightingShader, "debugView");
    if (debugLoc == -1) {
        std::cout << "Warning: debugView uniform not found!" << std::endl;
    }

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

    // Set light uniforms
    for (int i = 0; i < 3; i++) {
        std::string posName = "lightPositions[" + std::to_string(i) + "]";
        std::string colorName = "lightColors[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(lightingShader, posName.c_str()), 1, glm::value_ptr(lightPositions[i]));
        glUniform3fv(glGetUniformLocation(lightingShader, colorName.c_str()), 1, glm::value_ptr(lightColors[i]));
    }

    // Set view position
    glUniform3fv(glGetUniformLocation(lightingShader, "viewPos"), 1, glm::value_ptr(cameraPos));

    // Bind G-Buffer textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gAlbedo);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gSpecular);

    glUniform1i(glGetUniformLocation(lightingShader, "gPosition"), 0);
    glUniform1i(glGetUniformLocation(lightingShader, "gNormal"), 1);
    glUniform1i(glGetUniformLocation(lightingShader, "gAlbedo"), 2);
    glUniform1i(glGetUniformLocation(lightingShader, "gSpecular"), 3);
    // Draw quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

DeferredRenderer::~DeferredRenderer() {
    glDeleteFramebuffers(1, &gBuffer.gBuffer);
    glDeleteTextures(1, &gBuffer.gPosition);
    glDeleteTextures(1, &gBuffer.gNormal);
    glDeleteTextures(1, &gBuffer.gAlbedo);
    glDeleteTextures(1, &gBuffer.gSpecular);
    glDeleteRenderbuffers(1, &gBuffer.rboDepth);
}

void DeferredRenderer::debugView(GLuint debugShader, GLuint quadVAO) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(debugShader);

    // Bind G-Buffer texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);  // Change this to view different buffers
    glUniform1i(glGetUniformLocation(debugShader, "gBuffer"), 0);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}