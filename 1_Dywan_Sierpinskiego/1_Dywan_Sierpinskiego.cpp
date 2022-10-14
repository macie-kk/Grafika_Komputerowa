#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

using namespace std;

int W_SIZE;
int POZIOM = 4;
float DEFORMACJA = 0.5;
boolean KOLORY = 1;

float RandomFloat() {
	return (rand() % 100) * 0.01;
}

void RandomGlColor() {
	if (KOLORY) {
		glColor3f(RandomFloat(), RandomFloat(), RandomFloat());
	} else {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void RenderCarper(GLfloat x, GLfloat y, GLfloat size, GLint level) {
	if (level > 0) {
		size /= 3;
		level -= 1;

		RenderCarper(x, y, size, level);			// prawy gorny
		RenderCarper(x - size, y, size, level);		// srodkowy gorny
		RenderCarper(x - 2 * size, y, size, level);	// lewy gorny

		y -= size;	// zejscie poziom nizej

		RenderCarper(x, y, size, level);			// prawy srodkowy
		RenderCarper(x - 2 * size, y, size, level);	// lewy srodkowy

		y -= size;	// zejscie poziom nizej

		RenderCarper(x, y, size, level);			// prawy dolny
		RenderCarper(x - size, y, size, level);		// srodkowy dolny
		RenderCarper(x - 2 * size, y, size, level);	// lewy dolny

		return;
	}

	GLfloat def = (rand() % 40 - 20) * 0.1 * DEFORMACJA;

	glBegin(GL_QUADS);
	RandomGlColor();
	glVertex2f(x + def, y + def);
	RandomGlColor();
	glVertex2f(x + def, y - size + def);
	RandomGlColor();
	glVertex2f(x - size + def, y - size + def);
	RandomGlColor();
	glVertex2f(x - size + def, y + def);
	glEnd();
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	RenderCarper(W_SIZE / 9, W_SIZE / 9, W_SIZE / 4.5, POZIOM);
	glFlush();
}



void MyInit(void) {
	W_SIZE = glutGet(GLUT_WINDOW_WIDTH);

	cout << "Stopien deformacji: " << DEFORMACJA << endl;
	cout << "Stopien rekurencji: " << POZIOM << endl;
	cout << "Tryb kolorow: " << (KOLORY ? "Kolorowe" : "Monochromatyczne") << endl;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
	GLfloat AspectRatio;
	if (vertical == 0) {
		vertical = 1;
	}
	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(void) {
	float choice;

	cout << "> Stopien deformacji [0 - 1]: ";
	cin >> DEFORMACJA;
	cout << endl << "> Stopien rekurencji [1 - 5]: ";
	cin >> POZIOM;
	cout << endl << "> Kolory? [1 / 0]: ";
	cin >> KOLORY;

	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Drugi program w OpenGL");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMainLoop();
}
