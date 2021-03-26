/*	Bart�omiej Kunikowski - EiTI PW - Warszawa 2015/2016
*	Projekt mini tr�by powietrznej z li�ci na zaj�cia z Grafiki Komputerowej
*/

#include <Windows.h>
#include <glm\glm.hpp>

#include "Scene.h"
#include "Textures.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <chrono>
#include <thread>

#define MAX_FPS 60							// maksymalna liczba klatek na sekund�

using namespace std;

// deklaracja zmiennych globalnych
Scene scene;								// obiekt sceny, w kt�rej definujemy obiekty do renderingu
GLfloat angle = 0;							// k�t obrotu kamery
GLuint lightIntensity = 100;				// intensywno�� �wiate�

// rozmiary okna
int windowWidth, windowHeight;

// czy obr�t kamery jest zatrzymany (klawisz p)
bool stop = false;

// Pozycje kamery i celu kamery
glm::vec3 cameraPosition(0.0f, 50.0f, -100.0f);
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);

/*	Funkcja odpowiedzialna za rysowanie sceny.
	Jej dzia�anie sprowadza si� g��wnie do wywo�ania funkcji rysowania danej sceny
*/
void Draw()
{
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.DrawScene();

	glFlush();

	glutSwapBuffers();
}

/*	Funkcja odpowiedzialna za od�wie�anie wygl�du sceny.
	Jej dzia�anie polega na zmianie po�o�enia kamery, 
	intensywno�ci �wiate�, pilnowaniu odpowiedniej ilo�ci wy�wietlanych klatek na sekund�
	i wywo�ywaniu funkcji rysuj�cej.
*/
void Idle()
{
	if (stop == false)						// je�li nie wcisn�li�my klawisza aby obr�t sceny si� zatrzyma�
	{										// kamera zmienia swoj� pozycj�
		glMatrixMode(GL_PROJECTION);
		glRotatef(0.3f, 0.f, 1.f, 0.f);
		glMatrixMode(GL_MODELVIEW);
	}

	// zmiana intensywno�ci �wiate�
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::vec4(0.8 * lightIntensity/100, 0.8 * lightIntensity / 100, 0.8 * lightIntensity / 100, 1.0).data);
	glLightfv(GL_LIGHT1, GL_AMBIENT, glm::vec4(2.0 * lightIntensity / 100 + 1.0, 2.0 * lightIntensity / 100 + 1.0, 2.0 * lightIntensity / 100 + 1.0, 1.0).data);
	glPopMatrix();

	//pomiar czasu, rysowanie sceny i ograniczanie ilo�ci wy�wietlanych klatek
	float sTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
	Draw();
	
	float eTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
	float t = eTime - sTime;

	if (t < 1000 / MAX_FPS)
		std::this_thread::sleep_for(std::chrono::milliseconds((long)((1000/MAX_FPS) - t)));
}

/*	Funkcja obs�uguj�ca zmian� rozmiaru okna.
*/
void Resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/*	Funkcja obs�uguj�ca wci�ni�cie klawisza.
*/
void Key(unsigned char key, int x, int y)
{
	if (key == 'q') exit(0);										// po wci�ni�ciu klawisza q, program zako�czy dzia�anie

	if (key == 'p') stop = !stop;									// po wci�ni�ciu klawisza p, kamera przestanie si� porusza�

	if (key == '+' && lightIntensity != 100) lightIntensity++;		// po wci�ni�ciu klawisza +, zwi�kszy si� intensywno�� �wiat�a, mo�na zwi�ksza� do pewnego maksymalnego poziomu

	if (key == '-' && lightIntensity != 0) lightIntensity--;		// po wci�ni�ciu klawisza -, zmniejszy si� intensywno�� �wiat�a, mo�na zmniejsza� do pewnego minimalnego poziomu
}

/*	Funkcja inicjalizuj�ca dzia�anie programu, a tak�e inicjalizuj�ca pewne elementy sceny.
*/
void Init()
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);								// ca�e okno pokryj wybranym kolorem
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	
	// wyb�r perspektywy widzenia i po�o�enia kamery oraz jej celu
	gluPerspective(45.0, 1.0, 10.0, 400.0);
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraTarget[0], cameraTarget[1], cameraTarget[2], 0.0, 1.0, 0.0);

	// ustawianie odpowiednich flag programu
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_MODELVIEW);

	// �adowanie tekstur
	Textures::LoadTexture("klon.png", "leaf");
	Textures::LoadTexture("grass2.png", "grass");
	Textures::LoadTexture("panorama.png", "panorama");
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);										// inicjalizacja glut
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// ustwianie trybu wy�wietlania
	glutInitWindowSize(1280, 720);								// definiowanie rozmiaru okna
	glutInitWindowPosition(200, 200);							// definiowanie po�o�enia okna

	glutCreateWindow("Mini traba powietrzna - [q]uit");

	Init();

	// dodawanie funkcji obs�uguj�cych poszczeg�lne zdarzenia
	glutDisplayFunc(Draw);
	glutReshapeFunc(Resize);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Key);

	glutMainLoop();

	return 0;
}


