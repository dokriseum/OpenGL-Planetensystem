#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objloader.hpp"
#include "texture.hpp"
using namespace glm;
#include "shader.hpp"
#include "objects.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Universumskoerper.h"

Universumskoerper::Universumskoerper(char* name,const char* pathTexture, float posX, float posY, float posZ){
    this->name=name;
    bool res = loadOBJ(RESOURCES_DIR "/sphere.obj", vertices, uvs, normals);
    gameObjectModel = glm::mat4(1.0f);
    gameObjectModel = glm::translate(gameObjectModel, glm::vec3(posX, posY, posZ));
    gameObjectModel = glm::scale(gameObjectModel, glm::vec3(1, 1, 1));
    setTexture(pathTexture,programID);
}
Universumskoerper::Universumskoerper(char* name, const char* pathTexture, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ){
    this->name=name;
    bool res = loadOBJ(RESOURCES_DIR "/sphere.obj", vertices, uvs, normals);
    gameObjectModel = glm::mat4(1.0f);
    gameObjectModel = glm::translate(gameObjectModel, glm::vec3(posX, posY, posZ));
    gameObjectModel = glm::scale(gameObjectModel, glm::vec3(scaleX, scaleY, scaleZ));
    setTexture(pathTexture,programID);
}
Universumskoerper::~Universumskoerper() {}

void Universumskoerper::setPosition(float posX, float posY, float posZ) {
    gameObjectModel = glm::translate(gameObjectModel, glm::vec3(posX, posY, posZ));
}

void Universumskoerper::setScale(float scaleX, float scaleY, float scaleZ) {
    gameObjectModel = glm::scale(gameObjectModel, glm::vec3(scaleX, scaleY, scaleZ));
}

void Universumskoerper::setTexture(const char *path, unsigned int programmID) {
    this->texture = loadBMP_custom(path);
}

glm::mat4 Universumskoerper::getObjekt() {
    return this->gameObjectModel;
}


