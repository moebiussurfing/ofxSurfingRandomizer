#include "SurfingIndexRandomizer.h"

//--------------------------------------------------------------
SurfingIndexRandomizer::SurfingIndexRandomizer()
{
	path_Global = "SurfingIndexRandomizer";
	filename_RandomizerSettings = "SurfingRandom_Index.json";
}

//--------------------------------------------------------------
SurfingIndexRandomizer::~SurfingIndexRandomizer()
{
	exit();
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::setup(int _numPresets) {
	ofLogNotice(__FUNCTION__) << " amount presets: " << _numPresets;

	// Update control gui panel params
	amountPresets = _numPresets;

	indexSelected.set("Index", 0, 0, amountPresets - 1);

	//TODO:
	bGui.set("INDEX MAIN", false);
	bGui_Editor.set("INDEX EDIT", false);

	//-

	// Clicker
	if (amountPresets != 0)
	{
		amntBtnsClicker.setMax(amountPresets);

		amntBtnsClicker.set(3);
		//amntBtnsClicker.set(amountPresets);
	}

	params_Clicker.add(amntBtnsClicker);
	params_Clicker.add(respBtnsClicker);
	params_Clicker.add(bExtraClicker);
	params_Clicker.add(bMinimize);

	//-

	// Randomizers
	buildRandomizers();

	// To store settings
	params_RandomizerSettings.add(params_RandomizerIndex);
	params_RandomizerSettings.add(bGui_Editor);
	//params_RandomizerSettings.add(params_Clicker);

	//-

	params_Control.setName("SurfingIndexRandomizer");
	params_Control.add(indexSelected);

	ofAddListener(params_Control.parameterChangedE(), this, &SurfingIndexRandomizer::Changed_Control);
	ofAddListener(params_RandomizerIndex.parameterChangedE(), this, &SurfingIndexRandomizer::Changed_Control);

	//TODO:
	//default probs
	doReset();

	//----

	// Startup
	startup();
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::startup()
{
	DISABLE_CALLBACKS = false;

	// Load randomizers settings
	ofxSurfingHelpers::loadGroup(params_RandomizerSettings, path_Global + "/" + filename_RandomizerSettings);

	doDices();
}

//--------------------------------------------------------------
int SurfingIndexRandomizer::doRandomIndexChanged()
{
	ofLogVerbose(__FUNCTION__);

	//-

	int _r = indexSelected;

	if (bEnableRandomizerIndex)
	{
		// 1. Dice randomize

#ifndef DEBUG_randomTest
		{
			// Get a random between all possible dices (from 0 to dicesTotalAmount) and then select the preset associated to the resulting dice.
			// each preset has many dices: more dices means more probality to be selected by the randomizer
			randomizedDice = ofRandom(0, dicesTotalAmount);

			ofLogVerbose(__FUNCTION__) << "random: " << randomizedDice.get() << "/" << dicesTotalAmount;
		}
#endif

		//-

		// 2. Define limits for range dices associated to any preset

		//randomFactorsDices[0] = 0;
		for (int i = 0; i < presetsRandomFactor.size(); i++) {
			randomFactorsDices[i] = 0;

			for (int f = 0; f < i; f++) {
				randomFactorsDices[i] += presetsRandomFactor[f];
			}
		}

		//-

		// 3. Check if dice is inside both ranges. to select preset (_rr) associated to dice 

		int _rr = 0;
		for (int i = 0; i <= presetsRandomFactor.size(); i++) {

			// Define upper/lower limits for each dices/preset
			int start;
			int end;

			if (i == 0) start = 0;
			else start = randomFactorsDices[i - 1];

			// TODO:
			// Bug on last preset..

			//if (i == presetsRandomFactor.size() - 1)
			//	end = presetsRandomFactor.size()-1;
			//else
			//	end = randomFactorsDices[i];
			//end = randomFactorsDices[i];

			if (i != presetsRandomFactor.size()) end = randomFactorsDices[i];
			else end = dicesTotalAmount;

#ifdef DEBUG_randomTest
			//ofLogNotice(__FUNCTION__) << "[" << i << "] " << start << "-" << end;
			ofLogNotice(__FUNCTION__) << (start == end ? "\t\t\t\t\t" : "") << "[" << i << "] " << start << "-" << end;// this are empty probs
#endif
			// Check if dice is inside both limits
			if (randomizedDice >= start && randomizedDice < end)
			{
				_rr = i - 1;
			}
		}

#ifdef DEBUG_randomTest
		ofLogNotice(__FUNCTION__) << "dice: " << randomizedDice << "  ->  index preset: [" << _rr << "]";
		//ofLogNotice(__FUNCTION__) << "preset: " << _rr + 1;
#endif
		_r = _rr;
		//_r = _rr + 1;

		// debug
		ofLogVerbose(__FUNCTION__) << "DEBUG";
		for (int i = 0; i < presetsRandomFactor.size(); i++) {
#ifdef DEBUG_randomTest
			ofLogNotice(__FUNCTION__) << "randomFactorsDices: [" << i << "] " << randomFactorsDices[i];
			// << "-" << randomFactorsDices[i + 1] << endl;
#endif
		}

		// Last
#ifdef DEBUG_randomTest
		ofLogNotice(__FUNCTION__) << "randomFactorsDices: [" << presetsRandomFactor.size() << "] " << dicesTotalAmount;
#endif

		//for (int f = 1; f < presetsRandomFactor.size(); f++) {
		//	start = presetsRandomFactor[i - 1].get();
		//}
		//
		//if (randomizedDice >= presetsRandomFactor[i - 1].get() && randomizedDice < presetsRandomFactor[i].get()) {
		//}
		//}

		//for (auto &p : presetsRandomFactor) {
		//	dicesTotalAmount += p.get();
		//}
	}

	//else {
	//	int numTryes = 0;
	//	//avoid jump to same current preset
	//	while (indexSelected == _r)//if not changed
	//	{
	//		ofLogWarning(__FUNCTION__) << "Randomize not changed! Try #" << ofToString(++numTryes);
	//		ofLogNotice(__FUNCTION__) << "PRESET Previous was : " << ofToString(_r);
	//		ofLogNotice(__FUNCTION__) << "PRESET New Random is: " << ofToString(indexSelected);
	//		indexSelected = (int)ofRandom(0, mainGroupMemoryFilesPresets.size());
	//		
	//		//if (MODE_MemoryLive) _r = (int)ofRandom(0, mainGroupMemoryFilesPresets.size());
	//		//_r = (int)ofRandom(1, mainGroupMemoryFilesPresets.size() + 1);
	//	}
	//}

	return _r;
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::doRandom()
{
#ifdef DEBUG_randomTest
	ofLogNotice(__FUNCTION__) << "---------------------------------------------------";
#endif

	// We avoid that the random is the same previous preset (TODO:improve). we want force change, not stay in the same. 
	// bc sometimes the random gets the same current preset.

	int _PRESET_selected_PRE = indexSelected;

	int r = doRandomIndexChanged();

#ifdef DEBUG_randomTest
	ofLogNotice(__FUNCTION__) << "dicesTotalAmounts: " << ofToString(dicesTotalAmount);
#endif

	// TODO:
	// If there's only one posible dice.. cant avoid repetition. so force avoid toggle to false
	if (bAvoidRepeatRand && dicesTotalAmount < 2) bAvoidRepeatRand = false;

	if (bAvoidRepeatRand)
	{
		int numTryes = 0;
		ofLogVerbose(__FUNCTION__) << "Randomize Try #" << ofToString(++numTryes);
		ofLogVerbose(__FUNCTION__) << "PRESET : " << ofToString(r);

		while (r == _PRESET_selected_PRE && dicesTotalAmount > 1 && numTryes < 5) //while preset index not changed. TODO: avoid make more than 5 randoms..
		{
			r = doRandomIndexChanged();
			ofLogVerbose(__FUNCTION__) << "Randomize Try #" << ofToString(++numTryes) << " NOT changed!";
			ofLogVerbose(__FUNCTION__) << "PRESET Previous was : " << ofToString(_PRESET_selected_PRE);
			ofLogVerbose(__FUNCTION__) << "PRESET New Random is: " << ofToString(r);
			ofLogVerbose(__FUNCTION__) << "RETRY !";
		}
	}
	else ofLogVerbose(__FUNCTION__) << "PRESET : " << ofToString(r);

	ofLogVerbose(__FUNCTION__) << "Randomize " + group.getName() + "index : " << ofToString(r);

	//--

	// 4. Apply preset selection

	loadPreset(r);

	//--

	// 5. Start timer again

	if (bPlay)
	{
		randomizerTimer = ofGetElapsedTimeMillis();
	}
}

/*
//--------------------------------------------------------------
void SurfingIndexRandomizer::doReset()
{
	for (auto& p : presetsRandomFactor)
	{
		p = 1;
	}

	//TODO:
	// custom pattern is manually setted to the same as default startup!

	if (presetsRandomFactor.size() > 1) {
		presetsRandomFactor[0] = 8;
		presetsRandomFactor[1] = 8;
		presetsRandomFactor[2] = 8;
		presetsRandomFactor[3] = 5;
		presetsRandomFactor[4] = 5;
		presetsRandomFactor[5] = 5;
		presetsRandomFactor[6] = 2;
		presetsRandomFactor[7] = 2;
		presetsRandomFactor[8] = 2;

		dicesTotalAmount = 0;
		for (size_t i = 0; i < presetsRandomFactor.size(); i++)
		{
			dicesTotalAmount += presetsRandomFactor[i];
		}
	}
	else {
		ofLogError(__FUNCTION__) << "ZERO presetsRandomFactor.size!";

		dicesTotalAmount = 0;
	}
	randomizedDice.setMax(dicesTotalAmount - 1);
	randomizeDuration = 1000;
	randomizeDurationShort = randomizeDuration * randomizeDurationShortRatio;

	// Reset all to long
	for (auto& p : presetsRandomModeShort) {
		p.set(false);
	}

	// 3 last to true
	presetsRandomModeShort[presetsRandomModeShort.size() - 1] = true;
	presetsRandomModeShort[presetsRandomModeShort.size() - 2] = true;
	presetsRandomModeShort[presetsRandomModeShort.size() - 3] = true;
}
*/

//--------------------------------------------------------------
void SurfingIndexRandomizer::doReset()
{
	ofLogNotice(__FUNCTION__);

	int sz = presetsRandomFactor.size();
	int div = sz / 4;

	for (size_t i = 0; i < sz; i++)
	{
		if (i < div + 1) {
			presetsRandomFactor[i] = 10;
			presetsRandomModeShort[i] = false;
		}
		else if (i < 3 * div) {
			presetsRandomFactor[i] = 5;
			presetsRandomModeShort[i] = false;
		}
		else {
			presetsRandomFactor[i] = 1;
			presetsRandomModeShort[i] = true;
		}
	}

	doDices();

	randomizeDurationShortRatio = 0.25f;
	randomizeDurationBpm = 120.f;
	//randomizeDurationShort = randomizeDuration * randomizeDurationShortRatio;

	//--

	/*
	for (auto& p : presetsRandomFactor)
	{
		p = 0;
	}

	if (presetsRandomFactor.size() > 0)
	{
		presetsRandomFactor[0] = 1;
		dicesTotalAmount = 1;
		randomizedDice = 0;
	}
	else
	{
		dicesTotalAmount = 0;
	}

	randomizedDice.setMax(dicesTotalAmount - 1);

	randomizeDuration = 1000;
	randomizeDurationShort = randomizeDuration * randomizeDurationShortRatio;

	// reset all to long
	for (auto& p : presetsRandomModeShort)
	{
		p.set(false);
	}
	*/
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::buildRandomizers()
{
	// Radomizer
	setup_RandomizerIndexes();
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::exit()
{
	// Save randomizers settings
	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::saveGroup(params_RandomizerSettings, path_Global + "/" + filename_RandomizerSettings);

	ofRemoveListener(params_Control.parameterChangedE(), this, &SurfingIndexRandomizer::Changed_Control);
	ofRemoveListener(params_RandomizerIndex.parameterChangedE(), this, &SurfingIndexRandomizer::Changed_Control);
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::update()
{
	//----

	// Randomizer timer mode latch

	// TODO:
	// On this mode: when we click to some presets, we load the preset, but after duration, 
	// we jump back to preset index 0

	// Easy callback
	// latch mode
	//if (bIsDoneLoad && bEnableLatch && !bPlay)
	if (bIsDoneLoad && !bPlay)
	{
		bIsDoneLoad = false;
		randomizerTimer = ofGetElapsedTimeMillis();

		if (indexSelected != 0)
		{
			bLatchRun = true;
		}
		else
		{
			bLatchRun = false;
		}
	}

	//----

	//if (bPlay || bEnableLatch)// ?
	if (bPlay)
	{
		uint32_t _time = ofGetElapsedTimeMillis();
		timerRandomizer = _time - randomizerTimer;

		//ofLogNotice(__FUNCTION__) << " : " << timerRandomizer;

		if (indexSelected < presetsRandomModeShort.size()) {// avoid out of range

			// A. Long mode
			if (presetsRandomModeShort[indexSelected] == false)// get if it's marked as shor or long by default (false)
			{
				timerPlayerPct = ofMap(timerRandomizer, 0, randomizeDuration, 0, 1, true);

				if (timerRandomizer >= randomizeDuration)
				{
					//if (bEnableLatch) {
					//	if (bLatchRun) {
					//		loadPreset(0);
					//	}
					//}
					//else 
					{
						// TODO: can be improved calling directly the method! 
						// bc this flag will be readed on update()..
						bRandomRunIndex = true;
					}
				}
			}

			//-

			// B. Short mode
			else
			{
				timerPlayerPct = ofMap(timerRandomizer, 0, randomizeDurationShort, 0, 1, true);

				if (timerRandomizer >= randomizeDurationShort)
				{
					//if (bEnableLatch) 
					//{
					//	if (bLatchRun) loadPreset(0);
					//}
					//else bRandomRunIndex = true;

					bRandomRunIndex = true;
				}
			}
		}
	}
	else {
		timerPlayerPct = 0;
	}

	//--

	// 1.0.2 Draw progress bar for the randomizer timer

	//// long mode
	//if (presetsRandomModeShort[indexSelected - 1] == false) randomizerProgressPrc = timerRandomizer / (float)randomizeDuration;
	//// short mode
	//else randomizerProgressPrc = timerRandomizer / (float)randomizeDurationShort;
	// bar relative only to long

	if (bPlay)
	{
		randomizerProgress = 100 * timerRandomizer / (float)randomizeDuration;
	}
	//else if (bEnableLatch)
	//{
	//	if (bLatchRun)
	//	{
	//		randomizerProgress = 100 * timerRandomizer / (float)randomizeDuration;
	//	}
	//	else
	//	{
	//		randomizerProgress = 0;
	//	}
	//}
	else
	{
		randomizerProgress = 0;
	}
	randomizerProgressPrc = MIN(100, randomizerProgress / 100.f);
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::setup_RandomizerIndexes()
{
	bPlay.set("PLAY INDEX", false);
	bRandomRunIndex.set("RUN INDEX", false);
	bEnableRandomizerIndex.set("Enable", true);
	randomizeDurationBpm.set("t BPM", 120, 10, 400);
	randomizeDuration.set("t Duration", 1000, 10, randomize_MAX_DURATION);
	randomizeDurationShortRatio.set("t2 Ratio", 0.25, 0.005, 1);
	randomizeDurationShort.set("t2 Short", 250, 10, randomize_MAX_DURATION);// locked
	randomizedDice.set("DICE", 0, 0, amountPresets - 1);
	bAvoidRepeatRand.set("Avoid Repeat", true);
	bResetProbs.set("Reset Probs", false);
	//bEnableLatch.set("Latch", false);

	// Exclude
	//bPlay.setSerializable(false);
	randomizeDurationShort.setSerializable(false);// lock
	bRandomRunIndex.setSerializable(false);
	bResetProbs.setSerializable(false);

	// Erase
	presetsRandomFactor.clear();
	presetsRandomModeShort.clear();
	randomFactorsDices.clear();

	// Resize
	presetsRandomFactor.resize(amountPresets);
	presetsRandomModeShort.resize(amountPresets);
	randomFactorsDices.resize(amountPresets);

	int i;

	//TODO:
	// this can be a method to use as reset
	// Ints as probability for every preset
	params_PresetsProbs.clear();
	i = 0;
	for (auto& p : presetsRandomFactor) {
		string n = "Prob " + ofToString(i++);
		//limits
		int r2 = presetsRandomFactor.size() - 2;
		int r1 = 4;
		if (i < r1)//first 2 will be more prob!
			p.set(n, 8, 0, 10);
		else if (i >= r1 && i < r2)//mid prob
			p.set(n, 5, 0, 10);
		else if (i >= r2)//last 3 will be less prob!
			p.set(n, 2, 0, 10);

		params_PresetsProbs.add(p);
	}

	// Toggles to enable short duration mode
	params_PresetDurations.clear();
	i = 0;
	for (auto& p : presetsRandomModeShort) {
		string n = "Short " + ofToString(i++);

		if (i < presetsRandomModeShort.size() - 2)//last 3 will be short!
			p.set(n, false);
		else
			p.set(n, true);

		params_PresetDurations.add(p);
	}

	params_RandomizerIndex.clear();
	params_RandomizerIndex.setName("RANDOM INDEX CONTROLS");
	params_RandomizerIndex.add(bPlay);
	params_RandomizerIndex.add(bEnableRandomizerIndex);
	params_RandomizerIndex.add(bRandomRunIndex);
	params_RandomizerIndex.add(randomizeDurationBpm);
	params_RandomizerIndex.add(randomizeDuration);
	params_RandomizerIndex.add(randomizeDurationShortRatio);
	params_RandomizerIndex.add(randomizeDurationShort); // locked
	params_RandomizerIndex.add(bAvoidRepeatRand);
	params_RandomizerIndex.add(params_PresetsProbs); // probs
	params_RandomizerIndex.add(params_PresetDurations); // toggles
	params_RandomizerIndex.add(bResetProbs);
	//params_RandomizerIndex.add(bEnableLatch);

#ifdef DEBUG_randomTest
	params_RandomizerIndex.add(randomizedDice);
#endif
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::drawImGuiWidgets_IndexMain()
{
	if (bGui)
	{
		// Calculate common widgets sizes to fit current window being responsive.
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

		//--

		//// Minimize
		//ofxImGuiSurfing::AddToggleRoundedButton(bMinimize, ImVec2(1.5 * _h, 1.5 * (2 / 3.f) * _h));

		//ofxImGuiSurfing::AddSpacingSeparated();

		//-

		// Enable
		if (!bMinimize)
		{
			ofxImGuiSurfing::AddBigToggle(bEnableRandomizerIndex, _w100, _h * 1.5f);
			if (!bEnableRandomizerIndex) return;

			ofxImGuiSurfing::AddSpacingSeparated();
		}

		// Run Random
		ofxImGuiSurfing::AddBigButton(bRandomRunIndex, ImVec2(_w100, _h * 1.5f));

		ImGui::Spacing();

		//---

		// 1 Player

		// Blink by timer
		float tn = getPlayerPct();
		ofxImGuiSurfing::AddBigToggleNamed(bPlay, _w100, 2 * _h, "PLAYING INDEX", "PLAY INDEX", true, 1 - tn);

		// 2 Draw progress bar for the randomizer timer
		//ofxImGuiSurfing::ProgressBar(tn); // to get the short timers scaled to bar
		ofxImGuiSurfing::AddProgressBar(tn);

		ImGui::Spacing();

		if (!bMinimize)
		{
			ofxImGuiSurfing::AddCheckBox(bAvoidRepeatRand);
			//ofxImGuiSurfing::AddCheckBox(bEnableLatch);
		}

		ofxImGuiSurfing::AddSpacingSeparated();

		//--

		// 3. Index Selector

		ofxImGuiSurfing::AddParameter(indexSelected);

		ofxImGuiSurfing::AddSpacing();

		//--

		// Clicker
		{
			_w100 = ofxImGuiSurfing::getWidgetsWidth(1);
			_w50 = ofxImGuiSurfing::getWidgetsWidth(2);

			float __h = 2 * ofxImGuiSurfing::getWidgetsHeightUnit();

			ofxImGuiSurfing::AddMatrixClicker(indexSelected, respBtnsClicker, amntBtnsClicker, true, __h);

			//ImGui::Spacing();
		}

		//--

		if (!bMinimize)
		{
			if (!bGui_Editor) 
			{
				ofxImGuiSurfing::AddSpacingSeparated();

				ImGui::Text("CLOCK");
				ofxImGuiSurfing::AddSpacing();

				// BPM 

				ofxImGuiSurfing::AddParameter(randomizeDurationBpm);

				//--

				ImGui::Spacing();

				if (ImGui::Button("Half", ImVec2(_w50, _h))) { randomizeDurationBpm /= 2.f; }
				ImGui::SameLine();
				if (ImGui::Button("Double", ImVec2(_w50, _h))) { randomizeDurationBpm *= 2.f; }

				if (ImGui::Button("Reset", ImVec2(_w100, _h))) { randomizeDurationBpm = 120.f; }
			}

			//--
			/*
			ofxImGuiSurfing::AddSpacingSeparated();

			ofxImGuiSurfing::AddToggleRoundedButton(bExtraClicker);
			ofxImGuiSurfing::AddSpacing();
			if (bExtraClicker)
			{
				ImGui::Indent();

				if (ImGui::TreeNodeEx("CLICKER", ImGuiTreeNodeFlags_Framed))
				{
					ofxImGuiSurfing::AddToggleRoundedButton(respBtnsClicker);
					if (respBtnsClicker)
					{
						ofxImGuiSurfing::AddStepperInt(amntBtnsClicker);
					}

					ImGui::TreePop();
				}

				ImGui::Unindent();
			}
			*/
		}
	}
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::drawImGuiWidgets_IndexEditor()
{
	if (bGui_Editor)
	{
		// Calculate common widgets sizes to fit current window being responsive.
		float _w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
		float _w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width
		float _w3 = ofxImGuiSurfing::getWidgetsWidth(3); // 3 widgets third width
		float _w4 = ofxImGuiSurfing::getWidgetsWidth(4); // 4 widgets quarter width
		float _h = ofxImGuiSurfing::getWidgetsHeightRelative();

		//-

		//// Minimize
		//ofxImGuiSurfing::AddToggleRoundedButton(bMinimize, ImVec2(1.5 * _h, 1.5 * (2 / 3.f) * _h));

		//ofxImGuiSurfing::AddSpacingSeparated();

		//--

		//if (bEnableRandomizerIndex)
		{
			// BPM

			if (bMinimize)
			{
				if (ImGui::Button("Half", ImVec2(_w2, _h))) { randomizeDurationBpm /= 2.f; }
				ImGui::SameLine();
				if (ImGui::Button("Double", ImVec2(_w2, _h))) { randomizeDurationBpm *= 2.f; }
				if (ImGui::Button("Reset", ImVec2(_w1, _h))) {
					randomizeDurationBpm = 120.f;
				}

				ofxImGuiSurfing::AddParameter(randomizeDurationBpm);
				ofxImGuiSurfing::AddParameter(randomizeDuration);
				ofxImGuiSurfing::AddParameter(randomizeDurationShort);
				ofxImGuiSurfing::AddParameter(randomizeDurationShortRatio);
			}

			if (!bMinimize)
			{
				if (ImGui::TreeNodeEx("BPM", ImGuiTreeNodeFlags_Framed))
				{
					_w1 = ofxImGuiSurfing::getWidgetsWidth(1); // 1 widget full width
					_w2 = ofxImGuiSurfing::getWidgetsWidth(2); // 2 widgets half width

					if (ImGui::Button("Half", ImVec2(_w2, _h))) { randomizeDurationBpm /= 2.f; }
					ImGui::SameLine();
					if (ImGui::Button("Double", ImVec2(_w2, _h))) { randomizeDurationBpm *= 2.f; }
					if (ImGui::Button("Reset", ImVec2(_w1, _h))) { randomizeDurationBpm = 120.f; }

					ofxImGuiSurfing::AddParameter(randomizeDurationBpm);
					ofxImGuiSurfing::AddParameter(randomizeDuration);
					ofxImGuiSurfing::AddParameter(randomizeDurationShort);
					ofxImGuiSurfing::AddParameter(randomizeDurationShortRatio);

					ImGui::TreePop();
				}
			}

			//-

			if (!bMinimize)
			{
				ofxImGuiSurfing::AddSpacingSeparated();

				// Probs
				ofxImGuiSurfing::AddGroup(params_PresetsProbs, ImGuiTreeNodeFlags_None);

				// Duration
				ofxImGuiSurfing::AddGroup(params_PresetDurations, ImGuiTreeNodeFlags_None);//closed

				ImGui::Spacing();

				ofxImGuiSurfing::AddBigButton(bResetProbs, _w1, _h);
			}
		}
#ifdef DEBUG_randomTest
		ImGui::Text("%d/%d", randomizedDice.get(), randomizedDice.getMax());
#endif
	}
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::doDices() // Calculate all probabilities for all presets
{
	// sum total dices/all probs
	dicesTotalAmount = 0;
	for (auto& p : presetsRandomFactor) {
		dicesTotalAmount += p.get();
	}
	randomizedDice.setMax(dicesTotalAmount - 1);

	ofLogNotice(__FUNCTION__) << group.getName() << " dicesTotalAmount: " << dicesTotalAmount;
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::Changed_Control(ofAbstractParameter& e)
{
	if (!DISABLE_CALLBACKS)
	{
		string name = e.getName();

		// Exclude log
		if ((name != bAvoidRepeatRand.getName()) &&
			(name != bRandomRunIndex.getName()) //&&
			//(name != "index") &&
			//(name != indexSelected.getName())
			)
		{
			ofLogNotice(__FUNCTION__) << "name: " << group.getName() << " " << name << ": " << e;
		}

		if (false) {}

		//----

		// Index preset selector
		else if (name == indexSelected.getName())
		{
			ofLogVerbose(__FUNCTION__) << group.getName() << " index: " << indexSelected.get();

			// TODO:
			//selectorTARGET = indexSelected;
		}

		//--

		// Randomizer
		else if (name == bRandomRunIndex.getName() && bRandomRunIndex)
		{
			//ofLogNotice(__FUNCTION__) << group.getName() << "RANDOMIZE !";

			bRandomRunIndex = false;
			doRandom();
		}

		// Play Randomizer
		else if (name == bPlay.getName())
		{
			//ofLogNotice(__FUNCTION__) << group.getName() << "MODE TIMER: " << e;

			if (bPlay)
			{
				//bEnableLatch = false;

				// TODO: new test
				randomizerTimer = ofGetElapsedTimeMillis();
			}
		}

		//// Latch
		//else if (name == bEnableLatch.getName())
		//{
		//	//ofLogNotice(__FUNCTION__) << "MODE LATCH: " << e;
		//	if (bEnableLatch) {
		//		bPlay = false;
		//	}
		//}

		// Durations
		else if (name == randomizeDuration.getName())
		{
			ofLogNotice(__FUNCTION__) << "DURATION: " << e;

			randomizeDurationBpm = (MAX_DURATION_RATIO * 60000.f) / randomizeDuration;
		}
		else if (name == randomizeDurationBpm.getName())
		{
			ofLogNotice(__FUNCTION__) << "BPM: " << e;

			// 60,000 ms (1 minute) / Tempo (BPM) = Delay Time in ms for quarter-note beats
			randomizeDuration = (MAX_DURATION_RATIO * 60000.f) / randomizeDurationBpm;
			randomizeDurationShort = randomizeDuration * randomizeDurationShortRatio;
		}
		else if (name == randomizeDurationShortRatio.getName())
		{
			ofLogNotice(__FUNCTION__) << "Short ratio: " << e;
			randomizeDurationShort = randomizeDuration * randomizeDurationShortRatio;
		}
		else if (name == randomizeDurationShort.getName())
		{
			ofLogNotice(__FUNCTION__) << "Lock Short ratio: " << e;
			randomizeDurationShort = randomizeDuration * randomizeDurationShortRatio;
		}

#ifdef DEBUG_randomTest
		else if (name == "DICE")// when debug enabled: set dice by user to test
		{
			ofLogNotice(__FUNCTION__) << "DICE: " << e;
			doRandom();
		}
#endif
		else if (name == bResetProbs.getName() && bResetProbs)
		{
			ofLogNotice(__FUNCTION__) << "Reset Probs";
			bResetProbs = false;

			doReset();
		}

		//--

		// All other widgets/params that are populate on the thw fly, not hardcoded
		else
		{
			// check if changed prob sliders
			bool _doDices = false;
			for (int i = 0; i < presetsRandomFactor.size(); i++)
			{
				if (name == "Prob " + ofToString(i)) {
					ofLogNotice(__FUNCTION__) << name << " : " << e;
					_doDices = true;// TODO: would be faster making return on first 'true'
				}
			}
			if (_doDices) doDices();
		}
	}
}

//--------------------------------------------------------------
void SurfingIndexRandomizer::loadPreset(int p)
{
	ofLogVerbose(__FUNCTION__) << group.getName() << " : " << p;

	indexSelected = p;
	bIsDoneLoad = true;// TODO: workaround bc the done-loading happens out of the class..

	// TODO:
	////if (selectorTARGET) 
	//{
	//selectorTARGET = indexSelected;
	//}
}
