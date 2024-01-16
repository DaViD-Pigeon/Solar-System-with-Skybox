#ifndef __CUBETEXTURE_H__
#define __CUBETEXTURE_H__
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h"
using namespace std;

unsigned int loadCubemap(vector<std::string> faces);

#endif