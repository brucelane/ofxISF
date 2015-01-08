#include "ofMain.h"

#include "ofxISF.h"
#include "ofxOsc.h"
class ofApp : public ofBaseApp
{
public:
    
    ofxISF::Shader isf;
    vector<ofVideoPlayer> 	fingerMovie;
    vector<ofFbo*> 	normalized;
    ofVideoGrabber video;
    ofImage img;
    float scale;
    unsigned long long lastInteraction, searchStarted;
    unsigned long long fadeDelay, failDelay;
    string search;
    string display;
    int index;
    ofxOscSender sender;
    ofxOscReceiver receiver;
    void setup()
    {
        ofSetFrameRate(60);
        ofSetVerticalSync(true);
        ofBackground(0);
        
        sender.setup("127.0.0.1", 7777);
        receiver.setup(7778);
        
        scale = 3;
        ofSetWindowShape(5*720/scale, 5*540/scale);
        
        isf.setup(640, 480, GL_RGBA);
        isf.load("shaders/10FrameSum.fs");
        
        ofDirectory dir;
        dir.listDir(ofToDataPath("movies/test"));
        dir.sort();
        int max;
        if(dir.size() > 12){
            max = 10;
        }else{
            max = dir.size();
        }
        
        normalized.resize(max);
        
        for(int i = 0; i < max; i++){
            ofVideoPlayer player =  ofVideoPlayer();
            fingerMovie.push_back(player);
            fingerMovie[i].load(dir.getPath(i));
            fingerMovie[i].setLoopState(OF_LOOP_NONE);
            fingerMovie[i].play();
            fingerMovie[i].setVolume(0);
            fingerMovie[i].setSpeed(1.0);
            //
            normalized[i] = new ofFbo();
            normalized[i]->allocate(640, 480, GL_RGBA);
            normalized[i]->begin();
            ofClear(0, 0, 0, 0);
            normalized[i]->end();
            
            
            if(i == 0){
                isf.setImage("one", normalized[i]->getTexture());
            }else if(i == 1){
                isf.setImage("two", normalized[i]->getTexture());
            }else if(i == 2){
                isf.setImage("three", normalized[i]->getTexture());
            }else if(i == 3){
                isf.setImage("four", normalized[i]->getTexture());
            }else if(i == 4){
                isf.setImage("five", normalized[i]->getTexture());
            }else if(i == 5){
                isf.setImage("six", normalized[i]->getTexture());
            }else if(i == 6){
                isf.setImage("seven", normalized[i]->getTexture());
            }else if(i == 7){
                isf.setImage("eight", normalized[i]->getTexture());
            }else if(i == 8){
                isf.setImage("nine", normalized[i]->getTexture());
            }else if(i == 9){
                isf.setImage("ten", normalized[i]->getTexture());
            }
        }
        
        img.allocate(640, 480, OF_IMAGE_COLOR);
        
        fadeDelay = 1500;
        failDelay = 3000;
        index = 0;
    }
    
    void update()
    {
        oscWorkHorse();
        
        float t = ofGetElapsedTimef();
        for(int i = 0; i < fingerMovie.size(); i++){
            if(fingerMovie[i].isPlaying()){
                fingerMovie[i].update();
                normalized[i]->begin();
                ofClear(0, 0, 0, 0);
                ofSetColor(255, 255, 255);
                ofPushMatrix();
                float ratio = (640.0 / fingerMovie[i].getWidth())>(480.0/fingerMovie[i].getHeight())?(480.0/fingerMovie[i].getHeight()):(640.0/ fingerMovie[i].getWidth());
                ofScale(ratio, ratio);
                fingerMovie[i].draw(0,0);
                ofPopMatrix();
                normalized[i]->end();
            }else{
                normalized[i]->begin();
                ofClear(0, 0, 0, 0);
                normalized[i]->end();
            }
        }
        
        //        Below is an output check out the Shader
        //
        //
        //        ofPixels fooOne;
        //        ofPixels fooTwo;
        //        normalized[0]->readToPixels(fooOne);
        //        normalized[1]->readToPixels(fooTwo);
        //
        //        ofPixels pix;
        //        pix.allocate(fooOne.getWidth(), fooOne.getHeight(), fooOne.getNumChannels());
        //        int nChannels = pix.getNumChannels();
        //        int width = pix.getWidth();
        //        int height = pix.getHeight();
        //        for(int i = 0; i < pix.getWidth(); i++){
        //             for(int j = 0; j < pix.getHeight(); j++){
        //                 pix[(width*j+i)*nChannels] = ((fooOne[(width*j+i)*nChannels]+fooTwo[(width*j+i)*nChannels])/2)%255;
        //                 pix[(width*j+i)*nChannels+1] = ((fooOne[(width*j+i)*nChannels]+fooTwo[(width*j+i)*nChannels+1])/2)%255;
        //                 pix[(width*j+i)*nChannels+2] = ((fooOne[(width*j+i)*nChannels]+fooTwo[(width*j+i)*nChannels+2])/2)%255;
        ////                 pix[(width*j+i)*nChannels+3] = 255;
        //             }
        //        }
        //
        //        img.setFromPixels(pix);
        //        img.update();
        
        isf.update(t);
        ofSetWindowTitle(ofToString(ofGetFrameRate()));
    }
    
    void draw()
    {
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        ofSetColor(255, 255, 255, 255);
        ofPushMatrix();
        float ratio = (ofGetWindowWidth() / 640.0)>(ofGetWindowHeight()/ 480.0)?ofGetWindowHeight()/ 480.0:ofGetWindowWidth() / 640.0;
        ofLog()<<ratio<<endl;
        isf.draw(0, 0, 640*ratio, 480*ratio);
        ofPopMatrix();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        if(ofGetSystemTime() - lastInteraction < failDelay || ofGetSystemTime() - searchStarted < fadeDelay){
            
            ofSetColor(255, 0, 255, ofMap(ofGetSystemTime() - lastInteraction, 0, failDelay, 255, 0));
            ofDrawRectangle(ofGetWidth()/2-(8.0*search.length()+2)/2, ofGetHeight()/2-12, 8.0*(search.length()+2), 16);
            ofSetColor(255, 255, 255, ofMap(ofGetSystemTime() - lastInteraction, 0, failDelay, 255, 0));
            ofDrawBitmapString(display, ofPoint(ofGetWidth()/2-8.0*(search.length()-2)/2, ofGetHeight()/2));
        }else{
            ofSetColor(255, 255, 255, 255);
            search = "";
        }
    }
    
    void keyPressed(int key)
    {
        //isf.load("RGB GlitchMod.fs");
        lastInteraction = ofGetSystemTime();
        if(display == "searching!"){
            display = "";
        }
        if(key == OF_KEY_BACKSPACE){
            if(display.size() > 0){
                display.resize(display.size()-1);
                search.resize(search.size()-1);
            }else{
                display = "";
                search = "";
            }
        }else if(key != OF_KEY_RETURN){
            display+=key;
        }
        
    }
    
    void keyReleased(int key)
    {
        
        if(key != OF_KEY_RIGHT_SHIFT && key != OF_KEY_LEFT_SHIFT && key !=OF_KEY_SHIFT && key != OF_KEY_BACKSPACE && key != OF_KEY_DEL && key != OF_KEY_MODIFIER && key != OF_KEY_RETURN){
            search+=key;
        }
        
        if(key == OF_KEY_RETURN){
            for(int i = 0; i < fingerMovie.size(); i++){
                if(fingerMovie[i].isPlaying()){
                    fingerMovie[i].stop();
                }
                if(fingerMovie[i].isLoaded()){
                    fingerMovie[i].closeMovie();
                }
            }
            
            searchStarted = ofGetSystemTime();
            ofxOscMessage b;
            b.setAddress("/seach");
            b.addStringArg(search);
            sender.sendMessage(b, false);
            display = "searching!";
            search = "";
        }else{
            lastInteraction = ofGetSystemTime();
        }
    }
    void oscWorkHorse(){
        //read videos string from NODE
        
        while(receiver.hasWaitingMessages()){
            ofxOscMessage b;
            receiver.getNextMessage(&b);
            string file = b.getArgAsString(0);
            fingerMovie[index].load(ofToDataPath("movies/"+file));
            fingerMovie[index].setLoopState(OF_LOOP_NONE);
            fingerMovie[index].play();
            fingerMovie[index].setVolume(0);
            fingerMovie[index].setSpeed(1.0);
            
            index=(index+1)%fingerMovie.size();
        }
    }
};

int main(int argc, const char** argv)
{
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp);
    return 0;
}
