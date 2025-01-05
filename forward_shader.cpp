#include "forward_shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

GLuint LoadShader(const char* vertexPath, const char* fragmentPath) {
    // Read shader files
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Compile shaders
    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Check for compile errors
    GLint success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}

// For compute shaders
GLuint LoadComputeShader(const char* computePath) {
    std::string computeCode;
    std::ifstream cShaderFile;

    // Ensure ifstream objects can throw exceptions
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open file
        cShaderFile.open(computePath);
        std::stringstream cShaderStream;

        // Read file's buffer contents into streams
        cShaderStream << cShaderFile.rdbuf();

        // Close file handlers
        cShaderFile.close();

        // Convert stream into string
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* cShaderCode = computeCode.c_str();

    // Create compute shader
    GLuint compute = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(compute, 1, &cShaderCode, NULL);
    glCompileShader(compute);

    // Check for compile errors
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(compute, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, compute);
    glLinkProgram(program);

    // Check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shader as it's linked into program and no longer necessary
    glDeleteShader(compute);

    return program;
}