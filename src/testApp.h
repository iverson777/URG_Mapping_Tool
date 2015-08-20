#pragma once

#include "ofMain.h"
#include "ofxUrg.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ball.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
    int i;
    //urg
        ofxUrg::Device          urg;
        float                   scale;
    
    //cv
        ofFbo                   fbo;
        ofxCvColorImage         colorImg;
        ofxCvGrayscaleImage     grayImage;
        ofxCvGrayscaleImage 	grayBg;
        ofxCvGrayscaleImage 	grayDiff;
        
        ofxCvContourFinder      contourFinder;
        
        int                     threshold;
        bool                    bLearnBakground;
        
        vector<ofPoint>         warpPoint;
    
    //touch
        vector<ofPoint>         hitPoint;
        vector<ofPoint>         finalHitPoint;
        
        ofRectangle             screenSize;
        
        bool                    clickSwitch;
        float                   countTime;
        float                   startTime;
    
    
    //debug
        bool                    drawDebug;
    
    //scale for monitor
        bool                    needScale;
    
    //GUI
    ofxPanel                    gui;
    
    void                        setupGui();
    
        //GUI system
    ofParameter<float>          guiFPS;
    ofParameter<bool>           guiDoFullScreen;
    void    guiSetFullscreen(bool &_value){ofSetFullscreen(_value);};
    
    ofParameter<float>        guiWindowShapeW;
    ofParameter<float>        guiWindowShapeH;
    void    guiSetWinShapeW(float &_w);
    void    guiSetWinShapeH(float &_h);
    
    ofParameter<float>        guiWinPosX;
    ofParameter<float>        guiWinPosY;
    void    guiSetWinPosX(float &_x);
    void    guiSetWinPosY(float &_y);
    
    ofParameter<bool>           guiDoShow;
    void    guiSetShow(bool &_value){drawDebug=_value;};
    
    ofxButton                   guiDoSave;
    void saveGUI();
    
    ofxButton                   guiDoReset;
    void    guiReset();
    
        //GUI cv
    ofParameter<bool>           guiLearnBg;
    void    guiLBg(bool &_value){bLearnBakground=_value;};
    ofParameter<float>          guiCountTime;
    ofParameter<ofVec2f>        guiWarpPoint0;
    ofParameter<ofVec2f>        guiWarpPoint1;
    ofParameter<ofVec2f>        guiWarpPoint2;
    ofParameter<ofVec2f>        guiWarpPoint3;
    
        //GUI video
    ofxVec2Slider               guiVideoScale;
    float                       VideoXscale,VideoYscale;
    ofParameter<string>         guiCurrentVideo;
    ofParameter<string>         guiVideoNum0;
    ofParameter<string>         guiVideoNum1;
    ofParameter<string>         guiVideoNum2;
    ofParameter<string>         guiVideoNum3;
    ofParameter<string>         guiVideoNum4;
    ofParameter<string>         guiVideoNum5;
    
        //GUI ball
    ofxVec2Slider               guiBallExpPos;
    ofxVec2Slider               guiBallRevPos;
    
        //GUI URG
    ofxVec2Slider               guiURGScale;
    ofxVec2Slider               guiURGPos;
    
};
