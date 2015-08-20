//
//  ball.cpp
//  myWhiteParty
//
//  Created by Hu Chin Hsiang on 2015/7/28.
//
//

#include "ball.h"
ball::ball(){
    sphere.set(50,10);
}
void ball::setPos(float x, float y, float z, bool r){
    targetPos.set(ofVec3f(x,y,z));
    reverseTF=r;
    int rand;
    rand=(int)ofRandom(0,2);
    if(!circle.isAllocated()){
        if(rand==0){
            circle.loadImage("Green.png");
            color="green";
        }else{
            circle.loadImage("Yellow.png");
            color="yellow";
        }
    }
    speed = ofRandom(.9, .95);
    speed2 = ofRandom(.995, .9995);
    rotateDeg =ofRandom(-3, 3);
}
void ball::update(){
   
    if (reverseTF) {
    
    currentPosX=sphere.getPosition().x*speed+targetPos.x*(1-speed);
    currentPosY=sphere.getPosition().y*speed+targetPos.y*(1-speed);
    currentPosZ=sphere.getPosition().z*speed+targetPos.z*(1-speed);
        
    }else{
        
    currentPosX=sphere.getPosition().x*speed2+targetPos.x*(1-speed2);
    currentPosY=sphere.getPosition().y*speed2+targetPos.y*(1-speed2);
    currentPosZ=sphere.getPosition().z*speed2+targetPos.z*(1-speed2);
        if (currentPosZ>-5000) {
            speed2-=0.0009;
        }
        if (currentPosZ<-5000 && currentPosZ>-30000) {
            speed2-=0.02;
            
        }
        
    }
    sphere.setPosition(ofVec3f(currentPosX, currentPosY, currentPosZ));
    sphere.rotateAround(rotateDeg, ofVec3f(targetPos.x,targetPos.y,-30000), ofVec3f(targetPos));
    
    
    alpha = ofMap(currentPosZ, -10000, -1000, 128, 255);
    
}
void ball::draw(){
   // sphere.drawAxes(20);
    //sphere.draw();
    
    ofSetColor(255, 255, 255, alpha);
    circle.draw(currentPosX-circle.width/2, currentPosY-circle.height/2+100, currentPosZ,365,200.75f);//,730,180
    ofSetColor(255);
    
}