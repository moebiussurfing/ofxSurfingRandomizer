
#pragma once

#include "ofMain.h"


//--------
// OPTIONS

#define USE_RAW_IM_GUI__GROUP_RANDOMIZER
//#define USE_GUI_MANAGER__GROUP_RANDOMIZER

//--------


#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"

//#ifdef INCLUDE_ofxSurfingRandomizer
//#include "ofxSurfingRandomizer.h"
//#endif

class SurfingIndexRandomizer
{

public:
	SurfingIndexRandomizer();
	~SurfingIndexRandomizer();

	//#ifdef INCLUDE_ofxSurfingRandomizer
	//private:
	//	ofxSurfingRandomizer randomizer;
	//#endif

	// clicker layout
private:
	ofParameter<int> amntBtnsClicker{ "Max Buttons", 1, 1, 1 };
	ofParameter<bool> respBtnsClicker{ "Responsive", true };
	ofParameter<bool> bExtraClicker{ "Extra", false };

public:
	ofParameterGroup params_Clicker{ "Clicker" };

private:
	ofParameterGroup params_PresetsProbs{ "Index Probs" };
	ofParameterGroup params_PresetDurations{ "Index Duration" };

private:
	float timerPlayerPct;

public:
	float getPlayerPct() {
		return timerPlayerPct;
	}

public:

	void startup();
	//void doCheckRandomReady();

	void update();
	void keyPressed(int key);
	void exit();

private:

	ofParameterGroup group;

	void doDices();

	std::string path_Global;
	std::string filename_RandomizerSettings;

public:
	void setPath(string path)
	{
		path_Global = path;
		ofxSurfingHelpers::CheckFolder(path_Global);
	}

public:
	void setFilenameSettings(string folder)
	{
		ofLogNotice(__FUNCTION__) << "Path: " << folder;

		filename_RandomizerSettings = folder;
	}

public:
	void drawImGui();
	void drawImGui_IndexMain();
	void drawImGui_IndexEditor();

	//-

	ofParameter<bool> bGui_Editor;
	ofParameter<bool> bGui;

	void setBoolGui(ofParameter<bool> &b) {
		bGui.makeReferenceTo(b);
		//bGui = &b;
	}

	//ofParameter<bool> *bGui = NULL;
	//void setBoolGuiPtr(ofParameter<bool> &b) {
	//	bGui = &b;
	//}

public:
	void setup(ofParameter<int> & indexParam, ofParameter<bool> bOpenGui) {
		bGui.makeReferenceTo(bOpenGui);
		indexSelected.makeReferenceTo(indexParam);
		setup(indexParam.getMax() + 1);
	}
	void setup(int _numPresets);

	//-

public:
	ofParameter<int> indexSelected;// main group preset selector (current)

	//----

private:
	ofParameterGroup params_HelperTools;
	ofParameterGroup params_RandomizerIndex;
	ofParameterGroup params_Control;// to use on external gui

	void Changed_Control(ofAbstractParameter &e);

	int amountPresets;// amount of box-clickable handled presets on current favorites/kit
	bool bIsDoneLoad = false;
	void loadPreset(int p);
	ofParameter<bool> MODE_Editor{ "MODE EDIT", true }; // this mode improves performance disabling autosave, undo history..etc
	vector<int> keys; // queued trigger keys for each group ? (all presets) (size of)

	//----

	// randomizer

private:
	ofParameterGroup params_RandomizerSettings{ "Index_Editor" };

	//private:
public:
	ofParameter<bool> bPLAY; // play randomizer
	ofParameter<bool> bRandomizeIndex; // trig randomize index
	ofParameter<float> randomizeDurationBpm; // bpm
	ofParameter<int> randomizeDuration;
	ofParameter<int> randomizeDurationShort;
	ofParameter<float> randomizeDurationShortRatio;

private:
	ofParameter<bool> bEnableRandomizerIndex;
	//ofParameter<bool> MODE_LatchTrig; // this mode trigs the preset but goes back to preset 0 after duration timer
	ofParameter<bool> MODE_AvoidRandomRepeat; // this mode re makes randomize again if new index preset it's the same!
	ofParameter<bool> bResetProbs;
	ofParameter<int> randomizedDice; // to test
	bool bLatchRun = false;

private:
	int randomizeSpeed; // real time duration
	uint32_t randomizerTimer;
	float MAX_DURATION_RATIO = 2.0f;
	int randomize_MAX_DURATION = MAX_DURATION_RATIO * 6000;
	//int randomize_MAX_DURATION_SHORT = 6000 / 2.f;

	vector<ofParameter<int>> presetsRandomFactor; // probability of every preset
	vector<ofParameter<bool>> presetsRandomModeShort; // mode short for ebvery preset

	vector<int> randomFactorsDices;

	void buildRandomizers();
	void setup_RandomizerIndexes(); // engine to get a random between all posible dices (from 0 to dicesTotalAmount) and then select the preset associated to the resulting dice.
	int doRandomIndexChanged();
	void doResetDices(); // reset all probs to 0
	int dicesTotalAmount; // total dices summing the prob of any preset probability (PROB1 + PROB2 + ...)

	int timerRandomizer;

public:
	void doRandom(); // randomize wich preset (usually 1 to 8) is selected (not the params of the preset)

	//--

//private:
//	ofParameterGroup params_randomizer;

private:
	ofParameter<int> randomizerProgress{ "%", 0, 0, 100 };
	float randomizerProgressPrc;

	//--

	//----
	//
	// API
	//
	//----

	// randomizer helpers

public:

	//--------------------------------------------------------------
	void setPlayRandomizerTimer(bool b) // play randomizer timer
	{
		bPLAY = b;
		if (b) doRandom();
	}
	//--------------------------------------------------------------
	void setTogglePlayRandomizerPreset() // toggle randomizer timer
	{
		bPLAY = !bPLAY;
	}
	//--------------------------------------------------------------
	void setRandomizerDuration(float t)
	{
		randomizeDuration = t;
		randomizeDurationBpm = 60000.f / randomizeDuration;
	}
	//--------------------------------------------------------------
	void setRandomizerDurationShort(float t)
	{
		randomizeDurationShort = t;
	}
	//--------------------------------------------------------------
	void setRandomizerBpm(float bpm)
	{
		randomizeDurationBpm = bpm;
		// 60,000 ms (1 minute) / Tempo (BPM) = Delay Time in ms for quarter-note beats
		randomizeDuration = 60000.f / bpm;
		randomizeDurationShort = randomizeDuration * randomizeDurationShortRatio;
		//randomizeDurationShort = randomizeDuration / 2.f;
	}
	private:
	//--------------------------------------------------------------
	void doRandomizePresetFromFavs() // trig randomize and select one of the favs presets
	{
		bRandomizeIndex = true;
	}
	public:
	////--------------------------------------------------------------
	//void doRandomizePresetSelected() { // randomize params of current selected preset
	//	ofLogNotice(__FUNCTION__);
	//	doRandomPreset();
	//}
	//--------------------------------------------------------------
	ofParameterGroup getParamsRandomizers() {
		ofParameterGroup _g{ "RANDOMIZERS" };
		_g.add(bPLAY);
		_g.add(bRandomizeIndex);
		_g.add(randomizeDurationBpm);
		_g.add(randomizeDuration);
		_g.add(randomizeDurationShort);
		_g.add(randomizeDurationShortRatio);
		_g.add(randomizerProgress);
		return _g;
	}

public:
	void setModeRandomizeAvoidRepeat(bool b) {
		MODE_AvoidRandomRepeat = b;
	}
	void setModeEditor(bool b) {
		MODE_Editor = b;
	}

private:
	bool DISABLE_CALLBACKS = true; // to avoid startup crashes and objects are not initialized properly
};

