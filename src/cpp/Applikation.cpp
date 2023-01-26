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

    //this->pathsBMP[10]
}

Applikation::~Applikation(){
	glfwDestroyWindow(this->hwnd);
}

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

	//glfwSetKeyCallback(hwnd, key_callback);
	programmID = LoadShaders(SHADER_DIR "/StandardShading.vertexshader", SHADER_DIR "/StandardShading.fragmentshader");
	glUseProgram(programmID);

    std::vector<Universumskoerper> ukVector;
    ukVector.reserve(10);

    Universumskoerper ukSonne = Universumskoerper("Sonne",RESOURCES_DIR "/sonne.bmp", -5.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f);
    Universumskoerper ukMerkur = Universumskoerper("Merkur", RESOURCES_DIR "/merkur.bmp", 1.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f);
    Universumskoerper ukVenus = Universumskoerper("Venus", RESOURCES_DIR "/venus.bmp", 2.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.3f);
    Universumskoerper ukErde = Universumskoerper("Erde", RESOURCES_DIR "/erde.bmp", 3.0f, 0.0f, 0.0f, 0.3f, 0.3f, 0.3f);
    Universumskoerper ukMars = Universumskoerper("Mars", RESOURCES_DIR "/mars.bmp", 4.0f, 0.0f, 0.0f, 0.3f, 0.3f, 0.3f);
    Universumskoerper ukJupiter = Universumskoerper("Jupiter", RESOURCES_DIR "/jupiter.bmp", 5.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f);
    Universumskoerper ukSaturn = Universumskoerper("Saturn", RESOURCES_DIR "/saturn.bmp", 6.0f, 0.0f, 0.0f, 0.7f, 0.7f, 0.7f);
    Universumskoerper ukUranus = Universumskoerper("Uranus", RESOURCES_DIR "/uranus.bmp", 7.0f, 0.0f, 0.0f, 0.45f, 0.45f, 0.45f);
    Universumskoerper ukNeptun = Universumskoerper("Neptun", RESOURCES_DIR "/neptun.bmp", 8.0f, 0.0f, 0.0f, 0.45f, 0.45f, 0.45f);
    Universumskoerper ukPluto = Universumskoerper("Pluto", RESOURCES_DIR "/pluto.bmp", 9.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.1f);

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
    float n = 0.1f;
    float f = 100.0f;
    this->projektion = glm::perspective(45.0f, 16.0f / 9.0f, n, f);
    this->ansicht = glm::lookAt(glm::vec3(0.0, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    //----------------------------//
    //            WHILE           //
    //----------------------------//

    while (!glfwWindowShouldClose(hwnd)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //projektion = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 10.0f);
        //ansicht = glm::lookAt(glm::vec3(0,0,-5),glm::vec3(0,0,0),glm::vec3(0,1,0));
        modell = glm::mat4(1.0f);
        setTexture(ukSonne.getObjekt(),pathsBMP[0],programmID,renderInformationVector[0]);
        setTexture(ukMerkur.getObjekt(),pathsBMP[1],programmID,renderInformationVector[1]);
        setTexture(ukVenus.getObjekt(),pathsBMP[2],programmID,renderInformationVector[2]);
        setTexture(ukErde.getObjekt(),pathsBMP[3],programmID,renderInformationVector[3]);
        setTexture(ukMars.getObjekt(),pathsBMP[4],programmID,renderInformationVector[4]);
        setTexture(ukJupiter.getObjekt(),pathsBMP[5],programmID,renderInformationVector[5]);
        setTexture(ukSaturn.getObjekt(),pathsBMP[6],programmID,renderInformationVector[6]);
        setTexture(ukUranus.getObjekt(),pathsBMP[7],programmID,renderInformationVector[7]);
        setTexture(ukNeptun.getObjekt(),pathsBMP[8],programmID,renderInformationVector[8]);
        setTexture(ukPluto.getObjekt(),pathsBMP[9],programmID,renderInformationVector[9]);

        glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);
        for (int i = 0; i < ukVector.size(); i++) {
            Universumskoerper uk = ukVector[i];
            glm::mat4 tmp = uk.gameObjectModel;
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, uk.texture);
            glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);
            sendMVP(uk.gameObjectModel);
            glBindVertexArray(uk.renderVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, uk.vertices.size());
            uk.gameObjectModel = tmp;
        }
        //glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);
        glm::vec4 lightPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(programmID, "LightPosition_worldspace"), lightPos.x, lightPos.y,
                    lightPos.z);
        glfwSwapBuffers(hwnd);

    }

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
