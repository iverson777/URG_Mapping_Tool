#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
//ball
    balls.assign(20	, ball());
    setBallPos(false);
    //cam.reset();
    cam.setFarClip(30000);
    
    
    
//urg
    urg.setMode(ofxUrg::DISTANCE_INTENSITY);
    urg.setupEthernet();
    urg.start();
    scale=.09;
    
//cv
    grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
    fbo.allocate(320, 240,GL_RGB);
	bLearnBakground = true;
	threshold = 80;
    
    screenSize.width=960;//ofGetWidth();
    screenSize.height=1200;//ofGetHeight();
    screenSize.x=0;
    screenSize.y=0;
    warpPoint.push_back(ofPoint(70,6));//133,33
    warpPoint.push_back(ofPoint(154,4));//230,32
    warpPoint.push_back(ofPoint(166,203));//226,208
    warpPoint.push_back(ofPoint(60,206));//128,201
   
//touch
    clickSwitch=true;
    countTime=.5f;
    startTime=0;
    canTouchTF=false;

//system
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    drawDebug=true;
    ofSetWindowShape(960, 1200);
    ofSetWindowPosition(200, 0);
    //ofSetWindowPosition(-1920, 0);
    needScale=false;
    
//GUI
    setupGui();
}
void testApp::setupGui(){
    gui.setup("settings");
    //system
    
    gui.add(guiFPS.set("FPS",0,0,60));
    gui.add(guiDoFullScreen.set("fullscreen(f)",false));
    guiDoFullScreen.addListener(this, &testApp::guiSetFullscreen);
    
//    gui.add(guiWindowShape.setup("winShape", ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()), ofVec2f(960, 1200), ofVec2f(1920, 2400)));
//    gui.add(guiWindowShape.set("winShape", ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()), ofVec2f(960, 1200), ofVec2f(1920, 2400)));
    
    gui.add(guiWindowShapeW.set("winShapeW",ofGetWindowWidth(),960,1920));
    gui.add(guiWindowShapeH.set("winShapeH",ofGetWindowHeight(),0,2400));
    guiWindowShapeW.addListener(this, &testApp::guiSetWinShapeW);
    guiWindowShapeH.addListener(this, &testApp::guiSetWinShapeH);
    
//    gui.add(guiWindowPos.setup("winPos", ofVec2f(ofGetWindowPositionX(), ofGetWindowPositionY()), ofVec2f(0 , 0), ofVec2f(400, 400)));
//    gui.add(guiWindowPos.set("winPos", ofVec2f(ofGetWindowPositionX(), ofGetWindowPositionY()), ofVec2f(0, 0), ofVec2f(400, 400)));
    
    gui.add(guiWinPosX.set("winPosX",ofGetWindowPositionX(),0,400));
    gui.add(guiWinPosY.set("winPosY",ofGetWindowPositionY(),0,400));
    guiWinPosX.addListener(this, &testApp::guiSetWinPosX);
    guiWinPosY.addListener(this, &testApp::guiSetWinPosY);

    
    gui.add(guiDoShow.set("show GUI(d)",true));
    guiDoShow.addListener(this, &testApp::guiSetShow);
    
    gui.add(guiDoSave.setup("save GUI"));
    guiDoSave.addListener(this, &testApp::saveGUI);
    
    gui.add(guiDoReset.setup("reset GUI"));
    guiDoReset.addListener(this, &testApp::guiReset);
    
    //cv
    gui.add(guiLearnBg.set("Learn Bg",true));
    guiLearnBg.addListener(this, &testApp::guiLBg);
    
    gui.add(guiCountTime.set("Count Time",countTime,0,1));
    
    gui.add(guiWarpPoint0.set("Warp Point0",ofVec2f(warpPoint[0].x,warpPoint[0].y),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    gui.add(guiWarpPoint1.set("Warp Point1",ofVec2f(warpPoint[1].x,warpPoint[1].y),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    gui.add(guiWarpPoint2.set("Warp Point2",ofVec2f(warpPoint[2].x,warpPoint[2].y),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    gui.add(guiWarpPoint3.set("Warp Point3",ofVec2f(warpPoint[3].x,warpPoint[3].y),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    
    //video
    gui.add(guiVideoScale.setup("VideoScale", ofVec2f(VideoXscale, VideoYscale), ofVec2f(0, 0), ofVec2f(2, 2)));
    gui.add(guiCurrentVideo.set("CurrentVideo", ""));
    
    gui.add(guiVideoNum0.set("Video0", ""));
    gui.add(guiVideoNum1.set("Video1", ""));
    gui.add(guiVideoNum2.set("Video2", ""));
    gui.add(guiVideoNum3.set("Video3", ""));
    gui.add(guiVideoNum4.set("Video4", ""));
    gui.add(guiVideoNum5.set("Video5", ""));
    
    //ball
    gui.add(guiBallExpPos.setup("BallExpPos",ofVec2f(0, 0),ofVec2f(-1000, -1000),ofVec2f(1000, 1000)));
    gui.add(guiBallRevPos.setup("BallRevPos", ofVec2f(posX, posY), ofVec2f(-10000, -10000),ofVec2f(10000, 10000)));
    
    gui.loadFromFile("set.xml");
    
    //URG
    gui.add(guiURGScale.setup("URGScale", ofVec2f(scale, scale), ofVec2f(0, 0), ofVec2f(1, 1)));
    gui.add(guiURGPos.setup("URGPos", ofVec2f(-1000, 0), ofVec2f(-1500, -1500), ofVec2f(1500, 1500)));

}

//--------------------------------------------------------------
void testApp::update(){
//video
    //cout<<ofToString(goChangeTo)<<endl;

    //ball
    for(int i=0;i<balls.size();i++){
        balls[i].update();
    }
    cam.setPosition(ofVec3f(cam.getPosition().x+sin(ofGetElapsedTimef())*.9,cam.getPosition().y+cos(ofGetElapsedTimef())*.9,cam.getPosition().z));
    
//urg
    urg.update();
    
//cv
    if (bLearnBakground == true){
        grayBg = grayImage;
        bLearnBakground = false;
    }
    fbo.begin();
    ofClear(0);
    ofPushMatrix();
    ofScale(guiURGScale->x, guiURGScale->y);
    ofRotateZ(90);
    ofRotateX(180);
    ofTranslate(screenSize.width+guiURGPos->x, screenSize.height+guiURGPos->y);
    urg.drawDebugPolar();
    ofPopMatrix();
    fbo.end();
    
    ofPixels pixels;
    pixels.allocate(320, 240, OF_IMAGE_COLOR);
    fbo.readToPixels(pixels);
    colorImg.setFromPixels(pixels);
    
    if(warpPoint.size()==4){
        colorImg.warpPerspective(warpPoint[0], warpPoint[1], warpPoint[2], warpPoint[3]);
        cout<<warpPoint[0]<<endl;
    }
    
    grayImage=colorImg;
    grayDiff.absDiff(grayBg, grayImage);
    grayDiff.threshold(threshold);
    grayDiff.erode();
    grayDiff.dilate();
    contourFinder.findContours(grayDiff, 20, (320*240)/3, 10, true);
    
//touch
    //touch pos
    hitPoint.clear();
    for(i=0;i<contourFinder.nBlobs;i++){
        float x,y,xx;
        if(contourFinder.blobs[i].centroid.x<320/2-10){
            x=contourFinder.blobs[i].boundingRect.x+contourFinder.blobs[i].boundingRect.width;
        }else if(contourFinder.blobs[i].centroid.x>320/2+10){
            x=contourFinder.blobs[i].boundingRect.x;
        }else{
            xx=ofMap(contourFinder.blobs[i].centroid.x, 320/2-10, 320/2+10, 10, -10);
            x=contourFinder.blobs[i].centroid.x;
            
        }
        y=contourFinder.blobs[i].boundingRect.y;
        hitPoint.push_back(ofPoint(x,y));
    }
    
    if(contourFinder.nBlobs==0){
        finalHitPoint.clear();
        clickSwitch=true;
        startTime=0;
    }else{
        if(startTime==0){
            startTime=ofGetElapsedTimef();
        }
        if(ofGetElapsedTimef()-startTime>countTime){
            if(clickSwitch){
                finalHitPoint.clear();
                for (int i = 0; i < contourFinder.nBlobs; i++){
                    finalHitPoint.push_back(ofPoint(ofMap(hitPoint[i].x,0,320,0+screenSize.x,screenSize.width),ofMap(hitPoint[i].y,0,240,0+screenSize.y,screenSize.height*1.6)));
                }
                
            }
            clickSwitch=false;
            
        }
    }
    
    //touch timing
    int targetPos1,targetPos2;
    string hitColor1,hitColor2;
 //   if(currentVid!=0 && goChangeTo==-1){
        if(finalHitPoint.size()>=2){
            targetPos1=getClosestBall(finalHitPoint[0]);
            targetPos2=getClosestBall(finalHitPoint[1]);
            if(targetPos1!=-1){
                balls[targetPos1].targetPos.z=balls[targetPos1].targetPos.z+200;
                hitColor1=balls[targetPos1].color;
            }
            if(targetPos2!=-1){
                balls[targetPos2].targetPos.z=balls[targetPos2].targetPos.z+200;
                hitColor2=balls[targetPos2].color;
            }
            if(targetPos1!=-1 && targetPos2!=-1 && hitColor1!=hitColor2){
//                if(bingo[0] && bingo[1]){
//                    clickButton(5,2);
//                    
//                }
            }else{
                if(targetPos1!=-1){
                    if(balls[targetPos1].color=="green"){
                        clickButton(4,1);
                    }else{
                        clickButton(3,0);
                    }
                }else if(targetPos2!=-1){
                    if(balls[targetPos2].color=="green"){
                        clickButton(4,1);
                    }else{
                        clickButton(3,0);
                        
                    }
                }
            }
        }else if(finalHitPoint.size()==1){
            targetPos1=getClosestBall(finalHitPoint[0]);
            if(targetPos1!=-1){
                balls[targetPos1].targetPos.z=balls[targetPos1].targetPos.z+200;
                if(balls[targetPos1].color=="green"){
                    clickButton(4,1);
                }
                else {
                    clickButton(3,0);
                }
            }
        }
        
    else{
//        if(finalHitPoint.size()>0 && currentVid==0){
//            touchAlpha=100;
//            goChangeTo=1;
//        }
    }

}

//--------------------------------------------------------------
void testApp::draw(){
//video
    ofPushMatrix();
    if(needScale){
        ofScale(VideoXscale,VideoYscale);
    }
//    ofSetColor(255, 255, 255,touchAlpha);
//    videos[currentVid].draw(0,0);
    ofSetColor(255);
    ofPopMatrix();
//ball
    cam.begin();
    for(i=0;i<balls.size();i++){
        balls[i].draw();
    }
    cam.end();
    for(i=0;i<balls.size();i++){
        worldToScreen=cam.worldToScreen(balls[i].targetPos);
        diff=abs(balls[i].currentPosZ-balls[i].targetPos.z);
        if(diff < 2 && isExplode && !canTouchTF  && balls[i].currentPosZ>-3000){
            canTouchTF=true;
            cout<<"switch on" <<endl;
        }

        if(drawDebug){
            ofDrawBitmapStringHighlight(ofToString(i), worldToScreen);
        }
    }
    
//cv
    if(drawDebug){
        colorImg.draw(0,0);
        grayImage.draw(360,0);
        grayBg.draw(0,280);
        grayDiff.draw(360,280);
    
        ofPoint targetPos;
        for (i = 0; i < contourFinder.nBlobs; i++){
            contourFinder.blobs[i].draw(360,280);
            ofSetColor(255,255,0);
            ofFill();
            //targetPos.x=ofMap(hitPoint[i].x,0,320,0+screenSize.x,screenSize.width);
            //targetPos.y=ofMap(hitPoint[i].y,0,240,0+screenSize.y,screenSize.height);
            //ofCircle(targetPos, 20);
            
            ofSetColor(255);
        }
    }
    
//touch
    
    if(drawDebug){
        ofSetColor(255, 0,0);
        ofLine(320/2,0,320/2,240);
        ofSetColor(255);
    
//        if(touchAlpha>0){
//            touchAlpha-=20;
//        }
//        ofSetColor(255,255,0,touchAlpha);
        ofFill();
        for(i=0;i<finalHitPoint.size();i++){
            ofCircle(finalHitPoint[i], 20);
        }
        
    }
    ofSetColor(255);
    
//GUI
    if(drawDebug){
        ofShowCursor();
        guiFPS=ofGetFrameRate();
        guiDoShow = drawDebug;
        guiLearnBg = bLearnBakground;
        countTime = guiCountTime;
        warpPoint[0] = ofVec2f(guiWarpPoint0);
        warpPoint[1] = ofVec2f(guiWarpPoint1);
        warpPoint[2] = ofVec2f(guiWarpPoint2);
        warpPoint[3] = ofVec2f(guiWarpPoint3);
        VideoXscale = guiVideoScale->x;
        VideoYscale = guiVideoScale->y;
//        guiCurrentVideo = ofToString(currentVid);
//        guiVideoNum0 = ofToString(videos[0].getPosition());
//        guiVideoNum1 = ofToString(videos[1].getPosition());
//        guiVideoNum2 = ofToString(videos[2].getPosition());
//        guiVideoNum3 = ofToString(videos[3].getPosition());
//        guiVideoNum4 = ofToString(videos[4].getPosition());
//        guiVideoNum5 = ofToString(videos[5].getPosition());
        gui.draw();
    }

    
}

void testApp::clickButton(int goWhere, int bingoWho){
    if(goChangeTo==-1 && canTouchTF){
        bingo[bingoWho]=true;
        goChangeTo=goWhere;
        cout<<ofToString(goWhere)<<endl;
        canTouchTF=false;
        cout<<"switch off"<<endl;
    }
}

bool comparePosz(const ball  &a ,const ball  &b){
    return a.targetPos.z < b.targetPos.z;
    
}

void testApp::setBallPos(bool isEx){
    
    for(i=0;i<balls.size();i++){
        
        bool r;
        
        if(isEx){
            posX=ofRandom(0-ofGetWidth()/2+guiBallExpPos->x, ofGetWidth()/2+guiBallExpPos->x);
            posY=ofRandom(0-ofGetHeight()/2+guiBallExpPos->y, ofGetHeight()/2+guiBallExpPos->y);
            posZ=ofRandom(-3000, -1000);
            r=true;
        }else{
            //posX=0;
            //posY=4500;
            posX=guiBallRevPos->x;
            posY=guiBallRevPos->y;
            posZ=-30000;
            r=false;
        }
        
        balls[i].setPos(posX, posY, posZ, r);
    }
    isExplode=isEx;
    ofSort(balls,comparePosz);
}

int testApp::getClosestBall(ofPoint clickPos){
    ofPoint ballPos;
    float distance=1000.f;
    float tempDis;
    int targetBall=-1;
    for(i=0;i<balls.size();i++){
        ballPos=cam.worldToScreen(balls[i].targetPos);
        tempDis=clickPos.distance(ballPos);
        if(distance>tempDis){
            distance=tempDis;
            targetBall=i;
        }
    }
    if(distance<100){
        
        return targetBall;
    }else{
        return -1;
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key){
        case '1':
            goChangeTo=1;
            touchAlpha=100;
            break;
        case '2':
            clickButton(3,0);
            break;
        case '3':
            clickButton(4,1);
            break;
        case '4':
            if(bingo[0] && bingo[1]){
                clickButton(5,2);
            }
            break;
        case 'b':
            setBallPos(!isExplode);
            break;
        case 'd':
            drawDebug=!drawDebug;
            break;
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
        case 'r':
            warpPoint.clear();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        
	}
    if (key=='s') {
        
        gui.saveToFile("set.xml");
        
    }
    if (key=='r') {
        
        gui.loadFromFile("reset.xml");
        
    }
    
}

void testApp::saveGUI(){
    gui.saveToFile("set.xml");
}
void testApp::guiReset(){
    gui.loadFromFile("reset.xml");

}
void testApp::guiSetWinShapeW(float &_w){
    ofSetWindowShape(_w,guiWindowShapeH);
}
void testApp::guiSetWinShapeH(float &_h){
    ofSetWindowShape(guiWindowShapeW,_h);
}

void testApp::guiSetWinPosX(float &_x){
    ofSetWindowPosition(_x,guiWinPosY);
}
void testApp::guiSetWinPosY(float &_y){
    ofSetWindowPosition(guiWinPosX,_y);
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(warpPoint.size()<4){
        warpPoint.push_back(ofPoint(mouseX,mouseY));
        cout<<ofToString(ofToString(warpPoint.size())+","+ofToString(mouseX)+","+ofToString(mouseY))<<endl;
    }else{
        int targetBall;
        targetBall=getClosestBall(ofPoint(mouseX,mouseY));
        if(targetBall!=-1){
            balls[targetBall].targetPos.z=balls[targetBall].targetPos.z+200;
            if(balls[targetBall].color=="green"){
                clickButton(4,1);

            }else{
                clickButton(3,0);
                
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}