# A4

<div align='right'><font size=4><b>陈希尧</b> 3180103012</font>

[TOC]

## 功能说明

**控制**

* WSAD可以控制视角的前进后退左移右移
* HJKL可以控制视角的上下左右旋转（方向同vim）
* 鼠标点击后也可以拖移视角
* Q或ESC可以退出程序



**星体**

* 所有星体的半径、距离父星体距离、公转速度、自转速度、公转角、自转角、颜色RGB都可以定义，直接用初始化函数输入参数即可
* 需要显示的星体在`display()`函数中按父星体到子星体的顺序压入矩阵即可
* 全局参数在main.cpp前几行可以进行修改，具体的意义有注释



**编译**

Windows下使用mingw-64的g++编译，glut库使用freeglut，跨平台时编译参数"-L"需修改为freeglut的lib所在位置

已在run.bat中配置完毕编译参数，双击即可编译&运行



**demo说明**

demo中有一个太阳在中心自转，一个地球和一个木星绕着太阳转，月球和木卫二分别绕着地球和木星转，另外还有某个人造探测器绕着木卫二转（速度和角度等参数与实际情况并不一致）

注：编译时使用了静态链接，直接运行.exe即可查看效果，无需重新编译



## 原理与实现方法

### 星体运动的原理

通过以下方式将一个星体变换到其运动状态：

1. 将其沿y轴旋转revolveTilt（自转倾斜角，对于地球来说是23.5°）
2. 使其绕与z轴成revolveTilt的自转轴进行旋转
3. <a id="handleRotate"></a>位移，将其变换到三维空间中的`(distance * cos(rotateTilt) * cos(rotateAngle), distance * cos(rotateTilt) * sin(rotateAngle), distance * sin(rotateTilt) * cos(rotateAngle))`处（这相当于等周期的z方向的简谐振动和xy平面的椭圆周运动的叠加），其中rotateTilt为公转夹角参数（对地球来说即黄道角，1.5°）

依照此实现即可，需要注意的是有继承关系的天体（如月球和地球，木卫二和木星）的显示要在同一次矩阵的push和pop中间进行，且父星体的显示要于前面。

### 相机变换的原理

对于相机，我使用了球坐标下的点`(r, φ, θ) = (radicalDist, azimuthAngle, polarAngle)`来表示相机的视线方向，`(eyex, eyey, eyez)`表示相机所在位置，其中移动相机可以通过移动eyexyz的坐标来改变，移动则直接修改球坐标的两个角即可。

需要注意的是，由于`glLookAt`函数需要有centerxyz和upxyz参数，所以在显示时需要将球坐标转换到直角坐标`(rsinθcosφ, rsinθsinφ, rcosθ) = (centerx-eyex, centery-eyey, centerz-eyez)`来表示相机视线的向量

### CPP实现

为了面向对象特性，我将星体和相机分别封装为两个类，并在其中实现各种调用所需的功能。然后再用一个vector串联所有的星体，方便对他们进行遍历以进行状态的更新。

## 遇到的困难

1. 本来公转和自转都是用`glRotatef`函数，但是这样会导致自转的转轴会随着公转而旋转（然而这与事实不符），想了很久之后发现可以直接用`glTranslatef`搞定公转（具体变换的实现在<a href="#handleRotate">这里</a>），还不会有自转轴变化的问题
2. 开始时使用`Angle = ((int)Angle + (int)Velocity) % 360;`（因为浮点数无法取余运算）进行速度的更新，由于Velocity设置的比1还小，因此行星一直只自转不公转，导致我一直以为是旋转矩阵设置的问题，最后发现是这个问题了，改成`rotateAngle = ((rotateAngle + rotateV) > 360) ? rotateAngle + rotateV - 360 : rotateAngle + rotateV;`
3. 由于一开始没有理解清除opengl中对矩阵的使用的原理，我在每个星体话之前和之后都要进行矩阵的push和pop操作，但是这样子画出来的星体完全没有继承关系（如卫星和行星），后来才发现push/pop之后矩形就不会在进行点积运算了，所以有继承关系的必须在同一次push和pop里面
4. 想用鼠标控制的时候不能只写个`glutMouseFunc`，不写`glutMotionFunc`，这是没有用的
5. 深度的话除了开启深度测试，还要记得每次画图时清除`GL_DEPTH_BUFFER_BIT`

## 代码

```cpp
/**
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
 */
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <tuple>
#include <vector>

const double pi  = 3.1425927;
const double D2R = pi / 180;

/* Global Parameter Def */
#define USING_WIRE                    // Comment to use glutSolidSphere
const int spehereLineRatio   = 25;    // Control wire number of sphere
const int fluentRatio        = 2;     // The higher the value, the more fulent the animation
const double cam_move_step   = 0.25;  // Move step of camera using WSAD
const double cam_rotate_step = 1;     // Rotate step of camera using HJKL, in unit of degree
const double PRP_VRP_Dist    = 10;    // Distance from PRP to VRP
const double fovy            = 68;    // View volumn: angle of view
const double zNear           = 0.1;   // View volume: distance to near clipping plane
const double zFar            = 100;   // View volume: distance to far clipping plane

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
    void rotateUp(double step = cam_rotate_step, int flag = 1) {
        polarAngle -= flag * step;
    }

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

void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void mouseButton(int, int, int, int);
void mouseMove(int, int);
void idle();

cameraObj camera;
std::vector<celestialObj> starVec;

int main(int argc, char **argv) {
    // radius, distance, rotateV, revolveV, rotateTilt, revolveTilt, r, g, b
    starVec.emplace_back(2, 0, 0, 0.5, 0., 0., 255, 0, 0);        // sun
    starVec.emplace_back(0.4, 3, -3, -2, 10., 23.5, 0, 0, 255);   // earth
    starVec.emplace_back(0.1, 0.8, 9, 3, 0, 0., 255, 255, 255);   // moon
    starVec.emplace_back(1.1, 10, 1, 2, -45., 60., 0, 255, 0);    // jupyter
    starVec.emplace_back(0.5, 1.9, 5, 3, 80., 10., 255, 255, 0);  // europa
    starVec.emplace_back(0.2, 1, 10, 3, 20., 90., 255, 0, 255);   // something

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Apocalypse System");
    glClearColor(0., 0., 0., 0.);
    glShadeModel(GL_FLAT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
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
        case 'w': camera.moveForward(); break;
        case 's': camera.moveBackward(); break;
        case 'a': camera.moveLeft(); break;
        case 'd': camera.moveRight(); break;
        case 'h': camera.rotateLeft(); break;
        case 'j': camera.rotateDown(); break;
        case 'k': camera.rotateUp(); break;
        case 'l': camera.rotateRight(); break;
        case 'q': exit(0); break;
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
```