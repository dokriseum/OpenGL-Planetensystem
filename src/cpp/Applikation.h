#ifndef _APPLIKATION_H
#define _APPLIKATION_H

#include "GLFW/glfw3.h"
#include "RenderInformation.h"
#include "Universumskoerper.h"
#include "asset.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class Applikation {

public: 
	Applikation(unsigned int breite = 500, unsigned int hoehe = 500, const char* titel = "");
	~Applikation();

	void run();
	void sendMVP();

private:
	GLFWwindow* hwnd;
	int breite, hoehe;
	const char* titel;
	unsigned int programmID;
	glm::mat4 projektion;
    glm::mat4 ansicht;
    glm::mat4 modell;
    char* pathsBMP[10] = { RESOURCES_DIR "/sonne.bmp",
    RESOURCES_DIR "/merkur.bmp",
    RESOURCES_DIR "/venus.bmp",
    RESOURCES_DIR "/erde.bmp",
    RESOURCES_DIR "/mars.bmp",
    RESOURCES_DIR "/jupiter.bmp",
    RESOURCES_DIR "/saturn.bmp",
    RESOURCES_DIR "/uranus.bmp",
    RESOURCES_DIR "/neptun.bmp",
    RESOURCES_DIR "/pluto.bmp"
    };
    void sendMVP(mat4 gameObjectModel);
    void setTexture(glm::mat4 uk, const char *path, unsigned int programmID);
    void setTexture(glm::mat4 uk, const char *path, unsigned int programmID, RenderInformation ri);
    RenderInformation renderHelper(Universumskoerper uk);

};

#endif