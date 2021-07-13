#pragma once

#include "ofMain.h"


/*

TODO:

+ add store/recall of enable states

*/


#define USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER

//-

#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_Timers.h"

#include "SurfingIndexRandomizer.h" // index randomizer

#define DEFAULT_MIN_PCT 0.1f
#define DEFAULT_MAX_PCT 0.9f

class ofxSurfingRandomizer
{

public:
	ofxSurfingRandomizer();
	~ofxSurfingRandomizer();

	void setup(ofParameterGroup& group);
	void draw(ofEventArgs & args);
	void update(ofEventArgs & args);
	void exit();
	void keyPressed(ofKeyEventArgs &eventArgs);

	//-

private:
	SurfingIndexRandomizer surfingGroupRandomizer;
	ofParameter<int> indexTarget{ "index", 0, 0, 9 };
	bool bCustomIndex = false;
public:
	void setTarget(ofParameter<int> index)
	{
	bCustomIndex = true;
		indexTarget.makeReferenceTo(index);
	}

	//-

	// tester timers

private:
	ofParameter<bool> bPLAY;
	ofParameter<float> playSpeed;
	int tf;
	float tn;

private:
	void drawImGui_Main();
	void drawImGui_Params();
	void drawImGui_Editor();
	void drawImGui_Index();

public:
	void drawImGui_Widgets();

	//-

	// commands

public:
	void doRandomize();//do and set random in min/max range for all params
	void doRandomize(int index, bool bForce);//do random in min/max range for a param. bForce ignores enabler
	void doResetParams(bool bFull = false);//set to minimals from range or abs param itself
	void doResetRanges();//set ranges to abs min/max from each parameter

private:
	void doSaveState();//save the memory state
	void doLoadState();//load the memory state

	void doSetAll(bool b);
	void doDisableAll();
	void doEnableAll();

	//-

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	ofxSurfing_ImGui_Manager guiManager;
#endif

public:
	void setAutodraw(bool autodraw) { // required to set to true when only one ofxImGUi instance will be used between all the ofApp project add-ons using ofxImGui
		bAutoDraw = autodraw;
	}

private:
	bool bAutoDraw = true;

	ofParameter<bool> bMinimal{ "Minimal", false };

	//-

private:

	void setupEditor(ofParameterGroup& group);
	void addGroup(ofParameterGroup& group);

	ofParameterGroup params;//the external parameters or targets

	ofParameterGroup params_Editor;
	ofParameterGroup params_EditorGroups;//the range limits to randomize each param

	ofParameterGroup params_EditorEnablers;//the enabled params to randomize
	vector<ofParameter<bool>> enablersForParams;

	ofParameterGroup params_AppState;

	//-

	// exposed to external gui's
public:
	ofParameter<bool> bGui;
	ofParameter<bool> bGui_Params;
	ofParameter<bool> bGui_Editor;
	ofParameter<bool> bGui_Index;

	ofParameter<bool> bKeys;

	//-

//#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
//	//--------------------------------------------------------------
//	void setImGuiAutodraw(bool b){ guiManager.setImGuiAutodraw(b); }//required to set to false when only one ImGui instance is created
//
//	//--------------------------------------------------------------
//	void setImGuiSharedMode(bool b) {
//		guiManager.setSharedMode(b); // Force shared context
//	}
//#endif

private:
	//settings
	string path_Global = "ofxSurfingRandomizer/";
	string path_Editor;//the editor settings
	string path_AppState;//the control/panels states
	string path_MemoryState;//there's a memory state to play with reset/random workflow

	//----

	//TODO:
	//a custom group ImGui populater for custom group editor...
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

