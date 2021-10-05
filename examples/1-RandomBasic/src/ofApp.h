#pragma once

#include "ofMain.h"

#include "ofxSurfingRandomizer.h"

#include "ofxWindowApp.h" // -> Not required


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


class ofApp : public ofBaseApp{

public:
    void setup();
    void draw();

	ofxWindowApp windowApp;

	// A. Params to randomize
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

	//-

    void setupRandomizer();
	ofxSurfingRandomizer randomizer;

	// B. Index to randomize
	ofParameter<int> index{ "index", 0, 0, 8 };
	ofEventListener listenerIndex;
};
