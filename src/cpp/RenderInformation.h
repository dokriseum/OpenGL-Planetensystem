#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#ifndef _RENDERINFORMATION_H
#define _RENDERINFORMATION_H
//1
struct RenderInformation {
	glm::mat4 renderModel;
	std::vector<glm::vec3> renderVertices;
	std::vector<glm::vec2> renderUvs;
	std::vector<glm::vec3> renderNormals;
	glm::vec3 maxDistSun;
	glm::vec3 minDistSun;
	GLuint renderVertexArray;
	glm::vec3 renderCoordinates;
};

#endif

