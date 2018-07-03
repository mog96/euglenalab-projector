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
  for (unsigned int i = 0; i <= (unsigned int) tcpServer.getLastID(); i++) {
    if (!tcpServer.isClientConnected(i)) {
      ofLogNotice() << "client " << ofToString(i) << " not connected";
      continue;
    }

    // if we don't have a string allocated yet, create one
    if (i >= storeText.size()) {
      storeText.push_back(string());
    }

    // receive all the available messages, separated by '\n'
    // and keep only the last one
    string str;
    string tmp;
    do {
      str = tmp;

      // TODO: FIXME: STORE AND DRAW ALL ITEMS RECEIVED (rather than ignoring like this, else what is the point of TCP)

      tmp = tcpServer.receive(i);
    } while(tmp != "");
    storeText[i] = str;
    // ofLogNotice() << storeText[i];
  }
}

//--------------------------------------------------------------
void ofApp::draw() {
  drawProjection();
  for (unsigned int i = 0; i <= (unsigned int) tcpServer.getLastID(); i++) {
    // if there was a message set it to the corresponding client
    if (storeText[i].length() > 0) {
      // get the ip and port of the client
      string port = ofToString(tcpServer.getClientPort(i));
      string ip   = tcpServer.getClientIP(i);
      string info = "client " + ofToString(i) + " connected from " + ip + " on port: " + port;
      ofLogNotice() << info;

      // calculate client info text and received text below it
      int xPos = 15;
      int yPos = 80 + (12 * i * 4);
      ofDrawBitmapString(info, xPos, yPos);
      ofDrawBitmapString(storeText[i], xPos + 10, yPos + 20);

      if (jsonElement.parse(storeText[i])) {
        ofLogNotice() << jsonElement.getRawString();
        bool shouldClear = jsonElement["shouldClear"].asBool();
        int x = jsonElement["x"].asInt();
        int y = jsonElement["y"].asInt();
        if (shouldClear) {
          mesh.clear();
        }
        if (x >= 0 && y >= 0) {
          ofPoint point;
          point.set(x, y);
          mesh.addVertex(point);
          mesh.addColor(ofFloatColor(0.0, 0.0, 1.0)); // Blue
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

  ofPolyline polyline;
  ofPoint pt1;
  pt1.set(0, 0);
  polyline.addVertex(pt1);
  ofPoint pt2;
  pt2.set(320, 0);
  polyline.addVertex(pt2);
  ofPoint pt3;
  pt3.set(320, 240);
  polyline.addVertex(pt3);
  polyline.close();

  // mesh.draw();

  ofFill();
  ofSetColor(0, 0, 255);

  ofBeginShape();  
    for (size_t i = 0; i < polyline.getVertices().size(); i++) {
      ofVertex(polyline.getVertices().at(i).x, polyline.getVertices().at(i).y);
    }
  ofEndShape(); 

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