#include "lighter.h"

lighterObj::lighterObj(int light_src, GLfloat x, GLfloat y, GLfloat z,
    const GLfloat *light_ambient, const GLfloat *light_diffuse, const GLfloat *light_specular,
    bool point_light)
    : light_src(light_src), point_light(point_light) {
    this->pos[0] = x;
    this->pos[1] = y;
    this->pos[2] = z;

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightfv(light_src, GL_POSITION, pos);
    glLightfv(light_src, GL_AMBIENT, light_ambient);
    glLightfv(light_src, GL_DIFFUSE, light_diffuse);
    glLightfv(light_src, GL_SPECULAR, light_specular);
}

void lighterObj::reset(const GLfloat *mat_ambient, const GLfloat *mat_diffuse,
    const GLfloat *mat_specular, const GLfloat mat_shininess) {
    static GLfloat eyepos[] = {0, 0, 0, 0};  // eyepos[3] = 0 to set parallel light
    if (point_light)
        glLightfv(light_src, GL_POSITION, pos);
    else {
        eyepos[0] = camera.eyex;
        eyepos[1] = camera.eyey;
        eyepos[2] = camera.eyez;
        glLightfv(light_src, GL_POSITION, eyepos);
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void lighterObj::enable(void) {
    glEnable(GL_LIGHTING);
    glEnable(this->light_src);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
}
void lighterObj::disable() {
    glDisable(GL_LIGHTING);
    glDisable(this->light_src);
    glDisable(GL_AUTO_NORMAL);
    glDisable(GL_NORMALIZE);
}