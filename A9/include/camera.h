#ifndef _CAMERA_H
#define _CAMERA_H

#include "SolarSys.h"

/**
 * @class an implenetation of camera with all its parameter and movement function
 */
class cameraObj {
  public:
    double eyex = 0, eyey = -PRP_VRP_Dist, eyez = 0;
    int mouse_oldx, mouse_oldy;

    double azimuthAngle      = 90,            // Counter-clock from x+
        polarAngle           = 90;            // Down from z+
    const double radicalDist = PRP_VRP_Dist;  // From origin

    void lookAt();
    void reset();
    void moveForward(int flag = 1);
    void moveLeft(int flag = 1);
    void moveUp(int flag = 1);
    void rotateLeft(double step = cam_rotate_step, int flag = 1);
    void rotateUp(double step = cam_rotate_step, int flag = 1);
    void moveBackward();
    void moveRight();
    void moveDown();
    void rotateRight(double step = cam_rotate_step);
    void rotateDown(double step = cam_rotate_step);
};

#endif