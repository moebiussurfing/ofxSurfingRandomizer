#pragma once
#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ImGui_LayoutManager.h"

//#include "range_slider.h"
//#include "range_slider.inl"

#define DEFAULT_MIN_PCT 0.1f
#define DEFAULT_MAX_PCT 0.9f

class ofxSurfingRandomizer
{

public:
	ofxSurfingRandomizer();
	~ofxSurfingRandomizer();
	
	void draw();
	void exit();

	void setup(ofParameterGroup& group);
	void drawParams();
    void doRandomize();
    void doReset();

	//ofxImGui::Gui gui;
	ofxSurfing_ImGui_LayoutManager guiManager;

private:
	void setup_RandomizerPowered(ofParameterGroup& group);
	void addGroup_ToRandomizerPowered(ofParameterGroup& group);

	ofParameterGroup params;

	//TODO:
	//randomizer powered
	//better random engine with min- max for the params
	ofParameterGroup params_RandomizersPowered_Groups;
	
	ofParameterGroup params_RandomizersPowered_Toggles;

	//vector<ofAbstractParameter> randomizersPowered_Vector;
	vector<ofParameter<bool>> randomizersPowered_TogglesVector;

	void doSetAll(bool b);
	void doDisableAll();
	void doEnableAll();


	//helpers
	//if (info == typeid(bool))
	//{
	//	if (ImGui::Checkbox(GetUniqueName(parameter), (bool *)&tmpRef))
	//	{
	//		parameter.set(tmpRef);
	//		return true;
	//	}
	//	return false;
	//}
	//auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(parameter);
	//if (parameterBool)
	//{
	//	ofxImGui::AddParameter(*parameterBool);
	//	continue;
	//}

};

