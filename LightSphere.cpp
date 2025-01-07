#include "LightSphere.h"
#include <iostream>

LightSphere::LightSphere(int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    GenerateMesh(segments);
}

LightSphere::~LightSphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void LightSphere::GenerateMesh(int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (int y = 0; y <= segments; y++) {
        for (int x = 0; x <= segments; x++) {
            float xSegment = (float)x / (float)segments;
            float ySegment = (float)y / (float)segments;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
        }
    }

    for (int y = 0; y < segments; y++) {
        for (int x = 0; x < segments; x++) {
            indices.push_back((y + 1) * (segments + 1) + x);
            indices.push_back(y * (segments + 1) + x);
            indices.push_back(y * (segments + 1) + x + 1);

            indices.push_back((y + 1) * (segments + 1) + x);
            indices.push_back(y * (segments + 1) + x + 1);
            indices.push_back((y + 1) * (segments + 1) + x + 1);
        }
    }

    indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void LightSphere::Draw() {

    glBindVertexArray(VAO);

    GLint currentVAO;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}