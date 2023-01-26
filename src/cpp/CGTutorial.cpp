// Include Standardheader, steht bei jedem C/C++-Programm am Anfang
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW, GLEW ist ein notwendiges Übel. Der Hintergrund ist, dass OpenGL von Microsoft
// zwar unterstützt wird, aber nur in einer Uralt-Version. Deshalb beinhaltet die Header-Datei,
// die vom Betriebssystem zur Verfügung gestellt wird, nur Deklarationen zu den uralten Funktionen,
// obwohl der OpenGL-Treiber, und damit die OpenGL-dll die neuesten Funktionen implementiert.
// Die neueren Funktionen werden deshalb über diese Header-Datei separat zur Verfügung gestellt.
#include <GL/glew.h>

// Include GLFW, OpenGL definiert betriebssystemunabhängig die grafische Ausgabe. Interaktive
// Programme betätigen aber natürlich auch Funktionen für die Eingabe (z. B. Tastatureingaben).
// Dies geht bei jedem OS (z. B. Windows vs. MacOS/Unix) etwas anders. Um nun generell plattformunabhängig
// zu sein, verwenden wir GLFW, was die gleichen Eingabe-Funktionen auf die Implementierung unterschiedlicher
// OS abbildet. (Dazu gibt es Alternativen, glut wird z. B. auch häufig verwendet.)
#include <GLFW/glfw3.h>

// Include GLM, GLM definiert für OpenGL-Anwendungen Funktionen der linearen Algebra wie
// Transformationsmatrizen. Mann könnte GLM auch durch etwas anderes ersetzen oder aber in einem
// anderen Kontext verwenden.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Ab Uebung5 werden objloader.hpp und cpp benötigt
#include "objloader.hpp"
// Ab Uebung7 werden texture.hpp und cpp benötigt
#include "texture.hpp"

using namespace glm;

// In C- und C++-Programmen ist die Reihenfolge der include-Direktiven wichtig.
// Dateien, die mit Spitzklammern includiert werden, werden in den System-Verzeichnissen
// gesucht, die mit doppelten Hochkommata im lokalen Projektverzeichnis
// (wo genau ist in den Projekteinstellungen zu finden und ggf. zu ändern.) 

// Diese Datei benätigen wir, um die Shader-Programme komfortabel in die Hardware zu laden.
// (Mit der rechten Mouse-taste kännen Sie in VS diese Datei öffnen, um nachzuschauen, was dort deklariert wird.)
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// kuemmert sich um den Pfad zu den Shadern
#include "asset.hpp"

float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
float rotate_arm_buttom = 0.0f;
float rotate_arm_middle = 0.0f;
float rotate_arm_top = 0.0f;
float rotate_arm_horizon = 0.0f;

// Callback-Mechanismen gibt es in unterschiedlicher Form in allen mäglichen Programmiersprachen,
// sehr häufig in interaktiven graphischen Anwendungen. In der Programmiersprache C werden dazu 
// Funktionspointer verwendet. Man äbergibt einer aufgerufenen Funktion einer Bibliothek einen
// Zeiger auf eine Funktion, die zuräckgerufen werden kann. Die Signatur der Funktion muss dabei
// passen. Dieser Mechanismus existiert auch in C++ und wird hier verwendet, um eine einfache
// Fehlerbehandlung durchzufähren. Diese Funktion gibt Fehler aus, die beim Aufruf von OpenGL-Befehlen
// auftreten.
void error_callback(int error, const char* description)
{
	// Mit fputs gibt man hier den String auf den Standarderror-Kanal aus.
	// In der C-Welt, aus der das hier äbernommen ist, sind Strings Felder aus "char"s, die mit 
	// dem Wert null terminiert werden.
	fputs(description, stderr);
}

// Diese Funktion wird ebenfalls äber Funktionspointer der GLFW-Bibliothek äbergeben.
// (Die Signatur ist hier besonders wichtig. Wir sehen, dass hier drei Parameter definiert
//  werden mässen, die gar nicht verwendet werden.)
// Generell äberlassen wir der GLFW-Bibliothek die Behandlung der Input-Ereignisse (Mouse moved,
// button click, Key pressed, etc.).
// Durch die äbergabe dieser Funktion kännen wir Keyboard-Events 
// abfangen. Mouse-Events z. B. erhalten wir nicht, da wir keinen Callback an GLFW äbergeben.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	// Mit rechter Maustaste -> gehe zu Deklaration finden Sie andere Konstanten für Tasten.
	case GLFW_KEY_ESCAPE:
		// Das Programm wird beendet, wenn BenutzerInnen die Escapetaste betätigen.
		// Wir könnten hier direkt die C-Funktion "exit" aufrufen, eleganter ist aber, GLFW mitzuteilen,
		// dass wir das Fenster schliessen wollen (siehe Schleife unten).
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;

        case GLFW_KEY_W:
            angleX += 0.05f;
            break;
        case GLFW_KEY_S:
            angleX -= 0.05f;
            break;

        case GLFW_KEY_Q:
            angleZ += 0.05f;
            break;
        case GLFW_KEY_E:
            angleZ -= 0.05f;
            break;

        case GLFW_KEY_A:
            angleY += 0.05f;
            break;
        case GLFW_KEY_D:
            angleY -= 0.05f;
            break;

        case GLFW_KEY_0:
            angleX = 0.0f;
            angleY = 0.0f;
            angleZ = 0.0f;
            rotate_arm_buttom = 0.0f;
            rotate_arm_middle = 0.0f;
            rotate_arm_top = 0.0f;
            rotate_arm_horizon = 0.0f;
            break;

        case GLFW_KEY_KP_1:
            rotate_arm_buttom -= 0.05f;
            break;
        case GLFW_KEY_KP_2:
            rotate_arm_horizon += 0.05f;
            break;
        case GLFW_KEY_KP_3:
            rotate_arm_buttom += 0.05f;
            break;
        case GLFW_KEY_KP_4:
            rotate_arm_middle -= 0.05f;
            break;
        case GLFW_KEY_KP_6:
            rotate_arm_middle += 0.05f;
            break;
        case GLFW_KEY_KP_7:
            rotate_arm_top -= 0.05f;
            break;
        case GLFW_KEY_KP_9:
            rotate_arm_top += 0.05f;
            break;
        case GLFW_KEY_KP_8:
            rotate_arm_horizon -= 0.05f;
            break;

	default:
		break;
	}
}


// Diese drei Matrizen speichern wir global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden können. Ihre Bedeutung habe ich in der Vorlesung Geometrische
// Transformationen erklärt, falls noch nicht geschehen, jetzt anschauen!
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 Save;
GLuint programID; // OpenGL unterstützt unterschiedliche Shaderprogramme, zwischen denen man
                  // wechseln kann. Unser Programm wird mit der unsigned-integer-Variable programID
                  // referenziert.

// Ich habe Ihnen hier eine Hilfsfunktion definiert, die wir verwenden, um die Transformationsmatrizen
// zwischen dem OpenGL-Programm auf der CPU und den Shaderprogrammen in den GPUs zu synchronisieren.
// (Muss immer aufgerufen werden, bevor wir Geometriedaten in die Pipeline einspeisen.)
void sendMVP()
{
	// Zunächst können wir die drei Matrizen einfach kombinieren, da unser einfachster Shader
	// wirklich nur eine Transformationsmatrix benötigt, wie in der Vorlesung erklärt.
	// Später werden wir hier auch die Teilmatrizen an den Shader übermitteln mässen.
	// Interessant ist hier, dass man in C++ (wie auch in C#) den "*"-Operator überladen kann, sodass
	// man Klassenobjekte miteinander multiplizieren kann (hier Matrizen bzw. "mat4"), 
	// das ginge in JAVA so natürlich nicht.
	glm::mat4 MVP = Projection * View * Model;

	// "glGetUniformLocation" liefert uns eine Referenz auf eine Variable, die im Shaderprogramm
	// definiert ist, in diesem Fall heisst die Variable "MVP".
	// "glUniformMatrix4fv" überträgt Daten, genauer 4x4-Matrizen, aus dem Adressraum unserer CPU
	// (vierter Parameter beim Funktionsaufruf, wir generieren mit "&" hier einen Pointer auf das erste 
	//  Element, und damit auf das gesamte Feld bzw den Speicherbereich) 
	// in den Adressraum der GPUs. Beim ersten Parameter 
	// muss eine Referenz auf eine Variable im Adressraum der GPU angegeben werden.
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Aufgabe 6
    glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void zeichneKS(){
    Save = Model;
    Model = glm::scale(Model, glm::vec3(2,0.01,0.01));
    sendMVP();
    drawWireCube();
    Model = Save;
    Model = glm::scale(Model, glm::vec3(0.01,2,0.01));
    sendMVP();
    drawWireCube();
    Model = Save;
    Model = glm::scale(Model, glm::vec3(0.01,0.01,2));
    sendMVP();
    drawWireCube();
    Model = Save;
}
void zeichneSegment(float h)
{
    Save = Model;
    //Model = glm::translate(Model, glm::vec3(0, 1 * h, 0));
    //Model = glm::scale(Model, glm::vec3(h / 2.5, h / 1, h / 2.5));
    Model = glm::translate(Model, glm::vec3(0, h / 2, 0));
    Model = glm::scale(Model, glm::vec3(h / 4, h / 2, h / 4));
    sendMVP();
    drawSphere(128, 128);
    Model = Save;
}
// Einstiegspunkt für C- und C++-Programme (Funktion), Konsolenprogramme könnte hier auch Parameter erwarten
int mains(void)
{
	// Initialisierung der GLFW-Bibliothek
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);

	// öffnen eines Fensters für OpenGL, die letzten beiden Parameter sind hier unwichtig.
	// Diese Funktion darf erst aufgerufen werden, nachdem GLFW initialisiert wurde.
	// (Ggf. glfwWindowHint vorher aufrufen, um erforderliche Ressourcen festzulegen -> MacOSX)
	GLFWwindow* window = glfwCreateWindow(1024, // Breite
										  768,  // Höhe
										  "CG - Tutorial", // Überschrift
										  NULL,  // windowed mode
										  NULL); // shared window

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Wir könnten uns mit glfwCreateWindow auch mehrere Fenster aufmachen...
	// Spätestens dann wäre klar, dass wir den OpenGL-Befehlen mitteilen mässen, in
	// welches Fenster sie "malen" sollen. Wir mässen das aber zwingend auch machen,
	// wenn es nur ein Fenster gibt.

	// Bis auf Weiteres sollen OpenGL-Befehle in "window" malen.
	// Ein "Graphic Context" (GC) speichert alle Informationen zur Darstellung, z. B.
	// die Linienfarbe, die Hintergrundfarbe. Dieses Konzept hat den Vorteil, dass
	// die Malbefehle selbst weniger Parameter benötigen.
	// Erst danach darf man dann OpenGL-Befehle aufrufen!
    glfwMakeContextCurrent(window);

	// Initialisiere GLEW
	// (GLEW ermöglicht Zugriff auf OpenGL-API > 1.1)
	glewExperimental = true; // Diese Zeile ist leider notwendig.

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Auf Keyboard-Events reagieren (s. o.)
	glfwSetKeyCallback(window, key_callback);

	// Setzen von Dunkelblau als Hintergrundfarbe (erster OpenGL-Befehl in diesem Programm).
	// Beim späteren Löschen gibt man die Farbe dann nicht mehr an, sondern liest sie aus dem GC.
	// Der Wertebereich in OpenGL geht nicht von 0 bis 255, sondern von 0 bis 1, hier sind Werte
	// für R, G und B angegeben, der vierte Wert alpha bzw. Transparenz ist beliebig, da wir keine
	// Transparenz verwenden. Zu den Farben sei auf die entsprechende Vorlesung verwiesen!
	glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	// Kreieren von Shadern aus den angegebenen Dateien, kompilieren und linken und in
	// die Grafikkarte übertragen.
	//programID = LoadShaders(SHADER_DIR "/TransformVertexShader.vertexshader", SHADER_DIR "/ColorFragmentShader.fragmentshader");
    programID = LoadShaders(SHADER_DIR "/StandardShading.vertexshader", SHADER_DIR "/StandardShading.fragmentshader");

	// Diesen Shader aktivieren! (Man kann zwischen Shadern wechseln.)
	glUseProgram(programID);



    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool res = loadOBJ("../src/resources/teapot.obj", vertices, uvs, normals);
    // A5.3
    // Jedes Objekt eigenem VAO zuordnen, damit mehrere Objekte möglich sind
    // VAOs sind Container für mehrere Buffer, die zusammen gesetzt werden sollen.
    GLuint VertexArrayIDTeapot;
    glGenVertexArrays(1, &VertexArrayIDTeapot);
    glBindVertexArray(VertexArrayIDTeapot);
    // A5.3
    // Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer); // Kennung erhalten
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
    // Buffer zugreifbar für die Shader machen
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // A5.3
    // Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
    glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0
    glVertexAttribPointer(0,  // location = 0
                          3,  // Datenformat vec3: 3 floats für xyz
                          GL_FLOAT,
                          GL_FALSE, // Fixedpoint data normalisieren?
                          0, // Eckpunkte direkt hintereinander gespeichert
                          (void*) 0); // abweichender Datenanfang?

    GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); // siehe layout im vertex shader
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


    GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // siehe layout im vertex shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // Load the texture
    GLuint Texture = loadBMP_custom("../src/resources/mandrill.bmp");


	// Alles ist vorbereitet, jetzt kann die Eventloop laufen...
	while (!glfwWindowShouldClose(window))
	{
		// Läschen des Bildschirms (COLOR_BUFFER), man kann auch andere Speicher zusätzlich löschen,
		// kommt in späteren Übungen noch...
		// Per Konvention sollte man jedes Bild mit dem Löschen des Bildschirms beginnen, muss man aber nicht...
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Einstellen der Geometrischen Transformationen:
		// Wir verwenden dazu die Funktionen aus glm.h
		// Projektionsmatrix mit 45Grad horizontalem Öffnungswinkel, 4:3 Seitenverhältnis,
		// Frontplane bai 0.1 und Backplane bei 100. (Das sind OpenGL-Einheiten, keine Meter oder der gleichen.)
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
		// Viewmatrix, beschreibt, wo die Kamera steht, wo sie hinschaut, und wo oben ist.
		// Man muss angeben, wo oben ist, da es eine Mehrdeutigkeit gäbe, wenn man nur beschreiben
		// würde, wo die Kamera steht und wo sie hinschaut. Denken Sie an ein Flugzeug. Die Position 
		// des/r Piloten/in in der Welt ist klar, es ist dann auch klar, wo er/sie hinschaut. Das Flugzeug 
		// kann sich aber z. B. auf die Seite legen, dann würde der Horizont "kippen". Dieser Aspekt wird
		// mit dem up-Vektor (hier "oben") gesteuert.
		View = glm::lookAt(glm::vec3(0,0,-5), // die Kamera ist bei (0,0,-5), in Weltkoordinaten
						   glm::vec3(0,0,0),  // und schaut in den Ursprung
						   glm::vec3(0,1,0)); // Oben ist bei (0,1,0), das ist die y-Achse
	
		
		// Modelmatrix: Hier auf Einheitsmatrix gesetzt, was bedeutet, dass die Objekte sich im Ursprung
		// des Weltkoordinatensystems befinden.
		Model = glm::mat4(1.0f);
        //Model = glm::rotate(Model, 0.0f, vec3(0, 1, 0));
        //Model = glm::scale(Model, glm::vec3(1.0 / 500.0, 1.0 / 500.0, 1.0 / 500.0));


        Model = glm::rotate(Model, angleX, vec3(1, 0, 0));
        Model = glm::rotate(Model, angleY, vec3(0, 1, 0));
        Model = glm::rotate(Model, angleZ, vec3(0, 0, 1));


        Save = Model;
        Model = glm::translate(Model, glm::vec3(1.5, 0.0, 0.0));
        Model = glm::scale(Model, glm::vec3(1.0 / 1000.0, 1.0 / 1000.0, 1.0 / 1000.0));
        Model = glm::rotate(Model, -90.0f, vec3(1, 0, 0));

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);

        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

        // Diese Informationen (Projection, View, Model) müssen geeignet der Grafikkarte übermittelt werden,
		// damit sie beim Zeichnen von Objekten berücksichtigt werden können.
		sendMVP();


        // A5.4
        glBindVertexArray(VertexArrayIDTeapot);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		// Nachdem der GC in der Grafikkarte aktuell ist, also z. B. auch ein sendMVP ausgeführt wurde,
		// zeichen wir hier nun einen Würfel. Dazu werden in "drawWireCube" die Eckpunkte zur Grafikkarte
		// geschickt. Der gewählte Modus legt fest, wie die Punkte mit Linien verbunden werden.
		// Das werden wir uns später noch genauer anschauen. (Schauen Sie sich die schwarzen Linien genau an,
		// und überlegen Sie sich, dass das wirklich ein Würfel ist, der perspektivisch verzerrt ist.).
		// Die Darstellung nennt man übrigens "im Drahtmodell".
		//drawCube();





        // -----------
        Model = Save;
        Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
        sendMVP();
        //drawSphere(64,64);
        Model = glm::rotate(Model, rotate_arm_buttom, vec3(0, 0, 1));
        Model = glm::rotate(Model, rotate_arm_horizon, vec3(1, 0, 0));

        Model = glm::scale(Model, glm::vec3(1, 1, 1));
        zeichneSegment(1.0f);
        Model = glm::translate(Model, glm::vec3(0.0f, 1.0f, 0.0f));
        Model = glm::rotate(Model, rotate_arm_middle, vec3(0, 0, 1));
        zeichneSegment(0.7f);
        Model = glm::translate(Model, glm::vec3(0.0f, 0.7f, 0.0f));
        Model = glm::rotate(Model, rotate_arm_top, vec3(0, 0, 1));
        zeichneSegment(0.5f);
        zeichneKS();

        glm::vec4 lightPos = Model * glm::vec4(0, 0, 0, 1);
        glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

        //Model = glm::scale(Model, glm::vec3(2, 0.01f, 0.01f));
        //Model = glm::scale(Model, glm::vec3(0.01f, 2, 0.01f));
        //Model = glm::scale(Model, glm::vec3(0.01f, 0.01f, 2));
        // -----------

		// Bildende. 
		// Bilder werden in den Bildspeicher gezeichnet (so schnell wie es geht.). 
		// Der Bildspeicher wird mit der eingestellten Bildwiederholfrequenz (also z. B. 60Hz)
		// ausgelesen und auf dem Bildschirm dargestellt. Da beide Frequenzen nicht übereinstimmen, würde
		// man beim Auslesen auf unfertige Bilder stoßen. Das wäre als Flimmern auf dem Bildschirm zu
		// erkennen. (War bei älteren Grafikkarten tatsächlich so.).
		// Dieses Problem vermeidet man, wenn man zwei Bildspeicher benutzt, wobei in einen gerade
		// gemalt wird, bzw. dort ein neues Bild entsteht, und der andere auf dem Bildschirm ausgegeben wird.
		// Ist man mit dem Erstellen eines Bildes fertig, tauscht man diese beiden Speicher einfach aus ("swap").
		glfwSwapBuffers(window);

		// Hier fordern wir glfw auf, Ereignisse zu behandeln. GLFW könnte hier z. B. feststellen,
		// das die Mouse bewegt wurde und eine Taste betätigt wurde.
		// Da wir zurzeit nur einen "key_callback" installiert haben, wird dann nur genau diese Funktion
		// aus "glfwPollEvents" heraus aufgerufen.
        glfwPollEvents();
	}

    // A5.5
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteTextures(1, &Texture);

	// Wenn der Benutzer, das Schliesskreuz oder die Escape-Taste betätigt hat, endet die Schleife und
	// wir kommen an diese Stelle. Hier können wir aufräumen, und z. B. das Shaderprogramm in der
	// Grafikkarte löschen. (Das macht zur not das OS aber auch automatisch.)
	glDeleteProgram(programID);

	// Schießen des OpenGL-Fensters und beenden von GLFW.
	glfwTerminate();

	return 0; // Integer zurückgeben, weil main so definiert ist
}

