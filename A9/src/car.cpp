// #include "car.h"
// #include <cstdio>

// carObj::carObj() : celestialObj(0, 6, 0, 2, 0, 0, 0, 0, 0) {}
// carObj::~carObj() {}

// void carObj::initNurbs(int surface_num) {
//     theNurb_vec.resize(surface_num);
//     for (auto &theNurb : theNurb_vec) {
//         theNurb = gluNewNurbsRenderer();
//         gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
//         gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
//         gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid(*)())(this->nurbsError));
//     }
// }

// void carObj::nurbsError(GLenum errorCode) {
//     const GLubyte *estring;
//     estring = gluErrorString(errorCode);
//     fprintf(stderr, "Nurbs Error: %s\n", estring);
//     exit(0);
// }

// void carObj::drawUp(void) {
//     GLfloat ctl_pt[4][4][3] = {{{1, 3, 0}, {1, 3, 0}, {3, 3, 0}, {8, 2.5, 0}},
//         {{0, 2, 0}, {1, 3, 2}, {7, 3, 3}, {9, 2, 0}},
//         {{0, 1, 0}, {1, 0, 2}, {7, 0, 3}, {9, 1, 0}},
//         {{1, 0, 0}, {1, 0, 0}, {3, 0, 0}, {8, 0.5, 0}}};
//     GLfloat knots[8]        = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
//     gluBeginSurface(theNurb_vec[0]);
//     gluNurbsSurface(theNurb_vec[0], 8, knots, 8, knots, 4 * 3, 3, &ctl_pt[0][0][0], 4, 4,
//         GL_MAP2_VERTEX_3);
//     gluEndSurface(theNurb_vec[0]);
// }

// void carObj::drawDown(void) {
//     GLfloat ctl_pt[4][4][3] = {{{1, 0, 0}, {1, 0, 0}, {3, 0, 0}, {8, 0.5, 0}},
//         {{0, 1, 0}, {1, 0, -1}, {7, 0, -1.6}, {9, 1, 0}},
//         {{0, 2, 0}, {1, 3, -1}, {7, 3, -1.6}, {9, 2, 0}},
//         {{1, 3, 0}, {1, 3, 0}, {3, 3, 0}, {8, 2.5, 0}}};
//     GLfloat knots[8]        = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
//     gluBeginSurface(theNurb_vec[1]);

//     gluNurbsSurface(theNurb_vec[1], 8, knots, 8, knots, 4 * 3, 3, &ctl_pt[0][0][0], 4, 4,
//         GL_MAP2_VERTEX_3);
//     gluEndSurface(theNurb_vec[1]);
// }

// void carObj::drawTires(void) {
//     // LF
//     glPushMatrix();
//     glTranslatef(7.5, 0.4, -0.1);
//     glRotatef(90, 1, 0, 0);
//     draw_cylinder(0.7, 0.2);
//     glPopMatrix();

//     // RF
//     glPushMatrix();
//     glTranslatef(7.5, 2.6, -0.1);
//     glRotatef(90, -1, 0, 0);
//     draw_cylinder(0.7, 0.2);
//     glPopMatrix();

//     // LB
//     glPushMatrix();
//     glTranslatef(2, -0.1, 0.1);
//     glRotatef(90, 1, 0, 0);
//     draw_cylinder(0.9, 0.4);
//     glPopMatrix();

//     // RB
//     glPushMatrix();
//     glTranslatef(2, 3.1, 0.1);
//     glRotatef(90, -1, 0, 0);
//     draw_cylinder(0.9, 0.4);
//     glPopMatrix();
// }

// void carObj::drawSpoiler(void) {
//     const GLfloat fan_y[] = {0.6, 2.4};
//     for (int i = 0; i < 2; i++) {
//         glBegin(GL_POLYGON);
//         glVertex3f(2, fan_y[i], 0.5);
//         glVertex3f(1, fan_y[i], 0.5);
//         glVertex3f(0.5, fan_y[i], 2);
//         glVertex3f(1.5, fan_y[i], 2);
//         glEnd();
//     }

//     glBegin(GL_POLYGON);
//     glVertex3f(0.5, fan_y[0] - 1, 2);
//     glVertex3f(0.5, fan_y[1] + 1, 2);
//     glVertex3f(1.5, fan_y[1] + 0.3, 2);
//     glVertex3f(1.5, fan_y[0] - 0.3, 2);
//     glEnd();
// }

// void carObj::drawCockpit(void) {
//     const GLfloat pt[8][3] = {{3, 0.5, 0.5}, {6.5, 0.8, 0.5}, {6.5, 2.2, 0.5}, {3, 2.5,
//     0.5},
//         {4.5, 1, 2.}, {5.5, 1.2, 1.8}, {5.5, 1.8, 1.8}, {4.5, 2, 2.}};
//     glBegin(GL_TRIANGLE_STRIP);
//     glVertex3f(pt[0][0], pt[0][1], pt[0][2]);
//     glVertex3f(pt[4][0], pt[4][1], pt[4][2]);
//     glVertex3f(pt[1][0], pt[1][1], pt[1][2]);
//     glVertex3f(pt[5][0], pt[5][1], pt[5][2]);
//     glVertex3f(pt[2][0], pt[2][1], pt[2][2]);
//     glVertex3f(pt[6][0], pt[6][1], pt[6][2]);
//     glVertex3f(pt[3][0], pt[3][1], pt[3][2]);
//     glVertex3f(pt[7][0], pt[7][1], pt[7][2]);
//     glVertex3f(pt[0][0], pt[0][1], pt[0][2]);
//     glVertex3f(pt[4][0], pt[4][1], pt[4][2]);
//     glEnd();
//     glBegin(GL_POLYGON);

//     glVertex3f(pt[4][0], pt[4][1], pt[4][2]);
//     glVertex3f(pt[5][0], pt[5][1], pt[5][2]);
//     glVertex3f(pt[6][0], pt[6][1], pt[6][2]);
//     glVertex3f(pt[7][0], pt[7][1], pt[7][2]);
//     glEnd();
// }

// void carObj::draw(void) {
//     drawUp();
//     drawDown();
//     drawTires();
//     drawSpoiler();
//     drawCockpit();
// }