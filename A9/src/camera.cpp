#include "camera.h"
#include <cmath>

void cameraObj::lookAt() {
    double ctx = eyex + radicalDist * sin(polarAngle * D2R) * cos(azimuthAngle * D2R),
           cty = eyey + radicalDist * sin(polarAngle * D2R) * sin(azimuthAngle * D2R),
           ctz = eyez + radicalDist * cos(polarAngle * D2R);
    double upx = -radicalDist * cos(polarAngle * D2R) * cos(azimuthAngle * D2R),
           upy = -radicalDist * cos(polarAngle * D2R) * sin(azimuthAngle * D2R),
           upz = radicalDist * sin(polarAngle * D2R);
    gluLookAt(eyex, eyey, eyez, ctx, cty, ctz, upx, upy, upz);
}

void cameraObj::reset() {
    glLoadIdentity();
    lookAt();
    glutPostRedisplay();
}

void cameraObj::moveForward(int flag) {
    eyex += flag * cam_move_step * sin(polarAngle * D2R) * cos(azimuthAngle * D2R);
    eyey += flag * cam_move_step * sin(polarAngle * D2R) * sin(azimuthAngle * D2R);
    eyez += flag * cam_move_step * cos(polarAngle * D2R);
}

void cameraObj::moveLeft(int flag) {
    eyex -= flag * cam_move_step * sin(azimuthAngle * D2R);
    eyey += flag * cam_move_step * cos(azimuthAngle * D2R);
}

void cameraObj::moveUp(int flag) {
    eyex -= flag * cos(polarAngle * D2R) * cos(azimuthAngle * D2R);
    eyey -= flag * cos(polarAngle * D2R) * sin(azimuthAngle * D2R);
    eyez += flag * cam_move_step * sin(polarAngle * D2R);
}

void cameraObj::rotateLeft(double step, int flag) {
    azimuthAngle += flag * step;
}

void cameraObj::rotateUp(double step, int flag) {
    polarAngle -= flag * step;
}

void cameraObj::moveBackward() {
    moveForward(-1);
}

void cameraObj::moveRight() {
    moveLeft(-1);
}

void cameraObj::moveDown() {
    moveUp(-1);
}

void cameraObj::rotateRight(double step) {
    rotateLeft(step, -1);
}

void cameraObj::rotateDown(double step) {
    rotateUp(step, -1);
}