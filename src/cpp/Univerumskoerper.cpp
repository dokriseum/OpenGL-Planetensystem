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

Universumskoerper::Universumskoerper(const char* path, float posX, float posY, float posZ){
    bool res = loadOBJ(path, vertices, uvs, normals);
    gameObjectModel = glm::mat4(1.0f);
    gameObjectModel = glm::translate(gameObjectModel, glm::vec3(posX, posY, posZ));
    gameObjectModel = glm::scale(gameObjectModel, glm::vec3(1, 1, 1));
}
Universumskoerper::Universumskoerper(const char* path, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ){
    bool res = loadOBJ(path, vertices, uvs, normals);
    gameObjectModel = glm::mat4(1.0f);
    gameObjectModel = glm::translate(gameObjectModel, glm::vec3(posX, posY, posZ));
    gameObjectModel = glm::scale(gameObjectModel, glm::vec3(scaleX, scaleY, scaleZ));
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
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);

}

void Universumskoerper::setTextures(const char *path, unsigned int programmID, int array_index) {
    /**
    glGenTextures(10, textures);
    glBindTexture(GL_TEXTURE_2D, textures[array_index]);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, path);
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     **/

}

glm::mat4 Universumskoerper::getObjekt() {
    return this->gameObjectModel;
}


RenderInformation Universumskoerper::getRenderInformation(){
    RenderInformation renderInformation = RenderInformation();
    renderInformation.renderModel = this->gameObjectModel;
    renderInformation.renderVertices = this->vertices;
    renderInformation.renderUvs = this->uvs;
    renderInformation.renderNormals = this->normals;
    renderInformation.maxDistSun = this->maxDistSun;
    renderInformation.minDistSun = this->minDistSun;

    return renderInformation;
}

