#pragma once

#include "ofMain.h"

/*

	TODO:

	+ split index/params

	+ add store/recall handly

	+ display index and group names
	+ split toggles windows
		move to params mode / ranges
		make tree foldering
		use pushIDs to avoid collide
		allow adding on runtime?
	+ on ranges window, make columns to put short checkboxes 
		right to the prob sliders

	+ range
	+ draw value too, not only min max
	+ tune range slider on ImGui: drag from center
	+ add multidim vec and colors
	+ add resets for vec
	+ add another mode. center + % spread variation
	+ ransomizer: disable store enablers

*/

//----

// OPTIONAL

// Undo Engine

//#define INCLUDE__OFX_UNDO_ENGINE // -> Comment to disable feature and his dependency

//----


#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"
#include "TextBoxWidget.h"
#include "ofxSurfing_Timers.h"
#include "SurfingIndexRandomizer.h" // Index randomizer

#define DEFAULT_MIN_PCT 0.1f
#define DEFAULT_MAX_PCT 0.9f

#ifdef INCLUDE__OFX_UNDO_ENGINE
#include "ofxSurfingUndoHelper.h"
#endif

class ofxSurfingRandomizer
{
public:

	ofxSurfingRandomizer();
	~ofxSurfingRandomizer();

	void setup();
	void setup(ofParameterGroup& group);
	void setupGui();
	void startup();
	void draw(ofEventArgs& args);
	void update(ofEventArgs& args);
	void keyPressed(ofKeyEventArgs& eventArgs);
	void exit();

	// Legacy API, could remove
	// -> Exposed public to avoid bug that interferes between ImGui instances..
	void draw_ImGui();
	void drawGui() { draw_ImGui(); };

	void rebuildParamsGroup(ofParameterGroup& group);

private:

	void drawImGuiWindows();
	void drawImGuiWindow_Main();
	void drawImGuiWindow_Params();
	void drawImGuiWindow_Ranges();
	void drawImGuiWidgets_RangeEditorResets();
	void drawImGuiWidgets_RangeEditorVecRow(int indexParam, int dimParam);
	void drawImGuiWindows_Index();

	void drawHelp();
	void buildHelp();

	//--

	// Exposed to external GUI's

public:

	ofParameter<bool> bGui;
	ofParameter<bool> bGui_Main;
	ofParameter<bool> bGui_Index;
	ofParameter<bool> bGui_Params;
	ofParameter<bool> bGui_RangesEditor;

	ofParameter<bool> bGui_ModeIndex;//could be removed
	ofParameter<bool> bGui_ModeParams;//could be removed

	ofParameter<bool> bKeys;
	ofParameter<bool> bHelp;

	//--------------------------------------------------------------
	void setGuiVisible(bool b)
	{
		bGui = b;
		bGui_Main = b;//TODO:
	}

	//--

private:

	ofParameter<int> indexTarget{ "Index", 0, 0, 9 };

	SurfingIndexRandomizer surfingIndexRandomizer;

	bool bCustomIndex = false;

public:

	//--------------------------------------------------------------
	void setIndexPtr(ofParameter<int> index)
	{
		bCustomIndex = true;
		indexTarget.makeReferenceTo(index);

		indexTarget.set("Index", 0, 0, index.getMax());

		surfingIndexRandomizer.setPath(path_Global);
		surfingIndexRandomizer.setup(indexTarget/*, bGui_ModeIndex*/);
	}

	//--

public:

	ofParameter<bool> bPlay;
	ofParameter<bool> bPlay_Index;

	//--------------------------------------------------------------
	float getPctRandomizerIndex() {
		return (1 - surfingIndexRandomizer.getPlayerPct());
	}

private:

	ofParameter<bool> bTarget;
	ofParameter<float> playSpeed;
	int tf;
	float tn;

	const int WIDGET_R_DRAG = 70;
	const int WIDGET_R_SLIDER = 200;
	std::string spcl = "    "; // Space between min-max range slider label

	TextBoxWidget textBoxWidget;

	//--

	// Commands

//private:
public:

	enum ResetPramsType
	{
		RESET_PARAM_MIN = 0,
		RESET_PARAM_FULL_CENTER,
		RESET_PARAM_RANGE_CENTER,
		RESET_PARAM_MAX,
		RESET_RANGE_MIN,
		RESET_RANGE_MAX,
	};

public:

	void doRandomize();//do and set random in min/max range for all params
	void doRandomize(int index, bool bForce);//do random in min/max range for a param. bForce ignores enabler
	void doResetParamsFull(ResetPramsType type = RESET_PARAM_MIN);//set to minimals from range or abs param itself
	void doResetRangesFull();//set ranges to abs min/max from each parameter
	void doResetRangesHalf();//set ranges to abs min/max from each parameter but a bit closed
	
	bool isRandomized() { 
		if (bIsRandomized) {
			bIsRandomized = false;
			return true;
		}
		else return false;
	};

private:

	bool bIsRandomized = false;

	void doSaveState();//save the memory state
	void doLoadState();//load the memory state

	void doSetAll(bool b);
	void doDisableAll();
	void doEnableAll();

	//--

	ofxSurfing_ImGui_Manager guiManager;

	ofParameter<bool> bMinimize{ "Minimize", false };

	//--

private:

	void setupEditor(ofParameterGroup& group);
	void addGroup(ofParameterGroup& group);

	ofParameterGroup params; // the external parameters or targets

	ofParameterGroup params_Editor;
	ofParameterGroup params_EditorGroups; // the range limits to randomize each param

	ofParameterGroup params_EditorEnablers; // the enabled params to randomize
	vector<ofParameter<bool>> enablersForParams;

	ofParameterGroup params_AppState;

	//-

private:

	// Settings
	string path_Global = "ofxSurfingRandomizer/";
	string path_Editor; // the editor settings
	string path_AppState; // the control/panels states
	string path_MemoryState; // there's a memory state to play with reset/random workflow

	//--

#ifdef INCLUDE__OFX_UNDO_ENGINE
private:

	ofxSurfingUndoHelper undoManager;

#endif

	public:

		int getAmountEnabledParams() {
			int count = 0;
			for (size_t i = 0; i < enablersForParams.size(); i++)
			{
				if (enablersForParams[i]) count++;
			}
			return count;
		}
};

