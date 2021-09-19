#pragma once


/*

This example shows how we apply the randomizations to the both targets types:

A. Group Parameters
The scene parameters determines the shapes and sizes.
Notice that  you can enable some of the parameters that the groups includes.
Notice that the min-max ranges will be limiting the randomizations.

B. An int index 
The index target randoms determines wich of the colors and shapes to draw.
Notice that some colorsand shapes are more common to appears, and some appears with a short duration when the index happens.

KEYS:
SPACE      : Randomize Parameters
Ctrl+SPACE : Randomize Index
RETURN     : Play timed randomizer
BACKSPACE  : Reset parameters
LEFT-RIGHT : Browse Index

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

	// scene params
	ofParameterGroup params;
	ofParameter<float> size1;
	ofParameter<float> rotation1;
	ofParameter<float> rotation2;
	ofParameter<int> size2;

	ofxSurfingRandomizer randomizer;

	ofParameter<int> index{ "index", 0, 0, 3 };
	ofEventListener listenerIndex;

	void drawShape(int type, int x, int y, int size);
	ofColor color;

	ofxWindowApp windowApp;
};
