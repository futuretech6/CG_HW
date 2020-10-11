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
    glColor3f(0 / 256., 0 / 256., 0 / 256.);
    glBegin(GL_POLYGON);
    glVertex2f(0., 5040.);
    glVertex2f(0., 3360.);
    glVertex2f(10080., 3360.);
    glVertex2f(10080., 5040.);
    glEnd();

    glColor3f(255 / 256., 255 / 256., 255 / 256.);
    glBegin(GL_POLYGON);
    glVertex2f(0., 1680.);
    glVertex2f(0., 3360.);
    glVertex2f(10080., 3360.);
    glVertex2f(10080., 1680.);
    glEnd();

    glColor3f(0 / 256., 122 / 256., 61 / 256.);
    glBegin(GL_POLYGON);
    glVertex2f(0., 0.);
    glVertex2f(0., 1680.);
    glVertex2f(10080., 1680.);
    glVertex2f(10080., 0.);
    glEnd();

    // Triangle
    glColor3f(206 / 256., 17 / 256., 38 / 256.);
    glBegin(GL_TRIANGLES);
    glVertex2f(0., 5040.);
    glVertex2f(0., 0.);
    glVertex2f(5040., 2520.);
    glEnd();

    // Star
    glColor3f(255 / 256., 255 / 256., 255 / 256.);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1557, 2880);
    glVertex2f(1479, 2682);
    glVertex2f(1636, 2682);
    glVertex2f(1557, 2520);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1276, 2774);
    glVertex2f(1382, 2560);
    glVertex2f(1479, 2682);
    glVertex2f(1557, 2520);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1206, 2440);
    glVertex2f(1417, 2408);
    glVertex2f(1382, 2560);
    glVertex2f(1557, 2520);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1401, 2196);
    glVertex2f(1557, 2340);
    glVertex2f(1417, 2408);
    glVertex2f(1557, 2520);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1714, 2196);
    glVertex2f(1698, 2408);
    glVertex2f(1557, 2340);
    glVertex2f(1557, 2520);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1908, 2440);
    glVertex2f(1733, 2560);
    glVertex2f(1698, 2408);
    glVertex2f(1557, 2520);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(1839, 2744);
    glVertex2f(1636, 2682);
    glVertex2f(1733, 2560);
    glVertex2f(1557, 2520);
    glEnd();

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
