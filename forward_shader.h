#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>

GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
GLuint LoadComputeShader(const char* computePath);
#endif

