#pragma once

#include "ofMain.h"


/*

TODO:

+ add multidim vec and colors
+ add anothe mode. center + % spread variation
+ tune range slider on ImGui
	drag from center

*/


//----

// OPTIONAL
//#define INCLUDE__OFX_UNDO_ENGINE

//----


#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"
#include "TextBoxWidget.h"
#include "ofxSurfing_Timers.h"

#include "SurfingIndexRandomizer.h" // index randomizer

#define DEFAULT_MIN_PCT 0.1f
#define DEFAULT_MAX_PCT 0.9f

#ifdef INCLUDE__OFX_UNDO_ENGINE
#include "ofxSurfingUndoHelper.h"
#endif

class ofxSurfingRandomizer
{
	enum ResetPramsType
	{
	RESET_PARAM_MIN = 0,
	RESET_PARAM_MAX,
	RESET_RANGE_MIN,
	RESET_RANGE_MAX,
	};

#ifdef INCLUDE__OFX_UNDO_ENGINE
//public:
private:
	ofxSurfingUndoHelper undoManger;
#endif

	//----

public:
	ofxSurfingRandomizer();
	~ofxSurfingRandomizer();

	void setup(ofParameterGroup& group);
	void draw(ofEventArgs & args);
	void update(ofEventArgs & args);
	void exit();
	void keyPressed(ofKeyEventArgs &eventArgs);

	void startup();

	//-

private:
	SurfingIndexRandomizer surfingIndexRandomizer;
	ofParameter<int> indexTarget{ "index", 0, 0, 9 };
	bool bCustomIndex = false;

public:
	void setIndexPtr(ofParameter<int> index)
	{
		bCustomIndex = true;
		indexTarget.makeReferenceTo(index);

		indexTarget.set("Index", 0, 0, index.getMax());

		surfingIndexRandomizer.setPath(path_Global);
		surfingIndexRandomizer.setup(indexTarget, bGui_Index);
	}

	//-

	// tester timers

private:
	ofParameter<bool> bPLAY;
	ofParameter<bool> bTarget;
	ofParameter<float> playSpeed;
	int tf;
	float tn;

public:
	void draw_ImGui(); // -> exposed public to avoid bug that interferes between ImGui instances..

private:
	void drawImGui_Main();
	void drawImGui_Params();
	void drawImGui_RangeEditor();
	void drawImGui_Index();

//public:
	void drawImGui_Widgets();
	void drawHelp();
	TextBoxWidget textBoxWidget;
	
	//-

	// commands

public:
	void doRandomize();//do and set random in min/max range for all params
	void doRandomize(int index, bool bForce);//do random in min/max range for a param. bForce ignores enabler
	void doResetParamsFull(ResetPramsType type = RESET_PARAM_MIN);//set to minimals from range or abs param itself
	void doResetRangesFull();//set ranges to abs min/max from each parameter
	void doResetRangesHalf();//set ranges to abs min/max from each parameter but a bit closed

private:
	void doSaveState();//save the memory state
	void doLoadState();//load the memory state

	void doSetAll(bool b);
	void doDisableAll();
	void doEnableAll();

	//-

	ofxSurfing_ImGui_Manager guiManager;

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
	ofParameter<bool> bGui_RangesEditor;
	ofParameter<bool> bGui_Index;

	ofParameter<bool> bKeys;
	ofParameter<bool> bHelp;

	//-

//	//--------------------------------------------------------------
//	void setImGuiAutodraw(bool b){ guiManager.setImGuiAutodraw(b); }//required to set to false when only one ImGui instance is created
//
//	//--------------------------------------------------------------
//	void setImGuiSharedMode(bool b) {
//		guiManager.setSharedMode(b); // Force shared context
//	}

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

