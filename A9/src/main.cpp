/**
 * @file SolarSys.cpp
 * @author Scott Chen
 * @em 3180103012@zju.edu.cn
 * @brief This is an implementation to A8 of CG by prof. Hongzhi Wu
 *
 * @ref https://www.solarsystemscope.com/textures/
 * @ref
 * https://cpp.hotexamples.com/examples/-/-/glTexImage2D/cpp-glteximage2d-function-examples.html
 * @ref https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glGenTextures.xml
 * @ref https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
 * @ref https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLoadIdentity.xml
 */
#include "SolarSys.h"
#include "camera.h"
// #include "car.h"
#include "celestial.h"
#include "lighter.h"
#include <vector>

cameraObj camera;
std::vector<celestialObj> starVec;

lighterObj lighter_point(GL_LIGHT0, 0, 0, 0, light_ambient, light_diffuse, light_specular);

lighterObj lighter_parallel(
    GL_LIGHT0, 0, 0, 0, light_ambient, light_diffuse, light_specular, 0);
// carObj car;

int main(int argc, char *argv[]) {
    // radius, distance, rotateV, revolveV, rotateTilt, revolveTilt, r, g, b
    starVec.emplace_back(2, 0, 0, 0.5, 0., 0., 255, 0, 0, "./texture/1k_sun.bmp");  // sun
    starVec.emplace_back(0.4, 3, 2, 6, 10., 23.5, 0, 0, 255,
        "./texture/1k_earth_daymap.bmp");  // earth
    starVec.emplace_back(
        0.18, 0.8, -1, 3, 0, 0., 255, 255, 255, "./texture/1k_moon.bmp");  // moon
    starVec.emplace_back(
        0.8, 10, 2, 2, -15., 10., 0, 255, 0, "./texture/1k_jupiter.bmp");  // jupiter
    starVec.emplace_back(
        0.5, 2.9, 3, 3, 80., 10., 255, 255, 0, "./texture/1k_europa.bmp");  // europa
    starVec.emplace_back(0.05, 1, 5, 3, 20., 90., 255, 0, 255);             // satellite

    // car.initNurbs(2);

    gl_init(argc, argv);

    // for (auto &i : starVec)
    //     i.tex_on();

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

    // Sun
    glPushMatrix();
    lighter_parallel.reset(
        mat_ambient_sun, mat_diffuse_sun, mat_specular_sun, mat_shininess_sun);
    lighter_parallel.enable();
    starVec[0].tex_on();
    starVec[0].draw();
    starVec[0].tex_off();
    lighter_parallel.disable();
    glPopMatrix();

    // Earth
    lighter_point.reset(
        mat_ambient_earth, mat_diffuse_earth, mat_specular_earth, mat_shininess_earth);
    lighter_point.enable();
    glPushMatrix();
    starVec[1].tex_on();
    starVec[1].draw();
    starVec[1].tex_off();
    glPopMatrix();
    lighter_point.disable();

    // Moon
    lighter_point.reset(
        mat_ambient_moon, mat_diffuse_moon, mat_specular_moon, mat_shininess_moon);
    lighter_point.enable();
    glPushMatrix();
    starVec[1].draw();
    starVec[2].tex_on();
    starVec[2].draw();
    starVec[2].tex_off();
    glPopMatrix();
    lighter_point.disable();

    // jupiter
    lighter_point.reset(mat_ambient_jupiter, mat_diffuse_jupiter, mat_specular_jupiter,
        mat_shininess_jupiter);
    lighter_point.enable();
    glPushMatrix();
    starVec[3].tex_on();
    starVec[3].draw();
    starVec[3].tex_off();
    glPopMatrix();
    lighter_point.disable();

    // Europa
    lighter_point.reset(
        mat_ambient_europa, mat_diffuse_europa, mat_specular_europa, mat_shininess_europa);
    lighter_point.enable();
    glPushMatrix();
    starVec[3].draw();
    starVec[4].tex_on();
    starVec[4].draw();
    starVec[4].tex_off();
    glPopMatrix();
    lighter_point.disable();

    // Satellite
    lighter_point.reset(mat_ambient_satellite, mat_diffuse_satellite, mat_specular_satellite,
        mat_shininess_satellite);
    lighter_point.enable();
    glPushMatrix();
    starVec[3].draw();
    starVec[4].draw();
    starVec[5].draw();
    lighter_point.disable();
    glPopMatrix();

    // glPushMatrix();
    // car.trans();
    // glScalef(0.5, 0.5, 0.5);
    // glTranslatef(-4.5, -1.5, 0);
    // car.draw();
    // glPopMatrix();

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
    // car.rotate();
    glutPostRedisplay();
    Sleep(30 / fluentRatio);
}