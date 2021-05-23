#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ImGui.h"

#define DEFALUT_MIN_PCT 0.2f
#define DEFALUT_MAX_PCT 0.8f

class ofxSurfingRandomizer
{

public:

	ofxSurfingRandomizer();
	~ofxSurfingRandomizer();

	void setup(ofParameterGroup& group);
	void drawWidgets();
	void exit();
    void doRamdomize();
    void doReset();

private:

	void setup_RandomizerPowered(ofParameterGroup& group);
	void addGroupToRandomizerPowered(ofParameterGroup& group);

	//TODO:
	//randomizer powered
	//better random engine with min- max for the params
	ofParameterGroup params_RandomizersPowered;
	
	ofParameterGroup params_RandomizersPowered_Toggles;

	//vector<ofAbstractParameter> randomizersPowered_Vector;
	vector<ofParameter<bool>> randomizersPowered_TogglesVector;
	
};

