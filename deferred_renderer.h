#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

struct GBuffer {
    GLuint gBuffer;
    GLuint gPosition;
    GLuint gNormal;
    GLuint gAlbedo;
    GLuint gSpecular;
    GLuint rboDepth;
};

class DeferredRenderer {
public:
    GLuint getPositionTexture() const { return gBuffer.gPosition; }
    GLuint getNormalTexture() const { return gBuffer.gNormal; }
    GLuint getAlbedoTexture() const { return gBuffer.gAlbedo; }
    GLuint getSpecularTexture() const { return gBuffer.gSpecular; }
    DeferredRenderer(unsigned int width, unsigned int height);
    ~DeferredRenderer();

    void geometryPass(GLuint geometryShader, GLuint VAO,
        const glm::mat4& projection, const glm::mat4& view);
    void lightingPass(GLuint lightingShader, GLuint quadVAO, const glm::vec3& cameraPos);
    void debugView(GLuint debugShader, GLuint VAO);


private:
    GBuffer gBuffer;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
};