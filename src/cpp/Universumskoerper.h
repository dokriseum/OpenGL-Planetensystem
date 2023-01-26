#ifndef _UNIVERSUMSKOERPER_H
#define _UNIVERSUMSKOERPER_H

#include "GLFW/glfw3.h"
#include "RenderInformation.h"
#include "asset.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>



//1
class Universumskoerper {

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> location;
    unsigned int rotation;
    unsigned int programID;
    unsigned int textureID;
    GLuint texture;
    GLuint textures[10];
    glm::mat4 gameObjectModel;
    glm::vec3 minDistSun;
    glm::vec3 maxDistSun;


public:
    Universumskoerper(const char* path, float posX, float posY, float posZ);
    Universumskoerper(const char* path, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);
    ~Universumskoerper();
    RenderInformation getRenderInformation();
    void setPosition(float posX, float posY, float posZ);
    void setScale(float scaleX, float scaleY, float scaleZ);
    void setTexture(const char* path, unsigned int programmID);
    void setTexture(const char* path, unsigned int programmID, int array_index);
    void setTextures(const char* path, unsigned int programmID, int array_index);
    glm::mat4 getObjekt();

};
#endif
