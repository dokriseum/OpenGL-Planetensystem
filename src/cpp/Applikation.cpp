#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
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
#include "Universumskoerper.h"
#include "asset.hpp"
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

Applikation::Applikation(unsigned int breite, unsigned int hoehe, const char* titel) {
	this->breite = breite;
	this->hoehe = hoehe;
	this->titel = titel;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}

	this->hwnd = glfwCreateWindow(breite, hoehe, titel, NULL, NULL);
    distanzKamera = 40.0;
}

Applikation::~Applikation(){
	glfwDestroyWindow(this->hwnd);
}

/**
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(this->hwnd, GL_TRUE);
            break;

        case GLFW_KEY_J:
            distanzKamera++;
            break;
        case GLFW_KEY_I:
            if (distanzKamera > 0) {
                distanzKamera--;
            }
            break;

        case GLFW_KEY_W:
            break;
        case GLFW_KEY_S:
            break;

        case GLFW_KEY_Q:
            break;
        case GLFW_KEY_E:
            break;

        case GLFW_KEY_A:
            break;
        case GLFW_KEY_D:
            break;

        case GLFW_KEY_C:
            break;

        case GLFW_KEY_0:
            break;

        case GLFW_KEY_KP_1:

            break;
        case GLFW_KEY_KP_2:
            break;
        case GLFW_KEY_KP_3:
            break;
        case GLFW_KEY_KP_4:
            break;
        case GLFW_KEY_KP_6:
            break;
        case GLFW_KEY_KP_7:
            break;
        case GLFW_KEY_KP_8:
            break;
        case GLFW_KEY_KP_9:
            break;

        default:
            break;
    }
}
**/

void Applikation::sendMVP()
{
    // Zun‰chst kˆnnen wir die drei Matrizen einfach kombinieren, da unser einfachster Shader
    // wirklich nur eine Transformationsmatrix benˆtigt, wie in der Vorlesung erkl‰rt.
    // Sp‰ter werden wir hier auch die Teilmatrizen an den Shader ¸bermitteln m‰ssen.
    // Interessant ist hier, dass man in C++ (wie auch in C#) den "*"-Operator ¸berladen kann, sodass
    // man Klassenobjekte miteinander multiplizieren kann (hier Matrizen bzw. "mat4"),
    // das ginge in JAVA so nat¸rlich nicht.
    glm::mat4 MVP = projektion * ansicht * modell;

    // "glGetUniformLocation" liefert uns eine Referenz auf eine Variable, die im Shaderprogramm
    // definiert ist, in diesem Fall heisst die Variable "MVP".
    // "glUniformMatrix4fv" ¸bertr‰gt Daten, genauer 4x4-Matrizen, aus dem Adressraum unserer CPU
    // (vierter Parameter beim Funktionsaufruf, wir generieren mit "&" hier einen Pointer auf das erste
    //  Element, und damit auf das gesamte Feld bzw den Speicherbereich)
    // in den Adressraum der GPUs. Beim ersten Parameter
    // muss eine Referenz auf eine Variable im Adressraum der GPU angegeben werden.
    glUniformMatrix4fv(glGetUniformLocation(programmID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Aufgabe 6
    glUniformMatrix4fv(glGetUniformLocation(programmID, "M"), 1, GL_FALSE, &modell[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "V"), 1, GL_FALSE, &ansicht[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "P"), 1, GL_FALSE, &projektion[0][0]);
}

void drawVertices(Universumskoerper uk) {
    GLuint VertexArrayIDSphere;
    glGenVertexArrays(1, &VertexArrayIDSphere);
    glBindVertexArray(VertexArrayIDSphere);
    // A5.3
    // Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer); // Kennung erhalten
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
    // Buffer zugreifbar f¸r die Shader machen
    glBufferData(GL_ARRAY_BUFFER, uk.vertices.size() * sizeof(glm::vec3), &uk.vertices[0], GL_STATIC_DRAW);

    // A5.3
    // Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                            3,                            GL_FLOAT,                          GL_FALSE,                           0,                           (void*) 0);

}

//----------------------------//
//         Send MVP           //
//----------------------------//

void Applikation::sendMVP(glm::mat4 gameObjectModel) {
    glm::mat4 MVP = projektion * ansicht * gameObjectModel;

    glUniformMatrix4fv(glGetUniformLocation(programmID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "M"), 1, GL_FALSE, &gameObjectModel[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "V"), 1, GL_FALSE, &ansicht[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "P"), 1, GL_FALSE, &projektion[0][0]);
}

void Applikation::run() {
	if (!this->hwnd) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(hwnd);
	glewExperimental = true;

	if (glewInit()!=GLEW_OK) {
		fprintf(stderr, "GLEW wurde nicht ordnungsgem‰ﬂ initialisiert.");
		return;
	}

	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0f, 0.0f, 0.0f, 0.9f);

	glfwSetKeyCallback(hwnd, key_callback);
	programmID = LoadShaders(SHADER_DIR "/StandardShading.vertexshader", SHADER_DIR "/StandardShading.fragmentshader");
	glUseProgram(programmID);

    std::vector<Universumskoerper> ukVector;
    ukVector.reserve(10);

    Universumskoerper ukSonne = Universumskoerper("Sonne", 0, RESOURCES_DIR "/sonne.bmp", 0.0, 0.0, 2.0f, 2.0f, 2.0f);
    Universumskoerper ukMerkur = Universumskoerper("Merkur", 88, RESOURCES_DIR "/merkur.bmp", 0.58, 2.0, 0.2f, 0.2f, 0.2f);
    Universumskoerper ukVenus = Universumskoerper("Venus", 225, RESOURCES_DIR "/venus.bmp", 1.08, 4.0, 0.2f, 0.2f, 0.3f);
    Universumskoerper ukErde = Universumskoerper("Erde", 365, RESOURCES_DIR "/erde.bmp", 1.5, 6.0, 0.3f, 0.3f, 0.3f);
    Universumskoerper ukMars = Universumskoerper("Mars", 687, RESOURCES_DIR "/mars.bmp", 2.28, 8.0, 0.3f, 0.3f, 0.3f);
    Universumskoerper ukJupiter = Universumskoerper("Jupiter", 12*365, RESOURCES_DIR "/jupiter.bmp", 7.78, 10.0, 0.5f, 0.5f, 0.5f);
    Universumskoerper ukSaturn = Universumskoerper("Saturn", 29*365, RESOURCES_DIR "/saturn.bmp", 14.33, 12.0, 0.7f, 0.7f, 0.7f);
    Universumskoerper ukUranus = Universumskoerper("Uranus", 84*365, RESOURCES_DIR "/uranus.bmp", 28.72, 14.0, 0.45f, 0.45f, 0.45f);
    Universumskoerper ukNeptun = Universumskoerper("Neptun", 165*365, RESOURCES_DIR "/neptun.bmp", 44.98, 16.0, 0.45f, 0.45f, 0.45f);
    Universumskoerper ukPluto = Universumskoerper("Pluto", 248*365, RESOURCES_DIR "/pluto.bmp", 59.06, 18.0, 0.1f, 0.1f, 0.1f);

    ukVector.push_back(renderHelper(ukSonne));
    ukVector.push_back(renderHelper(ukMerkur));
    ukVector.push_back(renderHelper(ukVenus));
    ukVector.push_back(renderHelper(ukErde));
    ukVector.push_back(renderHelper(ukMars));
    ukVector.push_back(renderHelper(ukJupiter));
    ukVector.push_back(renderHelper(ukSaturn));
    ukVector.push_back(renderHelper(ukUranus));
    ukVector.push_back(renderHelper(ukNeptun));
    ukVector.push_back(renderHelper(ukPluto));
    ukSonne.setLeuchtkraft(programmID);
    float n = 0.1f;
    float f = 100.0f;
    this->projektion = glm::perspective(45.0f, 16.0f / 9.0f, n, f);

    //this->ansicht = glm::lookAt(glm::vec3(0.0, 0.0f, -distanzKamera), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    //----------------------------//
    //            WHILE           //
    //----------------------------//

    while (!glfwWindowShouldClose(hwnd)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //projektion = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 10.0f);
        //ansicht = glm::lookAt(glm::vec3(0,0,-5),glm::vec3(0,0,0),glm::vec3(0,1,0));
        modell = glm::mat4(1.0f);

        glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);
        for (int i = 0; i < ukVector.size(); i++) {
            Universumskoerper uk = ukVector[i];

            if(uk.name!="Sonne"){
                uk = movePlanet(uk,250);
            }
            else{
                printf("Sonne bei x: %f y: %f\n",uk.posX,uk.posZ);

            }
            glm::mat4 tmp = uk.gameObjectModel;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, uk.texture);
            glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);
            sendMVP(uk.gameObjectModel);
            glBindVertexArray(uk.renderVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, uk.vertices.size());
            uk.gameObjectModel = tmp;
        }

        //ukSonne.setLeuchtkraft(programmID);
        glm::vec3 lightPos = ukSonne.getObjekt() * glm::vec4(0.0f, 0.3f, 0.0f, 10.0f);//glm::vec3(4,4,?4);
        glUniform3f(glGetUniformLocation(programmID, "LightPosition_worldspace"), lightPos.x, lightPos.y,
                    lightPos.z);

        glfwSwapBuffers(hwnd);

    }

}

Universumskoerper Applikation::movePlanet(Universumskoerper uk,double speed){
    double a = uk.maxDistSun;
    double r = uk.minDistSun;
    double phiSkip = 360/uk.yearLength;
    phiSkip = phiSkip/speed;
    uk.phi += phiSkip;
    double x,y;
    x=r*cos(uk.phi);
    y=r*sin(uk.phi);
    glm::mat4 tmp = glm::translate(uk.gameObjectModel, glm::vec3(uk.posX-x, 0, uk.posZ-y));
    uk.posX=x;
    uk.posZ=y;
    if(uk.name=="Erde"){
         printf("Erde bei x: %f y: %f\n",uk.posX,uk.posZ);
    }

    uk.gameObjectModel=tmp;
    return uk;





}
//----------------------------//
//       render helper        //
//----------------------------//

//2
//helper method for setting ub the vertex-, uv- and normalbuffer
Universumskoerper Applikation::renderHelper(Universumskoerper uk) {
    std::vector<glm::vec3> vertices = uk.vertices;
    std::vector<glm::vec2> uvs = uk.uvs;
    std::vector<glm::vec3> normals = uk.normals;

    GLuint VertexArrayIDUniversumskoerper;
    glGenVertexArrays(1, &VertexArrayIDUniversumskoerper);
    glBindVertexArray(VertexArrayIDUniversumskoerper);

    // Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer); // Kennung erhalten
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
    // Buffer zugreifbar f?r die Shader machen
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
    glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0
    glVertexAttribPointer(0,  // location = 0
                          3,  // Datenformat vec3: 3 floats fuer xyz
                          GL_FLOAT,
                          GL_FALSE, // Fixedpoint data normalisieren ?
                          0, // Eckpunkte direkt hintereinander gespeichert
                          (void*)0); // abweichender Datenanfang ?

    GLuint normalbuffer; // Hier alles analog f?r Normalen in location == 2
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); // siehe layout im vertex shader
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint uvbuffer; // Hier alles analog f?r Texturkoordinaten in location == 1 (2 floats u und v!)
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // siehe layout im vertex shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    Universumskoerper uk2 = uk;
    uk2.renderVertexArray = VertexArrayIDUniversumskoerper;
    uk2.vertices = vertices;
    uk2.gameObjectModel = uk.gameObjectModel;
    return uk2;
}
