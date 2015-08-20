//
//  ball.h
//  myWhiteParty
//
//  Created by Hu Chin Hsiang on 2015/7/28.
//
//

#include "ofMain.h"

class ball {
public:
    ball();
    void update();
    void draw();
    void setPos(float x,float y, float z, bool r);
    ofVec3f             targetPos;
    ofSpherePrimitive   sphere;
    int               currentPosX;
    int               currentPosY;
    int               currentPosZ;
    float             speed,speed2;
    float             alpha;
    ofImage           circle;
    bool              reverseTF;
    int               rotateDeg;
    string            color;
};

