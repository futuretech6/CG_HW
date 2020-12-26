#ifndef _CELESTIAL_H
#define _CELESTIAL_H

#include "SolarSys.h"

extern unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

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
    GLubyte r, g, b;

    // State variables
    float rotateAngle  = 0;
    float revolveAngle = 0;

    // Texture Param
    bool enable_tex = 0;
    GLuint tex;
    GLUquadric *sphere;
    BITMAPINFOHEADER bitmapInfoHeader;  // bitmap信息头
    unsigned char *bitmapData;          // 纹理数据

  public:
    // Functions
    celestialObj(float radius, float distance, float rotateV, float revolveV,
        float rotateTilt, float revolveTilt, int r, int g, int b,
        const char *tex_path = NULL);

    void load_tex(char *const tex_path);

    void tex_on(void);

    void tex_off(void);

    void trans(void);
    void draw();

    void rotate();
};

#endif