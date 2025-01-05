#pragma once
#include <GL/glew.h>
#include <vector>
#include <cmath>

class LightSphere {
public:
    LightSphere(int segments = 16);
    ~LightSphere();
    void Draw();

private:
    void GenerateMesh(int segments);
    GLuint VAO, VBO, EBO;
    int indexCount;
    const float PI = 3.14159265359f;
};