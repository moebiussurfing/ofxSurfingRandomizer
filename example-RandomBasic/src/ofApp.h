#pragma once

#include "ofMain.h"

#include "ofxSurfingRandomizer.h"

#include "ofxWindowApp.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void draw();

	// params to randomize
	ofParameterGroup params;
	ofParameter<bool> bPrevious;
	ofParameter<bool> bNext;
	ofParameter<float> lineWidth;
	ofParameter<float> separation;
	ofParameter<float> speed;
	ofParameter<int> shapeType;
	ofParameter<int> amount;
	ofParameter<int> size;
	ofParameterGroup params2;
	ofParameter<int> shapeType2;
	ofParameter<int> amount2;
	ofParameter<int> size2;
	ofParameterGroup params3;
	ofParameter<float> lineWidth3;
	ofParameter<float> separation3;
	ofParameter<float> speed3;
	ofParameter<int> shapeType3;

	ofxSurfingRandomizer randomizer;

	ofParameter<int> index{ "index", 0, 0, 8 };
	ofEventListener listenerIndex;

	ofxWindowApp windowApp;
};