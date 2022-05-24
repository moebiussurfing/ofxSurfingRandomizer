#pragma once

#include "ofMain.h"

#include "ofxSurfingRandomizer.h"

#include "ofxWindowApp.h" // -> Not required


/*

	This example shows how we apply the randomizations to the both targets types:

	A. A Group of Parameters
	Here the Scene Parameters determines the shapes and sizes.
	Notice that you can enable some of the parameters that the groups includes.
	Or you can disable to hide the paramater and exclud it from param randomizations.
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

	ofParameterGroup params;
	ofParameter<float> size1;
	ofParameter<int> size2;
	ofParameter<float> rotation1;
	ofParameter<float> rotation2;
	ofParameter<int> indexColor;

	//-

    void setupRandomizer();
	ofxSurfingRandomizer randomizer;

	// B. Index to randomize
	ofParameter<int> index{ "index", 0, 0, 2 };
	ofEventListener listenerIndex;

	void drawScene();
};
