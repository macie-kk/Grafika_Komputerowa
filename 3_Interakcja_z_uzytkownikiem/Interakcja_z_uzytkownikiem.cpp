#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

# define M_PI 3.141592653589793238462643383279502884L


using namespace std;
struct Point3D {
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
};
typedef float point3[3];
const int N = 40;
int egg_model = 1;	// model jajka :: 1 - punkty :: 2 - linie :: 3 - wypelnione
int model = 1;	// wyswietlany model :: 1 - czajnik :: 2 - jajko
int view = 1; // tryb widoku :: 1 - modyfikacja obiektow :: 2 - modyfikacja kamery
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat thetaX = 0.0;   // kąt obrotu obiektu
static GLfloat thetaY = 0.0;   // kąt obrotu obiektu
static GLfloat thetaZ = 0.0;   // kąt obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLint mouse_status = 0;      // stan klawiszy myszy :: 0 - nie naciśnięto żadnego klawisza :: 1 - naciśnięty zostać lewy klawisz
static int x_pos_old = 0;     // poprzednia pozycja kursora myszy
static int y_pos_old = 0;     // poprzednia pozycja kursora myszy
static int z_pos_old = 0;     // poprzednia pozycja kursora myszy
static int delta_x = 0;       // różnica pomiędzy pozycją bieżącą i poprzednią kursora myszy
static int delta_y = 0;       // różnica pomiędzy pozycją bieżącą i poprzednią kursora myszy
static int delta_z = 0;       // różnica pomiędzy pozycją bieżącą i poprzednią kursora myszy
static int azymut = 0;
static int elewacja = 60;
static int azymut_old = 0;
static int elewacja_old = 0;
static GLfloat delta_azymut = 0;
static GLfloat delta_elewacja = 0;
static GLfloat R = 5;
static Point3D colors[N][N];

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

void Egg() {
	static Point3D points[N][N];

	for (int i = 0; i < N; i++) {
		float u = ((float)i) / (N - 1);

		for (int j = 0; j < N; j++) {
			float v = ((float)j) / (N - 1);

			points[i][j].x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
			points[i][j].y = -5 + 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
			points[i][j].z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);
		}
	}

	switch (egg_model) {
	case 1: // punkty
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				glColor3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glBegin(GL_POINTS);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glEnd();
			}
		}
		break;

	case 2: // siatka
		for (int i = 0; i < N - 1; i++) {
			for (int j = 0; j < N - 1; j++) {
				glColor3f(0, 1, 0);
				glBegin(GL_LINES);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
				glEnd();

				glBegin(GL_LINES);
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glEnd();
			}
		}
		break;

	case 3:	// trojkaty
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
				glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glEnd();
			}
		}
		break;
	}
}

void Axes(void) {
	point3  x_min = { -10.0, 0.0, 0.0 };
	point3  x_max = { 10.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -10.0, 0.0 };
	point3  y_max = { 0.0,  10.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -10.0 };
	point3  z_max = { 0.0, 0.0,  10.0 };

	//  pocz?tek i koniec obrazu osi y
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

void Mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;         // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		mouse_status = 1;          // wcięnięty został lewy klawisz myszy
		return;
	}
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		z_pos_old = y;
		mouse_status = 2;
		return;
	}
	mouse_status = 0;          // nie został wcięnięty żaden klawisz
}

void Motion(GLsizei x, GLsizei y) {
	delta_x = x - x_pos_old;
	delta_y = y - y_pos_old;
	delta_z = y - z_pos_old;
	delta_azymut = (x - azymut_old);
	delta_elewacja = (y - elewacja_old);

	x_pos_old = x;
	y_pos_old = y;
	z_pos_old = y;
	azymut_old = x;
	elewacja_old = y;
	glutPostRedisplay();     // przerysowanie obrazu sceny
} 

void setCamera() {
	viewer[0] = R * cos(azymut) * cos(elewacja);
	viewer[1] = R * sin(elewacja);
	viewer[2] = R * sin(azymut) * cos(elewacja);
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Axes(); // Zdefiniowanie położenia obserwatora

	// jeśli lewy klawisz myszy wcisnięty
	if (mouse_status == 1 && view == 1) {
		thetaX += delta_x * pix2angle;   
		thetaY += delta_y * pix2angle;
	}

	// jeśli prawy klawisz myszy wcisnięty
	if (mouse_status == 2 && view == 1) {
		thetaZ = delta_z * pix2angle;

		glMatrixMode(GL_PROJECTION);
		glTranslatef(0, 0, thetaZ);
		glMatrixMode(GL_MODELVIEW); 
	}

	// to samo dla ruchu kamerą
	if (mouse_status == 2 && view == 2) {
		R += delta_y;
		if (R < 0.25) R = 0.5;
		if (R > 15) R = 15;
		setCamera();
	}

	if (mouse_status == 1 && view == 2) {
		azymut += delta_azymut * pix2angle;
		elewacja += delta_elewacja * pix2angle;
		setCamera();
	}

	glRotatef(thetaY, 1.0, 0.0, 0.0); 
	glRotatef(thetaX, 0.0, 1.0, 0.0); 

	glColor3f(0.0f, 0.0f, 0.0f);
	if (model == 1) glutWireTeapot(3.0);
	if (model == 2) Egg();

	glFlush();
	glutSwapBuffers();
}

void MyInit(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
	pix2angle = 360.0 / (float)horizontal;
	glMatrixMode(GL_PROJECTION); // Przełączenie macierzy bieżącej na macierz projekcji
	glLoadIdentity();
	 gluPerspective(70, 1.0, 1.0, 30.0); // Ustawienie parametrów dla rzutu perspektywicznego

	if (horizontal <= vertical) glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
	else glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW); // Przełączenie macierzy bieżącej na macierz widoku modelu
	glLoadIdentity();
}

void keys(unsigned char key, int x, int y) {
	if (key == 'q') egg_model = 1;
	if (key == 'w') egg_model = 2;
	if (key == 'e') egg_model = 3;

	if (key == 'a') model = 1;
	if (key == 's') model = 2;

	if (key == 'z') view = 1;
	if (key == 'x') {
		gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glFlush();
		view = 2;
	}

	string viewmodel = model == 1 ? "czajnik" : "jajko";
	string viewtype = view == 1 ? "obiekty" : "kamera";
	cout << "Model: " << viewmodel << " :: Tryb: " << viewtype << "      \r";
	RenderScene(); // przerysowanie obrazu sceny
}

void main(void) {
	cout << "Sterowanie: " << endl;
	cout << "Zmiana modelu: [a] = czajnik :: [s] = jajko" << endl;
	cout << "Zmiana trybu poruszania: [z] = obiekty :: [x] = kamera" << endl;
	cout << "Zmiana modelu jajka: [q/w/e]" << endl << endl;

	InitColors();
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(glutGet(GLUT_INIT_WINDOW_WIDTH) / 2 + 300, glutGet(GLUT_INIT_WINDOW_HEIGHT) / 2 - 150);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Rzutowanie perspektywiczne");
	glutDisplayFunc(RenderScene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keys);

	// Określenie, że funkcja RenderScene będzie funkcją zwrotną
	// (callback function).  Będzie ona wywoływana za każdym razem
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
	// za zmiany rozmiaru okna                       

	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystąpieniem do renderowania
	glEnable(GL_DEPTH_TEST); // Włączenie mechanizmu usuwania niewidocznych elementów sceny
	glutMainLoop();
}
