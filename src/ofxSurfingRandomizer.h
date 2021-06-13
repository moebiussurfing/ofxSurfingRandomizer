#pragma once

#include "ofMain.h"


/*

TODO:

+ add store/recall of enable states
+ hide some labels
+ make some spaces minmax, random..

*/

// -> WARNING:
// You must comment this line to run the example!
// When using more ImGui instances, like combining many add-ons that uses ImGui, then you musts uncomment this line. 
#define USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
//#define USE_RANDOMIZE_IMGUI_EXTERNAL // trying to disable ImGui instance...
//#define USE_RANDOMIZE_IMGUI_LOCAL

//-

#include "ofxSurfingImGui.h"
//#include "ofxImGui.h"
//#include "imgui.h"
//#include "imgui_internal.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfing_Timers.h"

#define DEFAULT_MIN_PCT 0.1f
#define DEFAULT_MAX_PCT 0.9f

class ofxSurfingRandomizer
{

public:
	ofxSurfingRandomizer();
	~ofxSurfingRandomizer();

	void setup(ofParameterGroup& group);
	void draw(ofEventArgs & args);
	void exit();
	void keyPressed(int key);

	//-

	// tester timers
private:
	ofParameter<bool> bPlay;
	ofParameter<float> playSpeed;
	int tf;
	float tn;

private:
	void drawParams();

public:
	void drawImGuiWidgets();
	void drawImGuiWidgetsEditor();

	//-

	// commands
public:
	void doRandomize();//do and set random in min/max range for all params
	void doRandomize(int index, bool bForce);//do random in min/max range for a param. bForce ignores enabler
	void doResetParams();//set to minimals
	void doResetRanges();//set ranges to abs min/max from each parameter

private:
	void doSaveState();//save the memory state
	void doLoadState();//load the memory state

	void doSetAll(bool b);
	void doDisableAll();
	void doEnableAll();

	//-

//#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
//	ofxSurfing_ImGui_LayoutManager guiManager;
//#endif

	// ImGui

#ifdef USE_RANDOMIZE_IMGUI_LOCAL
	void setup_ImGui();
#endif

public:
	void setAutodraw(bool autodraw) {
		bAutoDraw = autodraw;
	}

private:
	bool bAutoDraw = true;

	//-

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	ofxSurfing_ImGui_Manager guiManager;
#endif

#ifdef USE_RANDOMIZE_IMGUI_LOCAL
	//void draw_ImGui();
	ofxImGui::Gui gui;
	ofxImGui::Settings mainSettings = ofxImGui::Settings();
	ImFont* customFont = nullptr;
	ofParameter<bool> auto_resize{ "Auto Resize", true };
	ofParameter<bool> bLockMouseByImGui{ "Mouse Locked", false };
	ofParameter<bool> auto_lockToBorder{ "Lock GUI", false };
#endif

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

	ofParameter<bool> bControls;

	//-

	// exposed to external gui's
public:
	ofParameter<bool> bGui;
	ofParameter<bool> bParams;
	ofParameter<bool> bEditor;
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

