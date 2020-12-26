#ifndef _CAR_H
#define _CAR_H

#include "SolarSys.h"
#include "celestial.h"
#include <vector>

/**
 * @class
 *
 */
class carObj : public celestialObj {
  private:
    std::vector<GLUnurbsObj *> theNurb_vec;

  public:
    carObj();
    ~carObj();

    void initNurbs(int surface_num);
    static void nurbsError(GLenum errorCode);
    void drawUp(void);
    void drawDown(void);
    void drawTires(void);
    void drawSpoiler(void);
    void drawCockpit(void);
    void draw(void);
};

#endif