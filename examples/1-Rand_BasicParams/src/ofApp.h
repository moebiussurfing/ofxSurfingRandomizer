#pragma once


/*
	This example shows how we apply the randomizations 
	to the A. Group Parameters
	The scene parameters determines the shapes and sizes.
	Notice that  you can enable some of the parameters that the groups includes.
	Notice that the min-max ranges will be limiting the randomizations.
*/

#include "ofMain.h"

#include "ofxSurfingRandomizer.h"

#include "ofxWindowApp.h"

class ofApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void drawScene();

	ofxSurfingRandomizer randomizer;

	// Scene Params
	ofParameter<float> alpha;
	ofParameter<float> round;
	ofParameter<float> rotation;
	ofParameter<int> size;

	// The parameters will be randomized
	ofParameterGroup params;

	void drawShape(int type, int x, int y, int size);
	ofColor color;

	ofxWindowApp windowApp;
};
