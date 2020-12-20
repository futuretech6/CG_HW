/**
 * @file SolarSys.h
 * @author Scott Chen
 * @em 3180103012@zju.edu.cn
 * @brief
 */
#ifndef SOLARSYS_H
#define SOLARSYS_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

const double PI  = 3.1425927;
const double D2R = PI / 180;

/* Global Parameter Def */
// #define USING_WIRE  // Comment to use glutSolidSphere

const int spehereLineRatio   = 100;   // Control wire number of sphere
const int fluentRatio        = 2;     // The higher the value, the more fulent the animation
const double cam_move_step   = 0.25;  // Move step of camera using WSAD
const double cam_rotate_step = 1;     // Rotate step of camera using HJKL, in unit of degree
const double PRP_VRP_Dist    = 10;    // Distance from PRP to VRP
const double fovy            = 68;    // View volumn: angle of view
const double zNear           = 0.01;  // View volume: distance to near clipping plane
const double zFar            = 100;   // View volume: distance to far clipping plane

const GLfloat light_ambient[]  = {.5, .5, 0, .1};
const GLfloat light_diffuse[]  = {.5, .5, .5, .5};
const GLfloat light_specular[] = {.2, 0, .5, .5};

const GLfloat mat_ambient_sun[]  = {.9, 0.3, 0.0, 0.9};
const GLfloat mat_diffuse_sun[]  = {.9, 0.3, 0.0, 0.9};
const GLfloat mat_specular_sun[] = {0.2, 0.2, 0.2, 0.2};
const GLfloat mat_shininess_sun  = 11.0;

const GLfloat mat_ambient_earth[]  = {.2, .2, .6, .4};
const GLfloat mat_diffuse_earth[]  = {.2, .2, .6, .6};
const GLfloat mat_specular_earth[] = {.1, .1, .1, .2};
const GLfloat mat_shininess_earth  = 5.0;

const GLfloat mat_ambient_moon[]  = {.8, .8, .8, .4};
const GLfloat mat_diffuse_moon[]  = {.8, .8, .8, .6};
const GLfloat mat_specular_moon[] = {.1, .1, .1, .2};
const GLfloat mat_shininess_moon  = 3.0;

const GLfloat mat_ambient_jupyter[]  = {.7, .5, .5, .4};
const GLfloat mat_diffuse_jupyter[]  = {.7, .5, .5, .4};
const GLfloat mat_specular_jupyter[] = {.1, .1, .1, .8};
const GLfloat mat_shininess_jupyter  = 3.0;

const GLfloat mat_ambient_europa[]  = {.6, .5, .3, .4};
const GLfloat mat_diffuse_europa[]  = {.6, .5, .3, .4};
const GLfloat mat_specular_europa[] = {.1, .1, .1, .2};
const GLfloat mat_shininess_europa  = 1.0;

const GLfloat mat_ambient_satellite[]  = {1, .5, .5, .4};
const GLfloat mat_diffuse_satellite[]  = {1, .5, .5, .4};
const GLfloat mat_specular_satellite[] = {.6, .6, .6, .2};
const GLfloat mat_shininess_satellite  = 1.0;

void gl_init(int argc, char **argv);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, GLint x, GLint y);
void keyboard_special(GLint key, GLint x, GLint y);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void idle();
void draw_cylinder(
    GLfloat radius, GLfloat height, GLubyte R = 255, GLubyte G = 255, GLubyte B = 255);

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

    void lookAt() {
        double ctx = eyex + radicalDist * sin(polarAngle * D2R) * cos(azimuthAngle * D2R),
               cty = eyey + radicalDist * sin(polarAngle * D2R) * sin(azimuthAngle * D2R),
               ctz = eyez + radicalDist * cos(polarAngle * D2R);
        double upx = -radicalDist * cos(polarAngle * D2R) * cos(azimuthAngle * D2R),
               upy = -radicalDist * cos(polarAngle * D2R) * sin(azimuthAngle * D2R),
               upz = radicalDist * sin(polarAngle * D2R);
        gluLookAt(eyex, eyey, eyez, ctx, cty, ctz, upx, upy, upz);
    }

    void reset() {
        glLoadIdentity();
        lookAt();
        glutPostRedisplay();
    }

    void moveForward(int flag = 1) {
        eyex += flag * cam_move_step * sin(polarAngle * D2R) * cos(azimuthAngle * D2R);
        eyey += flag * cam_move_step * sin(polarAngle * D2R) * sin(azimuthAngle * D2R);
        eyez += flag * cam_move_step * cos(polarAngle * D2R);
    }
    void moveLeft(int flag = 1) {
        eyex -= flag * cam_move_step * sin(azimuthAngle * D2R);
        eyey += flag * cam_move_step * cos(azimuthAngle * D2R);
    }
    void moveUp(int flag = 1) {
        eyex -= flag * cos(polarAngle * D2R) * cos(azimuthAngle * D2R);
        eyey -= flag * cos(polarAngle * D2R) * sin(azimuthAngle * D2R);
        eyez += flag * cam_move_step * sin(polarAngle * D2R);
    }
    void rotateLeft(double step = cam_rotate_step, int flag = 1) {
        azimuthAngle += flag * step;
    }
    void rotateUp(double step = cam_rotate_step, int flag = 1) { polarAngle -= flag * step; }

    void moveBackward() { moveForward(-1); }
    void moveRight() { moveLeft(-1); }
    void moveDown() { moveUp(-1); }
    void rotateRight(double step = cam_rotate_step) { rotateLeft(step, -1); }
    void rotateDown(double step = cam_rotate_step) { rotateUp(step, -1); }
};

/**
 * @class an implenetation of celestial object include sun, planet and satellite
 * and other smaller objects
 */
class celestialObj {
  private:
    // Const parameters
    float radius;
    float distance;  // distance from center
    float rotateV;
    float revolveV;
    float rotateTilt;
    float revolveTilt;
    std::tuple<int, int, int> colors;  // [0, 255]

    // State variables
    float rotateAngle  = 0;
    float revolveAngle = 0;

  public:
    // Functions
    celestialObj(float radius, float distance, float rotateV, float revolveV,
        float rotateTilt, float revolveTilt, int r, int g, int b)
        : radius(radius),
          distance(distance),
          rotateV(rotateV / fluentRatio),
          revolveV(revolveV / fluentRatio),
          rotateTilt(rotateTilt),
          revolveTilt(revolveTilt),
          colors(std::make_tuple(r, g, b)){};

    void trans(void) {
        using namespace std;
        glTranslatef(distance * cos(rotateTilt * D2R) * cos(rotateAngle * D2R),
            distance * cos(rotateTilt * D2R) * sin(rotateAngle * D2R),
            distance * sin(rotateTilt * D2R) * cos(rotateAngle * D2R));
        glRotatef(revolveAngle, -sin(revolveTilt * D2R), 0., cos(revolveTilt * D2R));
        glRotatef(revolveTilt, 0., -1., 0.);  // First rotate to tile angle
    }
    void draw() {
        using namespace std;
        trans();
        glColor3f(get<0>(colors) / 255., get<1>(colors) / 255., get<2>(colors) / 255.);
#ifdef USING_WIRE
        glutWireSphere(radius, spehereLineRatio, spehereLineRatio);
#else
        glutSolidSphere(radius, spehereLineRatio, spehereLineRatio);
#endif
    }

    void rotate() {
        rotateAngle += rotateV;
        rotateAngle = (rotateAngle > 360) ? rotateAngle - 360 : rotateAngle;
        revolveAngle += revolveV;
        revolveAngle = (revolveAngle > 360) ? revolveAngle - 360 : revolveAngle;
    }
};

extern cameraObj camera;
/**
 * @class
 *
 */
class lighterObj {
  private:
    GLfloat pos[4] = {0, 0, 0, 1};
    int light_src;
    bool spot_light;

  public:
    lighterObj(int light_src, GLfloat x, GLfloat y, GLfloat z, const GLfloat *light_ambient,
        const GLfloat *light_diffuse, const GLfloat *light_specular, bool spot_light = 1)
        : light_src(light_src), spot_light(spot_light) {
        this->pos[0] = x;
        this->pos[1] = y;
        this->pos[2] = z;

        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glLightfv(light_src, GL_POSITION, pos);
        glLightfv(light_src, GL_AMBIENT, light_ambient);
        glLightfv(light_src, GL_DIFFUSE, light_diffuse);
        glLightfv(light_src, GL_SPECULAR, light_specular);
    }
    void reset(const GLfloat *mat_ambient, const GLfloat *mat_diffuse,
        const GLfloat *mat_specular, const GLfloat mat_shininess) {
        static GLfloat eyepos[] = {0, 0, 0, 0};  // eyepos[3] = 0 to set parallel light
        if (spot_light)
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

    void enable(void) {
        glEnable(GL_LIGHTING);
        glEnable(this->light_src);
        glEnable(GL_AUTO_NORMAL);
        glEnable(GL_NORMALIZE);
    }
    void disable() {
        glDisable(GL_LIGHTING);
        glDisable(this->light_src);
        glDisable(GL_AUTO_NORMAL);
        glDisable(GL_NORMALIZE);
    }
};

// /**
//  * @class
//  *
//  */
// class carObj : public celestialObj {

//   public:
//     carObj() : celestialObj(0, 6, 0, 2, 0, 0, 0, 0, 0) {}
//     ~carObj() {}

//     void initNurbs(int surface_num) {
//         theNurb_vec.resize(surface_num);
//         for (auto &theNurb : theNurb_vec) {
//             theNurb = gluNewNurbsRenderer();
//             gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
//             gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
//             gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid(*)())(this->nurbsError));
//         }
//     }

//     static void nurbsError(GLenum errorCode) {
//         const GLubyte *estring;
//         estring = gluErrorString(errorCode);
//         fprintf(stderr, "Nurbs Error: %s\n", estring);
//         exit(0);
//     }

//     void drawUp(void) {
//         GLfloat ctl_pt[4][4][3] = {{{1, 3, 0}, {1, 3, 0}, {3, 3, 0}, {8, 2.5, 0}},
//             {{0, 2, 0}, {1, 3, 2}, {7, 3, 3}, {9, 2, 0}},
//             {{0, 1, 0}, {1, 0, 2}, {7, 0, 3}, {9, 1, 0}},
//             {{1, 0, 0}, {1, 0, 0}, {3, 0, 0}, {8, 0.5, 0}}};
//         GLfloat knots[8]        = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
//         gluBeginSurface(theNurb_vec[0]);
//         gluNurbsSurface(theNurb_vec[0], 8, knots, 8, knots, 4 * 3, 3, &ctl_pt[0][0][0], 4,
//         4,
//             GL_MAP2_VERTEX_3);
//         gluEndSurface(theNurb_vec[0]);
//     }

//     void drawDown(void) {
//         GLfloat ctl_pt[4][4][3] = {{{1, 0, 0}, {1, 0, 0}, {3, 0, 0}, {8, 0.5, 0}},
//             {{0, 1, 0}, {1, 0, -1}, {7, 0, -1.6}, {9, 1, 0}},
//             {{0, 2, 0}, {1, 3, -1}, {7, 3, -1.6}, {9, 2, 0}},
//             {{1, 3, 0}, {1, 3, 0}, {3, 3, 0}, {8, 2.5, 0}}};
//         GLfloat knots[8]        = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
//         gluBeginSurface(theNurb_vec[1]);

//         gluNurbsSurface(theNurb_vec[1], 8, knots, 8, knots, 4 * 3, 3, &ctl_pt[0][0][0], 4,
//         4,
//             GL_MAP2_VERTEX_3);
//         gluEndSurface(theNurb_vec[1]);
//     }

//     void drawTires(void) {
//         // LF
//         glPushMatrix();
//         glTranslatef(7.5, 0.4, -0.1);
//         glRotatef(90, 1, 0, 0);
//         draw_cylinder(0.7, 0.2);
//         glPopMatrix();

//         // RF
//         glPushMatrix();
//         glTranslatef(7.5, 2.6, -0.1);
//         glRotatef(90, -1, 0, 0);
//         draw_cylinder(0.7, 0.2);
//         glPopMatrix();

//         // LB
//         glPushMatrix();
//         glTranslatef(2, -0.1, 0.1);
//         glRotatef(90, 1, 0, 0);
//         draw_cylinder(0.9, 0.4);
//         glPopMatrix();

//         // RB
//         glPushMatrix();
//         glTranslatef(2, 3.1, 0.1);
//         glRotatef(90, -1, 0, 0);
//         draw_cylinder(0.9, 0.4);
//         glPopMatrix();
//     }

//     void drawSpoiler(void) {
//         const GLfloat fan_y[] = {0.6, 2.4};
//         for (int i = 0; i < 2; i++) {
//             glBegin(GL_POLYGON);
//             glVertex3f(2, fan_y[i], 0.5);
//             glVertex3f(1, fan_y[i], 0.5);
//             glVertex3f(0.5, fan_y[i], 2);
//             glVertex3f(1.5, fan_y[i], 2);
//             glEnd();
//         }

//         glBegin(GL_POLYGON);
//         glVertex3f(0.5, fan_y[0] - 1, 2);
//         glVertex3f(0.5, fan_y[1] + 1, 2);
//         glVertex3f(1.5, fan_y[1] + 0.3, 2);
//         glVertex3f(1.5, fan_y[0] - 0.3, 2);
//         glEnd();
//     }

//     void drawCockpit(void) {
//         const GLfloat pt[8][3] = {{3, 0.5, 0.5}, {6.5, 0.8, 0.5}, {6.5, 2.2, 0.5},
//             {3, 2.5, 0.5}, {4.5, 1, 2.}, {5.5, 1.2, 1.8}, {5.5, 1.8, 1.8}, {4.5, 2, 2.}};
//         glBegin(GL_TRIANGLE_STRIP);
//         glVertex3f(pt[0][0], pt[0][1], pt[0][2]);
//         glVertex3f(pt[4][0], pt[4][1], pt[4][2]);
//         glVertex3f(pt[1][0], pt[1][1], pt[1][2]);
//         glVertex3f(pt[5][0], pt[5][1], pt[5][2]);
//         glVertex3f(pt[2][0], pt[2][1], pt[2][2]);
//         glVertex3f(pt[6][0], pt[6][1], pt[6][2]);
//         glVertex3f(pt[3][0], pt[3][1], pt[3][2]);
//         glVertex3f(pt[7][0], pt[7][1], pt[7][2]);
//         glVertex3f(pt[0][0], pt[0][1], pt[0][2]);
//         glVertex3f(pt[4][0], pt[4][1], pt[4][2]);
//         glEnd();
//         glBegin(GL_POLYGON);

//         glVertex3f(pt[4][0], pt[4][1], pt[4][2]);
//         glVertex3f(pt[5][0], pt[5][1], pt[5][2]);
//         glVertex3f(pt[6][0], pt[6][1], pt[6][2]);
//         glVertex3f(pt[7][0], pt[7][1], pt[7][2]);
//         glEnd();
//     }

//     void draw(void) {
//         drawUp();
//         drawDown();
//         drawTires();
//         drawSpoiler();
//         drawCockpit();
//     }

//   private:
//     std::vector<GLUnurbsObj *> theNurb_vec;
// };

#endif