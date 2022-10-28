/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi 
//  układu współrzednych

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

# define M_PI 3.141592653589793238462643383279502884L

using namespace std;
typedef float point3[3];
struct Point3D {
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
};

static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy kąty obrotu
int deg = 45;
int model = 1; // 1- punkty, 2- siatka, 3 - wypełnione trójkąty
const int N = 50;
const bool autorotate = true;
Point3D colors[N][N];

float RandomFloat() {
	return (rand() % 101) * 0.01;
}

void InitColors() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			colors[i][j].x = RandomFloat();
			colors[i][j].y = RandomFloat();
			colors[i][j].z = RandomFloat();
		}
	}
}

void spinEgg() {
	theta[0] -= 0.5;
	if (theta[0] > 360.0) theta[0] -= 360.0;

	theta[1] -= 0.5;
	if (theta[1] > 360.0) theta[1] -= 360.0;

	theta[2] -= 0.5;
	if (theta[2] > 360.0) theta[2] -= 360.0;

	glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}

void Axes(void) {
	point3  x_min = { -1.0, 0.0, 0.0 };
	point3  x_max = { 1.0, 0.0, 0.0 };
	// początek i koniec obrazu osi x

	point3  y_min = { 0.0, -1.0, 0.0 };
	point3  y_max = { 0.0,  1.0, 0.0 };
	// początek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -1.0 };
	point3  z_max = { 0.0, 0.0,  1.0 };
	//  początek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();

}

void Egg() {
	Point3D points[N][N];

	for (int i = 0; i < N; i++) {
		float u = ((float)i) / (N - 1);

		for (int j = 0; j < N; j++) {
			float v = ((float)j) / (N - 1);

			points[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI * v);
			points[i][j].y = -5 + 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			points[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI * v);
		}
	}

	switch (model) {
	case 1:
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				glColor3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glBegin(GL_POINTS);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glEnd();
			}
		}
		break;
	case 2:
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < N - 1; j++) {
				glColor3f(0, 1, 0);
				glBegin(GL_LINES);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				glEnd();

				glBegin(GL_LINES);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
				glEnd();
			}
		}
		break;
	case 3:
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < N - 1; j++) {
				glBegin(GL_TRIANGLES);
				glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glEnd();

				glBegin(GL_TRIANGLES);
				glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glEnd();
			}
		}
	}
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Axes();
	glRotated(deg, 1, 1, 1);
	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na biały
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	Egg();
	glFlush();
	glutSwapBuffers();
}

void MyInit(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
	GLfloat AspectRatio;
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;          
	if (horizontal <= vertical)
		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void keys(unsigned char key, int x, int y) {
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;
	if (key == 'a') deg -= 2;
	if (key == 'd') deg += 2;

	RenderScene(); // przerysowanie obrazu sceny
}

void main(void) {
	InitColors();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(glutGet(GLUT_INIT_WINDOW_WIDTH) / 2 + 300, glutGet(GLUT_INIT_WINDOW_HEIGHT) / 2 - 150);
	glutCreateWindow("Układ współrzędnych 3-D");
	glutKeyboardFunc(keys);
	glutDisplayFunc(RenderScene);
	if(autorotate) glutIdleFunc(spinEgg);
	glutReshapeFunc(ChangeSize);     
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}

