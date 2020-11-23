/**
 * @author Scott Chen
 * @em 3180103012@zju.edu.cn
 * @brief This is an implementation to A2 of CG by prof. Hongzhi Wu
 * @note When drawing the star, use GL_LINE + GL_POLYGON is okay, but GL_FILL + GL_POLYGON will result in fill area error. That's why I use GL_TRIANGLE_STRIP.
 * @ref https://www.transmissionzero.co.uk/software/freeglut-devel/
 * @ref https://en.wikipedia.org/wiki/Flag_of_Jordan
 * @ref https://en.wikipedia.org/wiki/Flag_of_Jordan#/media/File:Flag_of_Jordan.svg
 * @ref https://www.ruanyifeng.com/blog/2018/08/svg.html
 */

// #include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Strips
    glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex2f(0., 5040.);
    glVertex2f(0., 0.);
    glVertex2f(10080., 0.);
    glVertex2f(10080., 5040.);
    glEnd();

    // Star

    // Edge
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glColor3f(0., 1., 0.);
    // glBegin(GL_POLYGON);
    // glVertex2f(0., 10.);
    // glVertex2f(0., 0.);
    // glVertex2f(10., 0.);
    // glVertex2f(10., 10.);
    // glEnd();

    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1200, 600);
    glutCreateWindow("Flag of Jordan");
    glClearColor(0., 0., 0., 1.);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 10080.0, 0.0, 5040.0, -1.0, 1.0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
