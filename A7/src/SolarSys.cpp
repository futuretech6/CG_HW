/**
 * @file SolarSys.cpp
 * @author Scott Chen
 * @em 3180103012@zju.edu.cn
 * @brief This is an implementation to A4 of CG by prof. Hongzhi Wu
 *
 * @ref glMatrix(): https://community.khronos.org/t/glpushmatrix-glpopmatrix/20189/2
 * @ref glRotatef(): axis from the origin through the point (x, y, z).
 * @ref gluLookAt():
 *      https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml
 * @ref spherical coordinate:
 *      https://zh.wikipedia.org/wiki/%E7%90%83%E5%BA%A7%E6%A8%99%E7%B3%BB
 * @ref object hidding: https://blog.csdn.net/Wadejr/article/details/7489928
 * @ref lighting: http://www.csc.villanova.edu/~mdamian/Past/graphicssp13/notes/lightlab.pdf
 * @ref lighting: https://www.glprogramming.com/red/chapter05.html
 * @ref NURBS_lighting: https://www.glprogramming.com/red/chapter12.html
 */
#include "SolarSys.h"

cameraObj camera;
std::vector<celestialObj> starVec;

lighterObj lighter;
carObj car;

int main(int argc, char *argv[]) {
    // radius, distance, rotateV, revolveV, rotateTilt, revolveTilt, r, g, b
    starVec.emplace_back(2, 0, 0, 0.5, 0., 0., 255, 0, 0);        // sun
    starVec.emplace_back(0.4, 3, -3, -2, 10., 23.5, 0, 0, 255);   // earth
    starVec.emplace_back(0.1, 0.8, 9, 3, 0, 0., 255, 255, 255);   // moon
    starVec.emplace_back(1.1, 10, 1, 2, -45., 60., 0, 255, 0);    // jupyter
    starVec.emplace_back(0.5, 1.9, 5, 3, 80., 10., 255, 255, 0);  // europa
    starVec.emplace_back(0.2, 1, 10, 3, 20., 90., 255, 0, 255);   // something

    car.init(2);

    gl_init(argc, argv);
    lighter.init();  // Must be placed after Shade model set

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
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
    lighter.enable();
    glTranslatef(4, 0, 0);
    glScalef(0.5, 0.5, 0.5);
    car.drawUp();
    car.drawDown();
    lighter.disable();
    glPopMatrix();

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

void keyboard(unsigned char key, int x, int y) {
    bool changed = 1;
    switch (key) {
        case 'w': camera.moveUp(); break;
        case 's': camera.moveDown(); break;
        case 'a': camera.moveLeft(); break;
        case 'd': camera.moveRight(); break;
        case 'q': camera.moveBackward(); break;
        case 'e': camera.moveForward(); break;
        case 'h': camera.rotateLeft(); break;
        case 'j': camera.rotateDown(); break;
        case 'k': camera.rotateUp(); break;
        case 'l': camera.rotateRight(); break;
        case 27: exit(0); break;
        default: changed = 0; break;
    }
    if (changed) {
        camera.reset();
    }
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
    glutPostRedisplay();
    Sleep(30 / fluentRatio);
}
