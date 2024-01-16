#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include <iostream>
unsigned int load_texture(const char * path);
#endif