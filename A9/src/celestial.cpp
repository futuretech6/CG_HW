#include "celestial.h"
#include <cmath>
#include <cstdio>

celestialObj::celestialObj(float radius, float distance, float rotateV, float revolveV,
    float rotateTilt, float revolveTilt, int r, int g, int b, const char *tex_path)
    : radius(radius),
      distance(distance),
      rotateV(rotateV / fluentRatio),
      revolveV(revolveV / fluentRatio),
      rotateTilt(rotateTilt),
      revolveTilt(revolveTilt),
      r(r),
      g(g),
      b(b),
      sphere(gluNewQuadric()) {
    if (tex_path)
        load_tex(const_cast<char *>(tex_path));
}

void celestialObj::load_tex(char *const tex_path) {
    bitmapData = LoadBitmapFile(tex_path, &bitmapInfoHeader);
}

void celestialObj::tex_on(void) {
    enable_tex = 1;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    // 指定当前纹理的放大/缩小过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D,
        0,                          // mipmap层次(通常为，表示最上层)
        GL_RGB,                     //我们希望该纹理有红、绿、蓝数据
        bitmapInfoHeader.biWidth,   //纹理宽带，必须是n，若有边框+2
        bitmapInfoHeader.biHeight,  //纹理高度，必须是n，若有边框+2
        0,                          //边框(0=无边框, 1=有边框)
        GL_RGB,                     // bitmap数据的格式
        GL_UNSIGNED_BYTE,           //每个颜色数据的类型
        bitmapData);                // bitmap数据指针

    glEnable(GL_TEXTURE_2D);
}

void celestialObj::tex_off(void) {
    enable_tex = 0;
    glDisable(GL_TEXTURE_2D);
}

void celestialObj::trans(void) {
    using namespace std;
    glTranslatef(distance * cos(rotateTilt * D2R) * cos(rotateAngle * D2R),
        distance * cos(rotateTilt * D2R) * sin(rotateAngle * D2R),
        distance * sin(rotateTilt * D2R) * cos(rotateAngle * D2R));
    glRotatef(revolveAngle, -sin(revolveTilt * D2R), 0., cos(revolveTilt * D2R));
    glRotatef(revolveTilt, 0., -1., 0.);  // First rotate to tile angle
}
void celestialObj::draw() {
    glMatrixMode(GL_MODELVIEW);
    trans();
    glColor3f(r / 255., g / 255., b / 255.);

    // #ifdef USING_WIRE
    //         glutWireSphere(radius, spehereLineRatio, spehereLineRatio);
    // #else
    //         glutSolidSphere(radius, spehereLineRatio, spehereLineRatio);
    // #endif
    gluQuadricDrawStyle(sphere, GLU_FILL);
    // if (enable_tex)
    glBindTexture(GL_TEXTURE_2D, tex);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, radius, spehereLineRatio, spehereLineRatio);
}

void celestialObj::rotate() {
    rotateAngle += rotateV;
    rotateAngle = (rotateAngle > 360) ? rotateAngle - 360 : rotateAngle;
    revolveAngle += revolveV;
    revolveAngle = (revolveAngle > 360) ? revolveAngle - 360 : revolveAngle;
}