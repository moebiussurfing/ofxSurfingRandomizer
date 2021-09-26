#pragma once


/*

This example shows how we apply the randomizations to the both targets types with a scene:

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

#define USE_ONLY_RECTANGLES // -> This is to (maybe) simplify the understanding of the addon. Index controls only the color.

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

	// Scene params
	ofParameterGroup params;
	ofParameter<float> alpha;
	ofParameter<float> round;
	ofParameter<float> rotation;
	ofParameter<int> size;
	ofParameter<glm::vec2> position;
	ofParameter<glm::vec3> rotator;

	ofxSurfingRandomizer randomizer;

	ofParameter<int> index{ "index", 0, 0, 3 }; //-> Index will set the color.
	ofEventListener listenerIndex;
	void refreshColorByIndex();
	ofColor colorByIndex;

	void drawShape(int type, int x, int y, int size);

	ofxWindowApp windowApp;
};
