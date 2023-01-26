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

void Universumskoerper::setLeuchtkraft(int programmID){
    /**
    GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_ambient_color[] = {0.0, 0.0, 0.0, 0.0};
    GLfloat light_diffuse_color[] = {1.0, 1.0, 1.0, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_color);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColorMaterial ( GL_FRONT, GL_DIFFUSE ) ;
    glEnable(GL_COLOR_MATERIAL);
    **/

    /**
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat pos[] = { 0.0, 0.0, 1.0, 0.0 };
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
     **/

    /**
    glm::vec3 lightPos = this->gameObjectModel * glm::vec4(0.0f, 0.3f, 0.0f, 1.0f);//glm::vec3(4,4,‐4);
    glUniform3f(glGetUniformLocation(programmID, "LightPosition_worldspace"), lightPos.x, lightPos.y,
                lightPos.z);
                **/
}

glm::mat4 Universumskoerper::getObjekt() {
    return this->gameObjectModel;
}


