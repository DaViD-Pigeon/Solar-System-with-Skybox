#ifndef SPHERE_H
#define SPHERE_H
// #include <glad/glad.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.h"

#define X_SEGMENTS 30
#define Y_SEGMENTS 30

const GLfloat PI= 3.14159265358979323846f;


class Sphere{
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<float>sphereVertices;
    std::vector<unsigned int>sphereIndices;
    std::vector<float>textureCoords;
    glm::vec3 Color;
    unsigned int NBO;
    unsigned int TBO;

    Sphere(float radius,glm::vec3 Color);
    void draw(Shader &shader_ ,glm::mat4 & projection, glm::mat4 &view, glm::mat4 &model,unsigned int texture);
    ~Sphere();
};

#endif