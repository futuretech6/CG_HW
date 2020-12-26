#include "SolarSys.h"
#include <cmath>
#include <cstdio>

// void draw_cylinder(GLfloat radius, GLfloat height, GLubyte R, GLubyte G, GLubyte B) {
//     GLfloat x                    = 0.0;
//     GLfloat y                    = 0.0;
//     const GLfloat angle_stepsize = 0.1;

//     /** Draw the tube */
//     glColor3ub(R, G, B);
//     glBegin(GL_QUAD_STRIP);
//     for (GLfloat angle = 0.0; angle <= 2 * PI + angle_stepsize; angle += angle_stepsize) {
//         x = radius * cos(angle);
//         y = radius * sin(angle);
//         glVertex3f(x, y, 0.5 * height);
//         glVertex3f(x, y, -0.5 * height);
//     }
//     glEnd();

//     /** Draw the circle on top of cylinder */
//     glColor3ub(R, G, B);
//     glBegin(GL_POLYGON);
//     for (GLfloat angle = 0.0; angle <= 2 * PI; angle += angle_stepsize) {
//         x = radius * cos(angle);
//         y = radius * sin(angle);
//         glVertex3f(x, y, 0.5 * height);
//     }
//     glEnd();

//     /** Draw the circle on bottom of cylinder */
//     glColor3ub(R, G, B);
//     glBegin(GL_POLYGON);
//     for (GLfloat angle = 2 * PI; angle >= 0; angle -= angle_stepsize) {
//         x = radius * cos(angle);
//         y = radius * sin(angle);
//         glVertex3f(x, y, -0.5 * height);
//     }
//     glEnd();
// }

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader) {
    FILE *filePtr;                      // 文件指针
    BITMAPFILEHEADER bitmapFileHeader;  // bitmap文件头
    unsigned char *bitmapImage;         // bitmap图像数据

    fopen_s(&filePtr, filename, "rb");  // 以“二进制+读”模式打开文件filename
    if (filePtr == NULL)
        return NULL;
    // 读入bitmap文件图
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    // 验证是否为bitmap文件
    // if (bitmapFileHeader.bfType != BITMAP_ID) {
    //     fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
    //     return NULL;
    // }

    // 读入bitmap信息头
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    bitmapInfoHeader->biSizeImage =
        bitmapInfoHeader->biWidth * bitmapInfoHeader->biHeight * 3;
    // 将文件指针移至bitmap数据
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    // 为装载图像数据创建足够的内存
    bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
    // 验证内存是否创建成功
    if (!bitmapImage) {
        fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
        return NULL;
    }

    // 读入bitmap图像数据
    fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
    // 确认读入成功
    if (bitmapImage == NULL) {
        fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
        return NULL;
    }

    //由于bitmap中保存的格式是BGR，下面交换R和B的值，得到RGB格式
    for (int imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
        unsigned char tempRGB     = bitmapImage[imageIdx];
        bitmapImage[imageIdx]     = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }
    // 关闭bitmap图像文件
    fclose(filePtr);
    return bitmapImage;
}