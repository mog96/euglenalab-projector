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
  drawProjectionFrame();
  for (unsigned int i = 0; i <= (unsigned int) tcpServer.getLastID(); i++) {
    if (!tcpServer.isClientConnected(i)) {
      ofLogNotice() << "client " << ofToString(i) << " not connected";
      continue;
    }
    // get the ip and port of the client
    string port = ofToString(tcpServer.getClientPort(i));
    string ip   = tcpServer.getClientIP(i);
    string info = "client " + ofToString(i) + " connected from " + ip + " on port: " + port;
    ofLogNotice() << info;

    // calculate where to draw client info text
    int xPos = 15;
    int yPos = 80 + (12 * i * 4);
    ofDrawBitmapString(info, xPos, yPos);

    // receive all the available messages, separated by '\n'
    // and keep only the last one
    string str;
    string tmp;
    do {
      str = tmp;
      // if (!str.empty()) {
      //   ofBackground(0, 0, 0);
      //   // TODO: draw info string again
      //   ofDrawBitmapString(str, xPos + 10, yPos + 20);
      // }
      tmp = tcpServer.receive(i);      
    } while(tmp != "");
    ofLogNotice() << "STR: " << str;

    if (str.length() > 0 && jsonElement.parse(str)) {
      ofLogNotice() << jsonElement.getRawString();
      const string elementType = jsonElement["type"].asString();
      const Json::Value& color = jsonElement["color"];
      const bool shouldClear = jsonElement["shouldClear"].asBool();
      if (shouldClear) {
        ofClear(ofColor(0, 0, 0));
      }
      if (elementType == "shape") {
        const Json::Value& vertices = jsonElement["vertices"];
        const bool shouldFill = jsonElement["shouldFill"].asBool();
        drawShape(vertices, color, shouldFill);
      } else if (elementType == "point") {
        const int x = jsonElement["x"].asInt();
        const int y = jsonElement["y"].asInt();
        drawPoint(x, y, color);
      }
    }
  }
}

void ofApp::drawProjectionFrame() {
  glPushMatrix();

  glTranslatef(posX, posY, posZ);
  glRotatef(rotX, 1, 0, 0);
  glRotatef(rotY, 0, 1, 0);
  glRotatef(rotZ, 0, 0, 1);
  glScalef(scaleX, scaleY, scaleZ);

  ofNoFill();
  ofSetColor(ofColor(255,255,255), 100);
  ofDrawRectangle(0, 0, 640, 480);

  glPopMatrix();
}

void ofApp::drawPoint(const int x, const int y, const Json::Value& color) {
  glPushMatrix();

  glTranslatef(posX, posY, posZ);
  glRotatef(rotX, 1, 0, 0);
  glRotatef(rotY, 0, 1, 0);
  glRotatef(rotZ, 0, 0, 1);
  glScalef(scaleX, scaleY, scaleZ);

  ofFill();
  ofEnableAlphaBlending();
    ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt(), color[3].asInt());
    ofDrawRectangle(x, y, 1, 1);
  ofDisableAlphaBlending();

  glPopMatrix();
}

void ofApp::drawShape(const Json::Value& vertices, const Json::Value& color, const bool shouldFill) {
  glPushMatrix();

  glTranslatef(posX, posY, posZ);
  glRotatef(rotX, 1, 0, 0);
  glRotatef(rotY, 0, 1, 0);
  glRotatef(rotZ, 0, 0, 1);
  glScalef(scaleX, scaleY, scaleZ);

  ofPolyline polyline;
  for (Json::ArrayIndex i = 0; i < vertices.size(); ++i) {
    polyline.addVertex(ofPoint(vertices[i][0].asInt(), vertices[i][1].asInt()));
  }
  polyline.close();

  if (shouldFill) {
    ofFill();
  }
  ofEnableAlphaBlending();
    ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt(), color[3].asInt());
    ofBeginShape();  
      for (size_t i = 0; i < polyline.getVertices().size(); i++) {
        ofVertex(polyline.getVertices().at(i).x, polyline.getVertices().at(i).y);
      }
    ofEndShape();
  ofDisableAlphaBlending();

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