#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	/*ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);

	video.initGrabber(1280, 720);

	chain.setup(1280, 720);
	chain.load("ZoomBlur.fs");
	chain.load("CubicLensDistortion.fs");

	chain.setImage(video.getTextureReference());*/
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	ofSetLogLevel(OF_LOG_VERBOSE);
	video.initGrabber(1280, 720);

	chain.setup(1280, 720);
	ofDirectory dir;
	dir.allowExt("fs");
	dir.allowExt("vs");

	//         This is the local ISF folder in bin/data/ISF
	dir.listDir(ofToDataPath("ISF-Folder"));

	//        use the following line for the VDMX ISF directory
	//        dir.listDir("/Library/Graphics/ISF");


	dir.sort();
	int count = 0;
	for (int i = 0; i < dir.size(); i++) {
		// Multi Pass Gaussian and Layer Masks are currently broken in OF
		if (!ofIsStringInString(dir.getPath(i), "Multi Pass Gaussian") && !ofIsStringInString(dir.getPath(i), "Layer Mask")) {
			cout << dir.getPath(i) << endl;
			chain.load(dir.getPath(i));
			chain.setEnable(count, false);
			count++;
		}
	}
	index = 0;
	chain.setImage(video.getTextureReference());
}

//--------------------------------------------------------------
void ofApp::update(){
	video.update();
	chain.update(); 
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	chain.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//chain.setEnable("ZoomBlur", !chain.getEnable("ZoomBlur"));
	if (key == OF_KEY_LEFT) {
		chain.setEnable(index, false);
		index--;
		if (index < 0) {
			index = chain.size() - 1;;
		}
		chain.setEnable(index, true);
	}
	else if (key == OF_KEY_RIGHT) {
		chain.setEnable(index, false);
		index++;
		if (index >= chain.size()) {
			index = 0;
		}
		chain.setEnable(index, true);
	}
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
