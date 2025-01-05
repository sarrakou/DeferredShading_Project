#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

// Function prototypes
GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
// For compute shaders
GLuint LoadComputeShader(const char* computePath);
#endif

