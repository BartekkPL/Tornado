/*	Bart³omiej Kunikowski - EiTI PW - Warszawa 2015/2016
*	Projekt mini tr¹by powietrznej z liœci na zajêcia z Grafiki Komputerowej
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

#define MAX_FPS 60							// maksymalna liczba klatek na sekundê

using namespace std;

// deklaracja zmiennych globalnych
Scene scene;								// obiekt sceny, w której definujemy obiekty do renderingu
GLfloat angle = 0;							// k¹t obrotu kamery
GLuint lightIntensity = 100;				// intensywnoœæ œwiate³

// rozmiary okna
int windowWidth, windowHeight;

// czy obrót kamery jest zatrzymany (klawisz p)
bool stop = false;

// Pozycje kamery i celu kamery
glm::vec3 cameraPosition(0.0f, 50.0f, -100.0f);
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);

/*	Funkcja odpowiedzialna za rysowanie sceny.
	Jej dzia³anie sprowadza siê g³ównie do wywo³ania funkcji rysowania danej sceny
*/
void Draw()
{
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.DrawScene();

	glFlush();

	glutSwapBuffers();
}

/*	Funkcja odpowiedzialna za odœwie¿anie wygl¹du sceny.
	Jej dzia³anie polega na zmianie po³o¿enia kamery, 
	intensywnoœci œwiate³, pilnowaniu odpowiedniej iloœci wyœwietlanych klatek na sekundê
	i wywo³ywaniu funkcji rysuj¹cej.
*/
void Idle()
{
	if (stop == false)						// jeœli nie wcisnêliœmy klawisza aby obrót sceny siê zatrzyma³
	{										// kamera zmienia swoj¹ pozycjê
		glMatrixMode(GL_PROJECTION);
		glRotatef(0.3f, 0.f, 1.f, 0.f);
		glMatrixMode(GL_MODELVIEW);
	}

	// zmiana intensywnoœci œwiate³
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::vec4(0.8 * lightIntensity/100, 0.8 * lightIntensity / 100, 0.8 * lightIntensity / 100, 1.0).data);
	glLightfv(GL_LIGHT1, GL_AMBIENT, glm::vec4(2.0 * lightIntensity / 100 + 1.0, 2.0 * lightIntensity / 100 + 1.0, 2.0 * lightIntensity / 100 + 1.0, 1.0).data);
	glPopMatrix();

	//pomiar czasu, rysowanie sceny i ograniczanie iloœci wyœwietlanych klatek
	float sTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
	Draw();
	
	float eTime = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	
	float t = eTime - sTime;

	if (t < 1000 / MAX_FPS)
		std::this_thread::sleep_for(std::chrono::milliseconds((long)((1000/MAX_FPS) - t)));
}

/*	Funkcja obs³uguj¹ca zmianê rozmiaru okna.
*/
void Resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/*	Funkcja obs³uguj¹ca wciœniêcie klawisza.
*/
void Key(unsigned char key, int x, int y)
{
	if (key == 'q') exit(0);										// po wciœniêciu klawisza q, program zakoñczy dzia³anie

	if (key == 'p') stop = !stop;									// po wciœniêciu klawisza p, kamera przestanie siê poruszaæ

	if (key == '+' && lightIntensity != 100) lightIntensity++;		// po wciœniêciu klawisza +, zwiêkszy siê intensywnoœæ œwiat³a, mo¿na zwiêkszaæ do pewnego maksymalnego poziomu

	if (key == '-' && lightIntensity != 0) lightIntensity--;		// po wciœniêciu klawisza -, zmniejszy siê intensywnoœæ œwiat³a, mo¿na zmniejszaæ do pewnego minimalnego poziomu
}

/*	Funkcja inicjalizuj¹ca dzia³anie programu, a tak¿e inicjalizuj¹ca pewne elementy sceny.
*/
void Init()
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);								// ca³e okno pokryj wybranym kolorem
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	
	// wybór perspektywy widzenia i po³o¿enia kamery oraz jej celu
	gluPerspective(45.0, 1.0, 10.0, 400.0);
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraTarget[0], cameraTarget[1], cameraTarget[2], 0.0, 1.0, 0.0);

	// ustawianie odpowiednich flag programu
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_MODELVIEW);

	// ³adowanie tekstur
	Textures::LoadTexture("klon.png", "leaf");
	Textures::LoadTexture("grass2.png", "grass");
	Textures::LoadTexture("panorama.png", "panorama");
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);										// inicjalizacja glut
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// ustwianie trybu wyœwietlania
	glutInitWindowSize(1280, 720);								// definiowanie rozmiaru okna
	glutInitWindowPosition(200, 200);							// definiowanie po³o¿enia okna

	glutCreateWindow("Mini traba powietrzna - [q]uit");

	Init();

	// dodawanie funkcji obs³uguj¹cych poszczególne zdarzenia
	glutDisplayFunc(Draw);
	glutReshapeFunc(Resize);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Key);

	glutMainLoop();

	return 0;
}


