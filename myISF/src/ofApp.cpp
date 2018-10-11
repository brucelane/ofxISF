#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);

	video.initGrabber(1280, 720);

	chain.setup(1280, 720);
	chain.load("ZoomBlur.fs");
	chain.load("CubicLensDistortion.fs");

	chain.setImage(video.getTextureReference());
}

//--------------------------------------------------------------
void ofApp::update(){
	video.update();
	chain.update(); 
}

//--------------------------------------------------------------
void ofApp::draw(){
	chain.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	chain.setEnable("ZoomBlur", !chain.getEnable("ZoomBlur"));
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
