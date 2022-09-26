#pragma once


/*

	This example shows how we apply the randomizations 
	to the both targets types on a scene:

	A. Group Parameters

	The scene parameters determines the shapes and sizes.
	Notice that  you can enable some of the parameters that the groups includes.
	Notice that the min-max ranges will be limiting the randomizations.

	B. An int index 
	
	An index could be used to select a preset or pick something on a list.
	The index target randoms determines which of the colors and shapes to draw.
	Notice that some colors and shapes are more common to appears, 
	and some appears with a short duration when the index happens.

	KEYS

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

	ofxSurfingRandomizer randomizer;

	// Index will the randomized.  
	// This int will set the color.
	ofParameter<int> index{ "index", 0, 0, 4 }; 

	ofEventListener listenerIndex;
	void refreshColorByIndex();
	ofColor colorByIndex;

	ofxWindowApp windowApp;
};
