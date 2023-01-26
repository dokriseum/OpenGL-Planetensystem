#ifndef _APPLIKATION_H
#define _APPLIKATION_H

#include "GLFW/glfw3.h"
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
    glm::mat4 modell;
    double rotationPositiv;
    double rotationNegtiv;
    double distanzKamera;
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
    Universumskoerper renderHelper(Universumskoerper uk);
    //void key_callback(int key, int scancode, int action, int mods);
    Universumskoerper movePlanet(Universumskoerper uk,double speed);
};

#endif