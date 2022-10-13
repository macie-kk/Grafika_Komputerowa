/*************************************************************************************/
// Trochę bardziej skomplikowany program wykorzystujący funkcje biblioteki OpenGL
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

using namespace std;
/*************************************************************************************/
// Funkcaja określająca, co ma być rysowane
// (zawsze wywoływana, gdy trzeba przerysować scenę)


int W_SIZE;

float RandomFloat() {
    return (rand() % 100) * 0.01;
}

void RandomGlColor() {
    glColor3f(RandomFloat(), RandomFloat(), RandomFloat());
}

void DrawCarpet(GLfloat x, GLfloat y, GLfloat size, GLint level) {
    if (level > 0) {
        level -= 1;
        size /= 3;

        DrawCarpet(x, y, size, level);                // prawy gorny
        DrawCarpet(x - size, y, size, level);        // srodkowy gorny
        DrawCarpet(x - 2 * size, y, size, level);    // lewy gorny

        y -= size;

        DrawCarpet(x, y, size, level);               // prawy srodkowy
        DrawCarpet(x - 2 * size, y, size, level);   // lewy srodkowy

        y -= size;

        DrawCarpet(x, y, size, level);               // prawy gorny
        DrawCarpet(x - size, y, size, level);       // srodkowy gorny
        DrawCarpet(x - 2 * size, y, size, level);   // lewy gorny
    } else {
        glBegin(GL_POLYGON);
            RandomGlColor();
            glVertex2f(x, y);
            RandomGlColor();
            glVertex2f(x, y - size);
            RandomGlColor();
            glVertex2f(x - size, y - size);
            RandomGlColor();
            glVertex2f(x - size, y);
        glEnd();
    }
}


void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    DrawCarpet(50, 50, W_SIZE/3, 3);
    //rysuj_dywan(50, 50, 100, poziom);
    glFlush();
    // Przekazanie poleceń rysujących do wykonania
}

/*************************************************************************************/
// Funkcja ustalająca stan renderowania
void MyInit(void) {
    W_SIZE = glutGet(GLUT_WINDOW_WIDTH);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // Kolor okna wnętrza okna - ustawiono na szary
}
/*************************************************************************************/
// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
// niezależnie od rozmiarów okna graficznego
void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szerokość i wysokość okna) są
// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna
{

    GLfloat AspectRatio;
    // Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna
    if (vertical == 0)
        // Zabezpieczenie pzred dzieleniem przez 0
        vertical = 1;
    glViewport(0, 0, horizontal, vertical);
    // Ustawienie wielkościokna okna urządzenia (Viewport)
    // W tym przypadku od (0,0) do (horizontal, vertical)
    glMatrixMode(GL_PROJECTION);
    // Określenie układu współrzędnych obserwatora
    glLoadIdentity();
    // Określenie przestrzeni ograniczającej

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
    // Wyznaczenie współczynnika proporcji okna
    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // określenie okna obserwatora.
    // Pozwala to zachować właściwe proporcje rysowanego obiektu
    // Do określenia okna obserwatora służy funkcja glOrtho(...)
    if (horizontal <= vertical)
        glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
    else
        glOrtho(-100.0 * AspectRatio, 100.0 * AspectRatio, -100.0, 100.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    // Określenie układu współrzędnych
    glLoadIdentity();
}
/*************************************************************************************/
// Główny punkt wejścia programu. Program działa w trybie konsoli
int main(void) {
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    // Ustawienie trybu wyświetlania
    // GLUT_SINGLE - pojedynczy bufor wyświetlania
    // GLUT_RGBA - model kolorów RGB
    glutCreateWindow("Drugi program w OpenGL");
    // Utworzenie okna i określenie treści napisu w nagłówku okna
    glutDisplayFunc(RenderScene);
    // Określenie, że funkcja RenderScene będzie funkcją zwrotną (callback)
    // Biblioteka GLUT będzie wywoływała tą funkcję za każdym razem, gdy
    // trzeba będzie przerysować okno
    glutReshapeFunc(ChangeSize);
    // Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną za
    // zmiany rozmiaru okna
    MyInit();
    // Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie
    // inicjalizacje konieczneprzed przystąpieniem do renderowania
    glutMainLoop();
    // Funkcja uruchamia szkielet biblioteki GLUT
}
