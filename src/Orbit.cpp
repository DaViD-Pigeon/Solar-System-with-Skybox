#include "Orbit.h"

Orbit::Orbit(float radius, glm::vec3 color) : orbitRadius(radius), orbitColor(color) {
    GenerateOrbitPath();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Orbit::~Orbit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Orbit::Draw(Shader &shader_ ,glm::mat4 & projection, glm::mat4 &view, glm::mat4 &model){
    shader_.use();
    shader_.setVec3("objectColor", glm::vec3(1.0f,1.0f,1.0f));
    shader_.set_3trans_matrix(glm::value_ptr(projection),glm::value_ptr(view),glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}

void Orbit::GenerateOrbitPath() {
    int numSegments = 1000;
    for (int i = 0; i < numSegments; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = orbitRadius * cos(theta);
        float y = orbitRadius * sin(theta);
        vertices.push_back(glm::vec3(x, y, 0.0f));
    }
}
