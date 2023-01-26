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
#include "Applikation.h"

int main(void) {
	Applikation app = Applikation(1280, 720, "Milchstrasse");
	app.run();

	//TODO: Objekt laden
	//TODO: Universumskoerper (UK) in Sonnensystem initialisieren
	//TODO: UK kriegt Position
	//TODO: auf GraKa anzeigen in die run()
	//TODO: 
	//TODO: 
}