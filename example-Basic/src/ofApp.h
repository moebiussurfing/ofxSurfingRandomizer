#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxSurfingRandomizer.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    ofxImGui::Gui gui;
	
	// all the parameters for the scene 
	ofParameterGroup paramsNested1;
	ofParameterGroup paramsNested2;
	ofParameter<bool> fill;
	ofParameter<float> lineWidth;
	ofParameter<ofFloatColor> color;
	ofParameter<int> shapeType;
	ofParameter<int> amount;
	ofParameter<int> separation;
	ofParameter<int> size;

	// the main group container
	ofParameterGroup params;

	ofxSurfingRandomizer randomizerGroup;
};
