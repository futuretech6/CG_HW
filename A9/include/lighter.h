#ifndef _LIGHTER_H
#define _LIGHTER_H

#include "SolarSys.h"
#include "camera.h"

extern cameraObj camera;
/**
 * @class lighter's implementation
 *
 */
class lighterObj {
  private:
    GLfloat pos[4] = {0, 0, 0, 1};
    int light_src;
    bool point_light;

  public:
    lighterObj(int light_src, GLfloat x, GLfloat y, GLfloat z, const GLfloat *light_ambient,
        const GLfloat *light_diffuse, const GLfloat *light_specular, bool point_light = 1);
    void reset(const GLfloat *mat_ambient, const GLfloat *mat_diffuse,
        const GLfloat *mat_specular, const GLfloat mat_shininess);

    void enable(void);
    void disable();
};

#endif