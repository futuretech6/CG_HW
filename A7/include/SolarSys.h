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

const double pi  = 3.1425927;
const double D2R = pi / 180;

/* Global Parameter Def */
// #define USING_WIRE                    // Comment to use glutSolidSphere
const int spehereLineRatio   = 25;    // Control wire number of sphere
const int fluentRatio        = 2;     // The higher the value, the more fulent the animation
const double cam_move_step   = 0.25;  // Move step of camera using WSAD
const double cam_rotate_step = 1;     // Rotate step of camera using HJKL, in unit of degree
const double PRP_VRP_Dist    = 10;    // Distance from PRP to VRP
const double fovy            = 68;    // View volumn: angle of view
const double zNear           = 0.1;   // View volume: distance to near clipping plane
const double zFar            = 100;   // View volume: distance to far clipping plane

// const GLfloat light_position0[] = {3, 0, 6, 1.0};
// const GLfloat light_ambient[]   = {0.5, 0.5, 0.5, 0.5};
// const GLfloat light_diffuse[]   = {0.5, 0.5, 0.5, 0.5};
// const GLfloat light_specular[]  = {0.5, 0.5, 0.5, 0.5};
// const GLfloat mat_specular[]    = {1.0, 1.0, 1.0, 1.0}; /* bright white */
// const GLfloat mat_diffuse[]     = {1.0, 0.5, 0.0, 1.0}; /* orange color */
// const GLfloat mat_ambient[]     = {1.0, 0.5, 0.0, 1.0}; /* same as diffuse */
// const GLfloat mat_shininess     = 5.0;

GLfloat mat_specular[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[]  = {50.0};
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

/**
 * @class an implenetation of camera with all its parameter and movement function
 */
class cameraObj {
  public:
    double eyex = 0, eyey = -PRP_VRP_Dist, eyez = 0;
    int mouse_oldx, mouse_oldy;

    double azimuthAngle = 90,            // Counter-clock from x+
        polarAngle      = 90,            // Down from z+
        radicalDist     = PRP_VRP_Dist;  // From origin

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

    void rotateLeft(double step = cam_rotate_step, int flag = 1) {
        azimuthAngle += flag * step;
    }

    void rotateUp(double step = cam_rotate_step, int flag = 1) { polarAngle -= flag * step; }

    void moveBackward() { moveForward(-1); }
    void moveRight() { moveLeft(-1); }
    void rotateRight(double step = cam_rotate_step) { rotateLeft(step, -1); }
    void rotateDown(double step = cam_rotate_step) { rotateUp(step, -1); }
};

/**
 * @class an implenetation of celestial object include sun, planet and satellite
 * and other smaller objects
 */
class celestialObj {
  public:
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

    void draw() {
        using namespace std;
        glColor3f(get<0>(colors) / 255., get<1>(colors) / 255., get<2>(colors) / 255.);
        glTranslatef(distance * cos(rotateTilt * D2R) * cos(rotateAngle * D2R),
            distance * cos(rotateTilt * D2R) * sin(rotateAngle * D2R),
            distance * sin(rotateTilt * D2R) * cos(rotateAngle * D2R));
        glRotatef(revolveAngle, -sin(revolveTilt * D2R), 0., cos(revolveTilt * D2R));
        glRotatef(revolveTilt, 0., -1., 0.);  // First rotate to tile angle

        // glutWireSphere(radius, radius * spehereLineRatio, radius * spehereLineRatio);

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

/**
 * @class
 *
 */
class lighterObj {
  public:
    // lighterObj() { this->init(); }

    void init(void) {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_SMOOTH);

        // glLightfv(GL_LIGHT1, GL_POSITION, light_position0);
        // glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        // glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        // glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

        // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        // glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        // glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        // glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        // glEnable(GL_AUTO_NORMAL);
        // glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);
    }

    static void enable(void) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT1);
        glEnable(GL_FRONT);
    }
    static void disable() {
        glDisable(GL_FRONT);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHTING);
    }
};

/**
 * @class
 *
 */
class carObj {
  public:
    carObj() {}
    ~carObj() {}

    void init(int surface_num) {
        theNurb_vec.resize(surface_num);
        for (auto &theNurb : theNurb_vec) {
            theNurb = gluNewNurbsRenderer();
            gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
            gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
            gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid(*)())(this->nurbsError));
        }
    }

    void drawUp(lighterObj *pLighter) {
        GLfloat ctl_pt[4][4][3] = {
            {{-2.0, -2.0, 1.0}, {-0.5, -2.0, 0.0}, {0.5, -2.0, -2.0}, {2.0, -2.0, 2.0}},
            {{-2.0, -0.5, 2.0}, {-0.5, -0.5, 1.5}, {0.5, -0.5, 0.0}, {2.0, -0.5, -2.0}},
            {{-2.0, 0.5, 2.0}, {-0.5, 0.5, 1.0}, {0.5, 0.5, -1.0}, {2.0, 0.5, 1.0}},
            {{-2.0, 2.0, -1.0}, {-0.5, 2.0, -1.0}, {0.5, 2.0, 0.0}, {2.0, 2.0, -0.5}}};
        GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
        int i, j;

        if (showPoints) {
            glPointSize(5.0);
            glColor3f(1.0, 1.0, 0.0);
            glBegin(GL_POINTS);
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    glVertex3f(ctl_pt[i][j][0], ctl_pt[i][j][1], ctl_pt[i][j][2]);
                }
            }
            glEnd();
        }

        // if (pLighter)
        //     pLighter->enable();
        // glColor3f(1, 1, 1);
        gluBeginSurface(theNurb_vec[0]);
        gluNurbsSurface(theNurb_vec[0], 8, knots, 8, knots, 4 * 3, 3, &ctl_pt[0][0][0], 4, 4,
            GL_MAP2_VERTEX_3);
        gluEndSurface(theNurb_vec[0]);
        // if (pLighter)
        //     pLighter->disable();
    }

    void drawDown(lighterObj *pLighter) {
        GLfloat ctl_pt[4][4][3] = {
            {{-2.0, -2.0, -1.0}, {-0.5, -2.0, 0.0}, {0.5, -2.0, 2.0}, {2.0, -2.0, -2.0}},
            {{-2.0, -0.5, -2.0}, {-0.5, -0.5, -1.5}, {0.5, -0.5, 0.0}, {2.0, -0.5, 2.0}},
            {{-2.0, 0.5, -2.0}, {-0.5, 0.5, -1.0}, {0.5, 0.5, 1.0}, {2.0, 0.5, -1.0}},
            {{-2.0, 2.0, 1.0}, {-0.5, 2.0, 1.0}, {0.5, 2.0, 0.0}, {2.0, 2.0, 0.5}}};
        GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
        int i, j;

        if (showPoints) {
            glPointSize(5.0);
            glColor3f(1.0, 0, 0.0);
            glBegin(GL_POINTS);
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    glVertex3f(ctl_pt[i][j][0], ctl_pt[i][j][1], ctl_pt[i][j][2]);
                }
            }
            glEnd();
        }

        if (pLighter)
            pLighter->enable();
        glColor3f(1, 0, 1);
        gluBeginSurface(theNurb_vec[1]);
        gluNurbsSurface(theNurb_vec[1], 8, knots, 8, knots, 4 * 3, 3, &ctl_pt[0][0][0], 4, 4,
            GL_MAP2_VERTEX_3);
        gluEndSurface(theNurb_vec[1]);
        if (pLighter)
            pLighter->disable();
    }

    static void nurbsError(GLenum errorCode) {
        const GLubyte *estring;
        estring = gluErrorString(errorCode);
        fprintf(stderr, "Nurbs Error: %s\n", estring);
        exit(0);
    }

  private:
    int pt_x_num, pt_y_num;
    int showPoints = 1;

    std::vector<GLUnurbsObj *> theNurb_vec;
};

/**
 * @class
 *
 */
class car {};

void gl_init(int argc, char **argv);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void idle();

#endif