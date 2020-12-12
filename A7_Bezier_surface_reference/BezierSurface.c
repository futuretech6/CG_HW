#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * An application that can be used to shade cubic Bezier curves.
 *
 * Mouse Commands:
 *
 *   Left-Click:            To add a point
 *   Right-Click-and-Drag:  To select a point and move it
 *
 *
 * On MS-Windows with MinGW, compile with:
 *   g++ -o shade -Wall shade.c -lglut32 -lGL -lGLU
 *
 * @author  Prof. David Bernstein, James Madison University
 */
static float min[] = {-4.0, -4.0, -4.0};
static float max[] = {4.0, 4.0, 4.0};
static float orthoMax[3], orthoMin[3];
static int windowWidth, windowHeight;

/*
 * A fourth order surface
 */
GLfloat points[4][4][3] = {
    {{-2.0, -2.0, 1.0}, {-0.5, -2.0, 0.0}, {0.5, -2.0, -2.0}, {2.0, -2.0, 2.0}},
    {{-2.0, -0.5, 2.0}, {-0.5, -0.5, 1.5}, {0.5, -0.5, 0.0}, {2.0, -0.5, -2.0}},
    {{-2.0, 0.5, 2.0}, {-0.5, 0.5, 1.0}, {0.5, 0.5, -1.0}, {2.0, 0.5, 1.0}},
    {{-2.0, 2.0, -1.0}, {-0.5, 2.0, -1.0}, {0.5, 2.0, 0.0}, {2.0, 2.0, -0.5}}};
int uSize = 4;
int vSize = 4;

// The number of subdivisions in the grid
int gridSize  = 200;
int uSelected = -1;
int vSelected = -1;

/**
 * Return the squared Euclidean sistance between two points
 */
GLfloat distanceSquared(
    GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2) {
    GLfloat d;

    d = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);

    return d;
}

/**
 * Find the closest point controlling the curve
 *
 * @param x   The x-coordinate to compare to
 * @param y   The y-coordinate to compare to
 * @param z   The z-coordinate to compare to
 * @param ui  The u-index of the clossest point (OUT)
 * @param vi  The v-index of the clossest point (OUR)
 */
void closestPointTo(GLfloat x, GLfloat y, GLfloat z, int &ui, int &vi) {
    GLfloat d, min;

    ui = -1;
    vi = -1;

    min = distanceSquared(points[0][0][0], points[0][0][1], points[0][0][2], x, y, z);
    ui  = 0;
    vi  = 0;

    for (int u = 0; u < uSize; u++) {
        for (int v = 0; v < vSize; v++) {

            d = distanceSquared(points[u][v][0], points[u][v][1], points[u][v][2], x, y, z);

            if (d < min) {
                ui  = u;
                vi  = v;
                min = d;
            }
        }
    }
}

/**
 * Convert from screen coordinates (in pixels) to model
 * coordinates
 *
 * @param x   The x-coordinate in pixels (IN)
 * @param y   The y-coordinate in pixels (IN)
 * @param fx  The converted x coordinate (OUT)
 * @param fy  The converted y coordinate (OUT)
 */
void screen2ortho(int x, int y, GLfloat &fx, GLfloat &fy) {
    fx = orthoMin[2] + (float)x / (float)windowWidth * (orthoMax[2] - orthoMin[2]);

    fy = orthoMin[1] +
         (float)(windowHeight - y) / (float)windowHeight * (orthoMax[1] - orthoMin[1]);
}

/**
 * The display callback
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make the Model-View matrix active
    glMatrixMode(GL_MODELVIEW);
    // Initialize the Model-View matrix
    glLoadIdentity();

    // Set the color
    glColor3f(1.0, 0.0, 0.0);
    // Set the point size
    glPointSize(3.);

    //[surface.
    //
    // The evaluator
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, uSize, 0, 1, uSize * 3, vSize, &points[0][0][0]);

    // Fill the surface
    glEvalMesh2(GL_FILL, 0, gridSize, 0, gridSize);

    //]surface.

    //[points.
    // Show the points
    glPointSize(3.);
    glBegin(GL_POINTS);
    {
        for (int u = 0; u < uSize; u++) {
            for (int v = 0; v < vSize; v++) {
                glVertex3fv(&points[u][v][0]);
            }
        }
        glEnd();
    }
    //]points.

    // Force the rendering (off-screen)
    glFlush();
    // Handle the double buffering
    glutSwapBuffers();
}

//[motion.
/**
 * The mouse drag callback
 *
 * @param x      The x-position of the mouse
 * @param y      The y-position of the mouse
 */
void motion(int x, int y) {
    GLfloat fx, fy;

    screen2ortho(x, y, fx, fy);

    if ((uSelected >= 0) && (vSelected >= 0)) {
        points[uSelected][vSelected][0] = fx;
        points[uSelected][vSelected][1] = fy;

        glutPostRedisplay();
    }
}
//]motion.

//[mouse.
/**
 * The mouse callback (i.e., the function that is called
 * each time a mouse button is pressed or released).
 *
 * @param button The button (e.g., GLUT_LEFT_BUTTON)
 * @param state  The state (e.g., GLUT_UP or GLUT_DOWN)
 * @param x      The x-position of the mouse
 * @param y      The y-position of the mouse
 */
void mouse(int button, int state, int x, int y) {
    GLfloat fx, fy;

    screen2ortho(x, y, fx, fy);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            closestPointTo(fx, fy, 0.0, uSelected, vSelected);
        } else {
            uSelected = -1;
            vSelected = -1;
        }

        glutPostRedisplay();
    }
}
//]mouse.

/**
 * The reshape callback (i.e., the function that is called
 * each time the window is re-sized).
 *
 * @param width   The new width
 * @param height  The new height
 */
void reshape(int width, int height) {
    float max3D, min3D;
    GLfloat aspect;

    windowWidth  = width;
    windowHeight = height;

    // Set the viewport
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Make the projection matrix current
    glMatrixMode(GL_PROJECTION);

    // Clear the projection matrix
    glLoadIdentity();

    // Find the largest and smallest values for all coordinates
    min3D = min[0];
    max3D = max[0];
    for (int i = 1; i < 3; i++) {
        if (min[i] < min3D)
            min3D = min[i];
        if (max[i] > max3D)
            max3D = max[i];
    }
    if (fabs(min3D) > max3D)
        max3D = fabs(min3D);
    min3D = -max3D;

    // Set the projection matrix (based on the aspect ratio)
    if (width <= height) {
        aspect      = (GLfloat)height / (GLfloat)width;
        orthoMin[0] = min3D;
        orthoMax[0] = max3D;
        orthoMin[1] = min3D * aspect;
        orthoMax[1] = max3D * aspect;
        orthoMin[2] = min3D;
        orthoMax[2] = max3D;
    } else {
        aspect      = (GLfloat)width / (GLfloat)height;
        orthoMin[0] = min3D * aspect;
        orthoMax[0] = max3D * aspect;
        orthoMin[1] = min3D;
        orthoMax[1] = max3D;
        orthoMin[2] = min3D;
        orthoMax[2] = max3D;
    }
    glOrtho(orthoMin[0], orthoMax[0], orthoMin[1], orthoMax[1], orthoMin[2], orthoMax[2]);

    // Make the Model-View matrix active
    glMatrixMode(GL_MODELVIEW);
}

/**
 * The entry point of the application.
 *
 * @param argc  The number of command line arguments
 * @param argv  The array of command line arguments
 * @return      A status code
 */
int main(int argc, char **argv) {
    windowWidth  = 500;
    windowHeight = 500;

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Surface Editor");

    // Callbacks
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);

    // Enable lighting and shading
    GLfloat ambient[]   = {0.2, 0.2, 0.2, 1.0};
    GLfloat position[]  = {0.0, 0.0, 2.0, 1.0};
    GLfloat diffuse[]   = {0.7, 0.0, 0.1, 1.0};
    GLfloat specular[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {50.0};

    glEnable(GL_LIGHTING);  // Enable lighting
    glEnable(GL_LIGHT0);    // Enable light 0

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    //[setup.
    // Enable the 2-dimensional evaluator
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(gridSize, 0.0, 1.0, gridSize, 0.0, 1.0);
    //]setup.

    // Start the event loop
    glutMainLoop();
}
