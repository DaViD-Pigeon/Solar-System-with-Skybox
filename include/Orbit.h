#ifndef __ORBIT_H__
#define __ORBIT_H__

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

#define M_PI 3.14159265358979323846

class Orbit {
public:
    Orbit(float radius, glm::vec3 color);
    ~Orbit();

    void Draw(Shader &shader_ ,glm::mat4 & projection, glm::mat4 &view, glm::mat4 &model);
    GLuint VAO, VBO;
    float orbitRadius;
    glm::vec3 orbitColor;
    std::vector<glm::vec3> vertices;

    void GenerateOrbitPath();
};

#endif // ORBIT_H
