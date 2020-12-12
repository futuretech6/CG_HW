#ifndef _myModel_H_
#define _myModel_H_

#ifndef OS_LINUX
#include "gl/glut.h"
#include <Windows.h>
#pragma execution_character_set("utf-8")
#endif
#include "glm/mat4x4.hpp"
#include "objshape.h"
#include "readmtl.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <map>
#include <string>
#include <utility>
using namespace std;

class myModel {
  public:
    myModel();
    void Draw();
    void DrawLines();
    void DrawPoints();
    void DrawMTL();
    void Rebuild();
    glm::vec3 GetVertexReal(int id);
    glm::vec3 &GetVertex(int id);
    glm::vec3 GetVNReal(int id);
    glm::vec3 &GetVN(int id);
    glm::vec2 &GetVT(int id);
    void SetVertexPos(int id, double x, double y, double z);
    void MatMapVertices();
    void Save(const string filename);
    void BindTexture();
    void BindTexture(string);

  public:
    int id;
    string name;
    string tex_name;
    string path;
    bool viewed;
    bool texed;
    std::vector<objPoly> ps;    // 多边形集合
    std::vector<glm::vec3> vs;  // 顶点集合
    std::vector<glm::vec3> vn;  // 法线集合
    std::vector<glm::vec2> vt;  // 贴图顶点集合
    map<string, MTL> mtls;
    glm::mat4 mat;  // 内部矩阵
    int triangleNum;
    int rectNum;

  private:
    static int myModel_ID;
    static map<string, int> STR2TEX;

  private:
    void DrawObjPoint(const objPoint &p);

  public:
    void BindMTL(string);
};

#endif
