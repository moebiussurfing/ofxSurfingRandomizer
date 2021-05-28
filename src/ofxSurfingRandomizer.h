#pragma once
#include "ofMain.h"

#include "ofxSurfingHelpers.h"

//#include "ofxSurfing_ImGui_LayoutManager.h"
#include "ofxSurfing_ImGuiBundle.h"

//TODO:
//#include "range_slider.h"
//#include "range_slider.inl"

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

private:
	//tester timers
	ofParameter<bool> bPlay;
	ofParameter<float> playSpeed;
	int tf;
	float tn;

private:
	void drawParams();

	//-

	//commands
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

	ofxSurfing_ImGui_LayoutManager guiManager;

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
	ofParameter<bool> bParams;
	ofParameter<bool> bEditor;
	ofParameter<bool> bControls;

public:
	ofParameter<bool> bGui;

	void setImGuiAutodraw(bool b){ guiManager.setImGuiAutodraw(b); }//required to set to false when only one ImGui instance is created

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

