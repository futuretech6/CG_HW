#pragma once
#ifndef _READMTL_H_
#define _READMTL_H_
#include "glm/vec3.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct MTL {
    string map_Ka;
    float Ka[4], Kd[4], Ks[4];
};

void read_mtl(string filename, map<string, MTL> &mtls);

#endif
