#pragma once

#include "ofMain.h"

#include "ofxSurfingRandomizer.h"

#include "ofxWindowApp.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void draw();

	// scene params
	ofParameterGroup params;
	ofParameter<float> size1;
	ofParameter<float> rotation;
	ofParameter<float> rotationOffset;
	ofParameter<int> size2;

	ofxSurfingRandomizer randomizer;

	ofParameter<int> index{ "index", 0, 0, 3 };
	ofEventListener listenerIndex;

	ofxWindowApp windowApp;

	void drawShape(int type, int x, int y, int size);
};
