#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  posX = 274.092;
  posY = 479.405;
  posZ = 0;

  scaleX = 0.178;
  scaleY = 0.233;
  scaleZ = 1.0;

  rotX = 181.502;
  rotY = -0.501;
  rotZ = -1.4;

  ofSetFullscreen(true);
  ofSetVerticalSync(true);
  ofSetFrameRate(PROJECTOR_FRAMERATE);
  ofSetCircleResolution(10);

  lastTime = 0;
  mode = CALIBRATION;
  tcpServer.setup(PORT);
  tcpServer.setMessageDelimiter("\n");
  lastSent = 0;

  mesh.setMode(OF_PRIMITIVE_POINTS);
  mesh.enableColors();

  ofBackground(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update() {
  
}

//--------------------------------------------------------------
void ofApp::draw() {
  drawProjection();

  for (unsigned int i = 0; i <= (unsigned int) tcpServer.getLastID(); i++) {
    if (!tcpServer.isClientConnected(i)) {
      ofLogNotice() << i + " not connected";
      continue;
    }

    // calculate where to draw the text
    int xPos = 15;
    int yPos = 80 + (12 * i * 4);

    // get the ip and port of the client
    string port = ofToString(tcpServer.getClientPort(i));
    string ip   = tcpServer.getClientIP(i);
    string info = "client " + ofToString(i) + " - connected from " + ip + " on port: " + port;


    // if we don't have a string allocated yet, create one
    if (i >= storeText.size()) {
      storeText.push_back( string() );
    }

    // receive all the available messages, separated by '\n'
    // and keep only the last one
    string str;
    string tmp;
    do {
      str = tmp;
      tmp = tcpServer.receive(i);
    } while(tmp != "");

    ofLogNotice() << str;

    // if there was a message set it to the corresponding client
    if(str.length() > 0){
      // draw the info text and the received text below it
      ofDrawBitmapString(info, xPos, yPos);
      ofDrawBitmapString(storeText[i], 25, yPos + 20);

      bool parsingSuccessful = response.parse(str);

      if (parsingSuccessful) {
        ofLogNotice() << response.getRawString();
        int x = (int)(response["x"].asDouble());
        int y = (int)(response["y"].asDouble());
        int shouldClear = (int)(response["shouldClear"].asDouble());

        if (x >= 0 && y >= 0) {
          ofPoint pt;
          pt.set(x, y);
          mesh.addVertex(pt);
          mesh.addColor(ofFloatColor(0.0, 0.0, 1.0)); // Blue
        }

        if (shouldClear > 0) {
          mesh.clear();
        }
      }
    }
  }

  drawMesh();
}

void ofApp::drawProjection() {
  glPushMatrix();

  glTranslatef(posX, posY, posZ);
  glRotatef(rotX, 1, 0, 0);
  glRotatef(rotY, 0, 1, 0);
  glRotatef(rotZ, 0, 0, 1);
  glScalef(scaleX, scaleY, scaleZ);

  // ofFill();
  ofNoFill();
  ofSetColor(ofColor(255,255,255), 100);
  ofDrawRectangle(0, 0, 640, 480);

  // path.setFillColor(ofColor::red);
  // path.rectangle(0, 0, 640, 480);
  // path.rectangle(20, 20, 620, 460);

  glPopMatrix();
}

void ofApp::drawMesh() {
  glPushMatrix();

  glTranslatef(posX, posY, posZ);
  glRotatef(rotX, 1, 0, 0);
  glRotatef(rotY, 0, 1, 0);
  glRotatef(rotZ, 0, 0, 1);
  glScalef(scaleX, scaleY, scaleZ);

  // ofFill();
  ofNoFill();
  // ofSetColor(ofColor(0,0,255), 100);

  mesh.clear();

  ofPoint pt1;
  pt1.set(0, 0);
  mesh.addVertex(pt1);
  mesh.addColor(ofColor(0, 0, 255)); // Blue
  ofPoint pt2;
  pt2.set(320, 0);
  mesh.addVertex(pt2);
  mesh.addColor(ofColor(0, 255, 0));
  ofPoint pt3;
  pt3.set(320, 240);
  mesh.addVertex(pt3);
  mesh.addColor(ofColor(255, 0, 0));

  mesh.draw();

  glPopMatrix();
}

void ofApp::drawPoint(int x,int y) {
  glPushMatrix();

  glTranslatef(posX, posY, posZ);
  glRotatef(rotX, 1, 0, 0);
  glRotatef(rotY, 0, 1, 0);
  glRotatef(rotZ, 0, 0, 1);
  glScalef(scaleX, scaleY, scaleZ);

  // ofFill();
  ofNoFill();
  ofSetColor(ofColor(0, 0, 255), 100);
  ofDrawRectangle(x, y, 1, 1);

  // path.setFillColor(ofColor::red);
  // path.rectangle(0, 0, 640, 480);
  // path.rectangle(20, 20, 620, 460);

  glPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  // Scale X
  if (key == 'm') {
    scaleX +=0.001;
  }

  if (key == 'n') {
    scaleX -=0.001;
  }

  // Scale Y
  if (key == 'l') {
    scaleY += 0.001;
  }

  if (key == 'k') {
    scaleY -= 0.001;
  }

  // Change X/Y position
  if (key == 'w') {
    posY +=0.1;
  }

  if (key == 'z') {
    posY -=0.1;
  }

  if (key == 'a') {
    posX -=0.1;
  }

  if (key == 'd') {
    posX +=0.1;
  }

  // Rotate X
  if (key == 't') {
    rotX -= 0.1;
  }

  if (key == 'y') {
    rotX += 0.1;
  }

  // Rotate Y
  if (key == 'u') {
    rotY -= 0.1;
  }

  if (key == 'i') {
    rotY += 0.1;
  }

  // Rotate Z
  if (key == 'o') {
    rotZ -=0.1;
  }

  if(key=='p'){
      rotZ +=0.1;
  }

  ofLogNotice() << "==========================";
  ofLogNotice() << "{" << posX << ", " << posY << ", "  << posZ << "}";
  ofLogNotice() << "{" << scaleX << ", " << scaleY << ", "  << scaleZ << "}";
  ofLogNotice() << "{" << rotX << ", " << rotY << ", "  << rotZ << "}";
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}