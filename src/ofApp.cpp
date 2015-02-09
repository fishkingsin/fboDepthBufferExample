#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofEnableArbTex();
    ofEnableSmoothing();
    ofEnableSeparateSpecularLight();
    light.setup();

    light.setSpecularColor(ofColor::white);
    ofFbo::Settings settings;
    settings.width = ofGetWidth();					// width of images attached to fbo
    settings.height = ofGetHeight();					// height of images attached to fbo
    settings.useDepth = true;				// whether to use depth buffer or not
    settings.useStencil = true;				// whether to use stencil buffer or not
    settings.depthStencilAsTexture = true;			// use a texture instead
    settings.internalformat = GL_RGB;
    fbo.allocate(settings);
    depthImageFbo.allocate(settings);
    light.setPosition(fbo.getWidth()*0.5, fbo.getHeight()*0.5, 500);
    shader.load("depthShader");
    
    
    tile.loadImage("tile.png");
    tile.resize(128, 128);
    
    depth.allocate(fbo.getWidth(),fbo.getHeight(),OF_IMAGE_COLOR);
    tileOrigin.clone(tile);
    sis.allocate(depth.getWidth() + tile.getWidth(), fbo.getHeight(), OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){
    
//gl stuff ------------------------------------brgin------------------------------------
    glEnable(GL_DEPTH_TEST);
    light.enable();
    fbo.begin();
    
    ofClear(0, 0, 0);
    ofPushMatrix();
    
    ofTranslate(fbo.getWidth()*0.5, fbo.getHeight()*0.5,0);
    ofRotate(ofGetFrameNum()*0.5, 0, 1, 0);
    ofRotate(180, 1, 0, 0);
    glutSolidTeapot(ofGetWidth()*0.1);
    ofPopMatrix();
    
    fbo.end();
    ofSetColor(255);
    glDisable(GL_DEPTH_TEST);
    
    
    depthImageFbo.begin();
    shader.begin();
    fbo.getDepthTexture(). draw(0,0,fbo.getWidth(),fbo.getHeight());
    shader.end();
    depthImageFbo.end();
//gl stuff ------------------------------------end------------------------------------
    
//autostereogram stuff================================================================
    ofPixels pixels;
    depthImageFbo.readToPixels(pixels);
    depth.setFromPixels(pixels);
    float depthMultiplier = .2;
    if(ofGetMousePressed()) {
        depthMultiplier = ofMap(mouseX, 0, ofGetWidth(), 0, +1);
    }
    depth.setImageType(OF_IMAGE_GRAYSCALE);
    if(bNoise)
    {
        for (int i = 0; i < tile.getWidth(); i++){
            for (int j = 0; j < tile.getHeight(); j++){
                tile.setColor(i, j, ofColor(ofRandom(255)));
            }
        }
    }

    ofxAutostereogram::makeAutostereogram(tile, depth, depthMultiplier, sis);

}

//--------------------------------------------------------------
void ofApp::draw(){
        ofSetColor(255);
    sis.update();
    sis.draw(0, 0, ofGetWidth(), ofGetHeight());
    depthImageFbo.draw(0,0,128,128);
    depth.draw(128,0,128,128);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='f')
    {
        ofToggleFullscreen();
    }
    if(key=='n')
    {
        bNoise=!bNoise;
        if(!bNoise)
        {
            tile.clone(tileOrigin);
        }
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
