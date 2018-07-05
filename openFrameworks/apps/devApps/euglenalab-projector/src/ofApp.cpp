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

// MARK: - Draw

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
    ofLogNotice() << "RECEIVED STRING: " << str;

    if (str.length() > 0 && jsonElement.parse(str)) {
      ofLogNotice() << jsonElement.getRawString();
      const string command = jsonElement["command"].asString();
      if (command == "clearScreen") {
        ofClear(ofColor(0, 0, 0));
      } else if (command == "drawPoint") {
        const int x = jsonElement["x"].asInt();
        const int y = jsonElement["y"].asInt();
        const Json::Value& color = jsonElement["color"];
        drawPoint(x, y, color);
      } else if (command == "drawLine") {
        const Json::Value& vertices = jsonElement["vertices"];
        const Json::Value& color = jsonElement["color"];
        drawLine(vertices, color);
      } else if (command == "drawShape") {

        ofLogNotice() << "SHAPE DETECTED";

        const Json::Value& vertices = jsonElement["vertices"];
        const Json::Value& color = jsonElement["color"];
        const bool shouldFill = jsonElement["shouldFill"].asBool();
        drawShape(vertices, color, shouldFill);
      } else if (command == "drawEllipse") {
        const int x = jsonElement["x"].asInt();
        const int y = jsonElement["y"].asInt();
        const int w = jsonElement["w"].asInt();
        const int h = jsonElement["h"].asInt();
        const Json::Value& color = jsonElement["color"];
        const bool shouldFill = jsonElement["shouldFill"].asBool();
        drawEllipse(x, y, w, h, color, shouldFill);
      }
    }
  }
}

void ofApp::applyGLMatrixTransformations() {
  glTranslatef(posX, posY, posZ);
  glRotatef(rotX, 1, 0, 0);
  glRotatef(rotY, 0, 1, 0);
  glRotatef(rotZ, 0, 0, 1);
  glScalef(scaleX, scaleY, scaleZ);
}

void ofApp::drawProjectionFrame() {
  glPushMatrix();
  applyGLMatrixTransformations();

  ofNoFill();
  ofSetColor(ofColor(255,255,255), 100);
  ofDrawRectangle(0, 0, 640, 480);

  glPopMatrix();
}

// color is expected as an ofxJson array
void ofApp::drawPoint(const int x, const int y, const Json::Value& color) {
  glPushMatrix();
  applyGLMatrixTransformations();

  ofFill();
  // ofEnableAlphaBlending();
  //   ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt(), color[3].asInt());
  //   ofDrawRectangle(x, y, 1, 1);
  // ofDisableAlphaBlending();
  ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt(), color[3].asInt());
  ofDrawRectangle(x, y, 1, 1);

  glPopMatrix();
}

// color is expected as an ofxJson array
void ofApp::drawLine(const Json::Value& vertices, const Json::Value& color) {
  glPushMatrix();
  applyGLMatrixTransformations();

  ofPolyline polyline;
  for (Json::ArrayIndex i = 0; i < vertices.size(); ++i) {
    polyline.addVertex(ofPoint(vertices[i][0].asInt(), vertices[i][1].asInt()));
  }
  // ofEnableAlphaBlending();
  //   ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt(), color[3].asInt());
  //   ofBeginShape();  
  //     for (size_t i = 0; i < polyline.getVertices().size(); i++) {
  //       ofVertex(polyline.getVertices().at(i).x, polyline.getVertices().at(i).y);
  //     }
  //   ofEndShape();
  // ofDisableAlphaBlending();
  ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt());
  polyline.draw();

  glPopMatrix();
}

void ofApp::drawShape(const Json::Value& vertices, const Json::Value& color,
    const bool shouldFill) {
  glPushMatrix();
  applyGLMatrixTransformations();

  ofPolyline polyline;
  for (Json::ArrayIndex i = 0; i < vertices.size(); ++i) {
    polyline.addVertex(ofPoint(vertices[i][0].asInt(), vertices[i][1].asInt()));
  }
  polyline.close();

  if (shouldFill) {
    ofFill();
  }
  // ofEnableAlphaBlending();
  //   ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt(), color[3].asInt());
  //   ofBeginShape();  
  //     for (size_t i = 0; i < polyline.getVertices().size(); i++) {
  //       ofVertex(polyline.getVertices().at(i).x, polyline.getVertices().at(i).y);
  //     }
  //   ofEndShape();
  // ofDisableAlphaBlending();
  ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt());
  ofBeginShape();  
    for (size_t i = 0; i < polyline.getVertices().size(); i++) {
      ofVertex(polyline.getVertices().at(i).x, polyline.getVertices().at(i).y);
    }
  ofEndShape();

  glPopMatrix();
}

void ofApp::drawEllipse(const int x, const int y, const int w, const int h,
    const Json::Value& color, const bool shouldFill) {
  glPushMatrix();
  applyGLMatrixTransformations();

  if (shouldFill) {
    ofFill();
  }
  // ofEnableAlphaBlending();
  //   ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt(), color[3].asInt());
  //   ofBeginShape();  
  //     for (size_t i = 0; i < polyline.getVertices().size(); i++) {
  //       ofVertex(polyline.getVertices().at(i).x, polyline.getVertices().at(i).y);
  //     }
  //   ofEndShape();
  // ofDisableAlphaBlending();
  ofSetColor(color[0].asInt(), color[1].asInt(), color[2].asInt());
  ofDrawEllipse(x, y, w, h);

  glPopMatrix();
}

// MARK: - Exit

void ofApp::exit() {
  ofLogNotice() << "disconnecting all clients";
  tcpServer.disconnectAllClients();
  ofLogNotice() << "===========================";
  ofLogNotice() << "        terminating        ";
  ofLogNotice() << "===========================";
}

// MARK: - Keyboard Input

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
  if (key == 'q') {
    ofExit();
  } else {
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