/**
 * @file SolarSys.cpp
 * @author Scott Chen
 * @em 3180103012@zju.edu.cn
 * @brief This is an implementation to A4 of CG by prof. Hongzhi Wu
 *
 * @ref http://cse.csusb.edu/tongyu/courses/cs621/notes/color.php
 *
 */
#include "SolarSys.h"

cameraObj camera;
std::vector<celestialObj> starVec;

lighterObj lighter1, lighter2;
carObj car;

int main(int argc, char *argv[]) {
    // radius, distance, rotateV, revolveV, rotateTilt, revolveTilt, r, g, b
    starVec.emplace_back(2, 0, 0, 0.5, 0., 0., 255, 0, 0);        // sun
    starVec.emplace_back(0.4, 3, -3, -2, 10., 23.5, 0, 0, 255);   // earth
    starVec.emplace_back(0.1, 0.8, 9, 3, 0, 0., 255, 255, 255);   // moon
    starVec.emplace_back(1.1, 10, 1, 2, -45., 60., 0, 255, 0);    // jupyter
    starVec.emplace_back(0.5, 1.9, 5, 3, 80., 10., 255, 255, 0);  // europa
    starVec.emplace_back(0.2, 1, 10, 3, 20., 90., 255, 0, 255);   // something

    car.initNurbs(2);

    gl_init(argc, argv);
    lighter1.init(0, 0, 0);  // Must be placed after Shade model set

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}

void gl_init(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Apocalypse System");
    glClearColor(0., 0., 0., 0.);
    glShadeModel(GL_FLAT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lighter1.enable();

    glPushMatrix();
    starVec[0].draw();
    glPopMatrix();

    glPushMatrix();
    starVec[1].draw();
    starVec[2].draw();
    glPopMatrix();

    glPushMatrix();
    starVec[3].draw();
    starVec[4].draw();
    starVec[5].draw();
    glPopMatrix();

    glPushMatrix();
    car.trans();
    glScalef(0.5, 0.5, 0.5);
    glTranslatef(-4.5, -1.5, 0);
    car.draw();
    glPopMatrix();

    lighter1.disable();

    glFlush();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fovy, (double)w / (double)h, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.lookAt();
}

void keyboard(unsigned char key, GLint x, GLint y) {
    bool changed = 1;
    switch (key) {
        case 'w': camera.moveForward(); break;
        case 's': camera.moveBackward(); break;
        case 'a': camera.moveLeft(); break;
        case 'd': camera.moveRight(); break;

        case 'h': camera.rotateLeft(); break;
        case 'j': camera.rotateDown(); break;
        case 'k': camera.rotateUp(); break;
        case 'l': camera.rotateRight(); break;

        case '=': camera.moveUp(); break;
        case '-': camera.moveDown(); break;

        case 'q': exit(0); break;
        case 27: exit(0); break;

        default: changed = 0; break;
    }
    if (changed)
        camera.reset();
}

void keyboard_special(GLint key, GLint x, GLint y) {
    bool changed = 1;
    switch (key) {
        case GLUT_KEY_UP: camera.rotateUp(); break;
        case GLUT_KEY_DOWN: camera.rotateDown(); break;
        case GLUT_KEY_LEFT: camera.rotateLeft(); break;
        case GLUT_KEY_RIGHT: camera.rotateRight(); break;
        default: changed = 0; break;
    }
    if (changed)
        camera.reset();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        camera.mouse_oldx = x;
        camera.mouse_oldy = y;
    }
}

void mouseMove(int x, int y) {
    camera.rotateLeft((x - camera.mouse_oldx) / 5);
    camera.rotateUp((y - camera.mouse_oldy) / 5);
    camera.mouse_oldx = x;
    camera.mouse_oldy = y;
    camera.reset();
}

void idle() {
    for (auto &i : starVec)
        i.rotate();
    car.rotate();
    glutPostRedisplay();
    Sleep(30 / fluentRatio);
}

void draw_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B) {
    GLfloat x                    = 0.0;
    GLfloat y                    = 0.0;
    const GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3ub(R, G, B);
    glBegin(GL_QUAD_STRIP);
    for (GLfloat angle = 0.0; angle <= 2 * PI + angle_stepsize; angle += angle_stepsize) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, 0.5 * height);
        glVertex3f(x, y, -0.5 * height);
    }
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3ub(R, G, B);
    glBegin(GL_POLYGON);
    for (GLfloat angle = 0.0; angle <= 2 * PI; angle += angle_stepsize) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, 0.5 * height);
    }
    glEnd();

    /** Draw the circle on bottom of cylinder */
    glColor3ub(R, G, B);
    glBegin(GL_POLYGON);
    for (GLfloat angle = 2 * PI; angle >= 0; angle -= angle_stepsize) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, -0.5 * height);
    }
    glEnd();
}