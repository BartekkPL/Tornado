#include "Scene.h"
#include "Textures.h"
#include <ctime>
#include <cstdlib>

// iloœæ liœci w i poza tornadem
#define NUMBER_LEAF_IN_TORNADO 1500
#define NUMBER_LEAF_OUT_TORNADO 1000

Scene::Scene():
	angle(0),
	lightPosition(0.0f, 0.0f, 0.0f, 1.0f)
{
	srand(time(NULL));
	for (int i = 0; i < NUMBER_LEAF_IN_TORNADO; i++)			// generujemy liœcie w tornadzie
	{
		leaves.push_back(new Leaf());
		leaves[i]->selfTurnAngle = rand() % 360;
	}

	for (int i = NUMBER_LEAF_IN_TORNADO; i < NUMBER_LEAF_IN_TORNADO + NUMBER_LEAF_OUT_TORNADO; i++)		// generujemy liœcie poza tornadem
	{
		// losujemy ich pozycje i k¹ty obrotu
		leaves.push_back(new Leaf());
		leaves[i]->position.x = rand() % 45 + 5;
		leaves[i]->position.RotateY(rand() % 360);
		leaves[i]->position.y = 0.5;
		leaves[i]->selfTurnAngle = rand() % 360;
		leaves[i]->angle = 0;
		leaves[i]->maxHeight = 0;
		leaves[i]->maxStep = 0;
	}

	// definiowanie œwiate³ w scenie
	Light sunLight(GL_LIGHT0, glm::vec4(0.2, 0.2, 0.2, 1), glm::vec4(0.8, 0.8, 0.8, 1), glm::vec4(1, 1, 1, 1), lightPosition);
	Light environmentLight(GL_LIGHT1, glm::vec4(3, 3, 3, 1), glm::vec4(0, 0, 0, 1), glm::vec4(1, 1, 1, 1), glm::vec4(0, 50, 0, 1));

	environmentLight.Activate();		// aktywujemy œwiat³o
	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glTranslatef(0.0f, -100.0f, 0.0f);	// modyfikacja po³o¿enia œwiat³a
	sunLight.Activate();				// aktywujemy œwiat³o

	glPopMatrix();

}

Scene::~Scene()
{
	for (int i = 0; i < leaves.size(); i++)		// czyœcimy pamiêæ
		delete(leaves[i]);
}

/*	Funkcja rysuj¹ca ca³¹ scenê
*/
void Scene::DrawScene()
{
	srand(time(NULL));

	angle++;									// zwiêkszamy dla ka¿dej klatki
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Textures::getTexture("leaf"));		// bindujemy odpowiedni¹ teksturê
	
	for (int i = 0; i < NUMBER_LEAF_IN_TORNADO; i++)		// tworzymy g³ówn¹ czêœæ tornada
	{
		glPushMatrix();
		
		glRotatef(angle + ((rand() % 100) / 100.0f), 0.0, 1.0, 0.0);
		glRotatef(17 * i, 0.0, 1.0, 0.0);
		glTranslatef(10 + (15.0 / NUMBER_LEAF_IN_TORNADO * i) + sin((i + angle * 0.05)) * 5, 10 + (30.0 / NUMBER_LEAF_IN_TORNADO * i) + cos((i + angle * 0.05)) * 5 + sin((i + angle * 0.05)) * 3, 0.0);
		glRotatef(leaves[i]->selfTurnAngle + angle * 0.01, 1.0, 0.0, 0.0);
		createLeaf();

		glPopMatrix();
	}

	// losujemy liœcia spoza g³ównej czêœci tornada, który ma siê poderwaæ i przelecieæ kawa³ek
	int los = NUMBER_LEAF_IN_TORNADO + rand() % NUMBER_LEAF_OUT_TORNADO;
	if (leaves[los]->angle == leaves[los]->maxStep)						// jeœli ju¿ nie leci to wyznacz mu modyfikatory lotu
	{
		leaves[los]->position.RotateY(leaves[los]->angle);				// zmieñ jego pozycjê wyznaczan¹ przez k¹t obrotu i dotychczasow¹ pozycjê
		leaves[los]->maxStep = rand() % 800 + 200;
		leaves[los]->maxHeight = rand() % 80;
		leaves[los]->angle = 0;
	}

	// rysowanie liœci spoza g³ównej czêœci tornada
	for (int i = NUMBER_LEAF_IN_TORNADO; i < NUMBER_LEAF_IN_TORNADO + NUMBER_LEAF_OUT_TORNADO; i++)
	{
		glPushMatrix();

		if (leaves[i]->angle != leaves[i]->maxStep)						// je¿eli jeszcze lec¹ to odpowiednio modyfikuj pozycjê
			glTranslatef(0.0, leaves[i]->maxHeight * (float)sin(M_PI*(180 * leaves[i]->angle++ / leaves[i]->maxStep) / 180), 0.0);
		glRotatef(leaves[i]->angle, 0.0, 1.0, 0.0);
		glTranslatef(leaves[i]->position.x, leaves[i]->position.y, leaves[i]->position.z);
		glRotatef(leaves[i]->selfTurnAngle, 1.0, 0.0, 0.0);
		createLeaf();

		glPopMatrix();
	}

	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// poni¿ej bêdzie wykonywane rysowanie krajobrazu
	glBindTexture(GL_TEXTURE_2D, Textures::getTexture("panorama"));
	glPushMatrix();

	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, TRUE);
	gluQuadricNormals(cylinder, GLU_SMOOTH);

	// Tworzenie listy wyœwietlania

	GLuint mycylinderID = glGenLists(1);
	glNewList(mycylinderID, GL_COMPILE);
	glTranslatef(0.0, 50.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(cylinder, 100.0, 100.0, 50, 20, 20);
	glEndList();
	gluDeleteQuadric(cylinder);
	glCallList(mycylinderID);

	glPopMatrix();

	glPushMatrix();

	// poni¿ej bêdziemy tworzyæ pod³o¿e pokryte tekstur¹ trawy
	glBindTexture(GL_TEXTURE_2D, Textures::getTexture("grass"));
	GLfloat TEXGRASS_SCALE = 10.0f;
	
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(TEXGRASS_SCALE, 0);
	glVertex3f(-300.0f, 0, -300.0f);

	glNormal3f(0, 0, -1);
	glTexCoord2f(TEXGRASS_SCALE, TEXGRASS_SCALE);
	glVertex3f(-300.0f, 0, 300.0f);

	glNormal3f(0, 0, -1);
	glTexCoord2f(0, TEXGRASS_SCALE);
	glVertex3f(300.0f, 0, 300.0f);

	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0);
	glVertex3f(300.0f, 0, -300.0f);

	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);

	glDisable(GL_TEXTURE_2D);

}

/*	Funkcja tworz¹ca pojedynczy lisc
*/
void Scene::createLeaf()
{
	GLfloat TEX_SCALE = -1.0f;
	glBegin(GL_POLYGON);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.5 * TEX_SCALE, 0.28 * TEX_SCALE);
	glVertex3f(0, -0.45, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.28 * TEX_SCALE, 0.23 * TEX_SCALE);
	glVertex3f(-0.45, -0.55, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.3 * TEX_SCALE, 0.35 * TEX_SCALE);
	glVertex3f(-0.4, -0.3, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.1 * TEX_SCALE, 0.5 * TEX_SCALE);
	glVertex3f(-0.8, 0, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.15 * TEX_SCALE, 0.55 * TEX_SCALE);
	glVertex3f(-0.7, 0.1, -0.05);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.13 * TEX_SCALE, 0.68 * TEX_SCALE);
	glVertex3f(-0.75, 0.35, -0.1);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.23 * TEX_SCALE, 0.65 * TEX_SCALE);
	glVertex3f(-0.55, 0.3, -0.05);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.25 * TEX_SCALE, 0.73 * TEX_SCALE);
	glVertex3f(-0.5, 0.45, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.38 * TEX_SCALE, 0.63 * TEX_SCALE);
	glVertex3f(-0.25, 0.25, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.35 * TEX_SCALE, 0.85 * TEX_SCALE);
	glVertex3f(-0.3, 0.7, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.43 * TEX_SCALE, 0.83 * TEX_SCALE);
	glVertex3f(-0.15, 0.65, -0.05);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.5 * TEX_SCALE, 0.95 * TEX_SCALE);
	glVertex3f(0, 0.9, -0.1);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.58 * TEX_SCALE, 0.83 * TEX_SCALE);
	glVertex3f(0.15, 0.65, -0.05);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.65 * TEX_SCALE, 0.85 * TEX_SCALE);
	glVertex3f(0.3, 0.7, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.63 * TEX_SCALE, 0.63 * TEX_SCALE);
	glVertex3f(0.25, 0.25, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.75 * TEX_SCALE, 0.73 * TEX_SCALE);
	glVertex3f(0.5, 0.45, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.78 * TEX_SCALE, 0.65 * TEX_SCALE);
	glVertex3f(0.55, 0.3, -0.05);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.88 * TEX_SCALE, 0.68 * TEX_SCALE);
	glVertex3f(0.75, 0.35, -0.1);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.85 * TEX_SCALE, 0.55 * TEX_SCALE);
	glVertex3f(0.7, 0.1, -0.05);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.9 * TEX_SCALE, 0.5 * TEX_SCALE);
	glVertex3f(0.8, 0, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.7 * TEX_SCALE, 0.35 * TEX_SCALE);
	glVertex3f(0.4, -0.3, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.73 * TEX_SCALE, 0.23 * TEX_SCALE);
	glVertex3f(0.45, -0.55, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.5 * TEX_SCALE, 0.28 * TEX_SCALE);
	glVertex3f(0, -0.45, 0);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.51 * TEX_SCALE, 0.05 * TEX_SCALE);
	glVertex3f(0.02, -0.9, 0.05);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0.49 * TEX_SCALE, 0.05 * TEX_SCALE);
	glVertex3f(-0.02, -0.9, 0.05);

	glEnd();
}