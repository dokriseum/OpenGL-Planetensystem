#ifndef _UNIVERSUMSKOERPER_H
#define _UNIVERSUMSKOERPER_H

#include "GLFW/glfw3.h"
#include "asset.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>



//1
class Universumskoerper {

private:

public:
    std::string name;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    glm::mat4 gameObjectModel;
    double posX;
    double posZ;
    double posY;
    double phi;
    double yearLength;
    double minDistSun;
    double maxDistSun;
    glm::vec3 renderCoordinates;
    GLuint renderVertexArray;
    unsigned int programID;
    unsigned int textureID;

    Universumskoerper(std::string name,double yearLength, const char* pathTexture, double minDistSun, double maxDistSun);
    Universumskoerper(std::string name,double yearLength, const char* pathTexture, double minDistSun, double maxDistSun, float scaleX, float scaleY, float scaleZ);
    ~Universumskoerper();
    void setPosition(float posX, float posY, float posZ);
    void setScale(float scaleX, float scaleY, float scaleZ);
    void setTexture(const char* path, unsigned int programmID);
    glm::mat4 getObjekt();
    void setLeuchtkraft(int programmID);

    GLuint texture;
};
#endif
