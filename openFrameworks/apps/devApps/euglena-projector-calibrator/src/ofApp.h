#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxJSON.h"

#define PROJECTOR_WIDTH 100
#define PROJECTOR_HEIGHT 75
#define PROJECTOR_FRAMERATE 5
#define PORT 32001

enum Mode {CALIBRATION, TEST, PRODUCTION};

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

    void drawProjection();
    void drawPoint(int x, int y);
    void drawLine();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		Mode mode;

		float lastTime;

		float posX;
    float posY;
    float posZ;

    float scaleX;
    float scaleY;
    float scaleZ=1.0;

    float rotX;
    float rotY;
    float rotZ;

    ofxTCPServer tcpServer;
    ofxJSONElement response;

    vector <string> storeText;

    uint64_t lastSent;

    ofMesh mesh;
};
