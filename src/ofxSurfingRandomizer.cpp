#include "ofxSurfingRandomizer.h"

//--------------------------------------------------------------
ofxSurfingRandomizer::ofxSurfingRandomizer() {

	ofAddListener(ofEvents().draw, this, &ofxSurfingRandomizer::draw);
	ofAddListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingRandomizer::keyPressed);

	path_Editor = path_Global + "SurfingRandom_Ranges.json";
	path_AppState = path_Global + "SurfingRandom_AppSession.json";
	path_MemoryState = path_Global + "SurfingRandom_MemoryState.json";

	bGui.set("SURFING RND", true);
	bGui_Editor.set("RND RANGE", true);
	bGui_Params.set("RND PARAMS", true);
	bGui_Index.set("RND INDEX", true);

	params_AppState.setName("ofxSurfingRandomizer");
	params_AppState.add(bGui);
	params_AppState.add(bKeys.set("Keys", true));
	params_AppState.add(bHelp.set("Help", false));
	params_AppState.add(bGui_Params);
	params_AppState.add(bGui_Editor);
	params_AppState.add(bGui_Index);
	params_AppState.add(bMinimal);
	params_AppState.add(bPLAY.set("PLAY", false));
	params_AppState.add(bTarget.set("Target", false));
	params_AppState.add(playSpeed.set("Speed", 0.5, 0, 1));
	params_AppState.add(surfingIndexRandomizer.params_Clicker);

	bPLAY.setSerializable(false);
}

//--------------------------------------------------------------
ofxSurfingRandomizer::~ofxSurfingRandomizer() {
	ofRemoveListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	ofRemoveListener(ofEvents().draw, this, &ofxSurfingRandomizer::draw);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingRandomizer::keyPressed);

	exit();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::setup(ofParameterGroup& group) {
	//ofAddListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	//ofAddListener(ofEvents().draw, this, &ofxSurfingRandomizer::draw, OF_EVENT_ORDER_AFTER_APP);

	ofxSurfingHelpers::CheckFolder(path_Global);

	params = group; // store the external target params

	//setupEditor(group); // build the editor for all the params

	//--

#ifdef INCLUDE__OFX_UNDO_ENGINE
	undoManger.setPathGlobal(path_Global);
	undoManger.setup(params);
	//params_AppState.add(undoManger.getParamsAppState());
#endif

	//--

	// Gui

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	//guiManager.setImGuiAutodraw(bAutoDraw);
	//guiManager.setup();

	// The main user control
	guiManager.setSettingsPathLabel("ofxSurfingRandomizer");
	guiManager.setAutoSaveSettings(true);
	guiManager.setup(IM_GUI_MODE_INSTANTIATED);
#endif

	//--

	// Index randomizer

	if (!bCustomIndex) {
		const int NUM_INDEX_ITEMS = 9;
		indexTarget.set("Index", 0, 0, NUM_INDEX_ITEMS - 1);
	}

	//surfingIndexRandomizer.indexSelected.makeReferenceTo(indexTarget);// TODO: link
	//surfingIndexRandomizer.setup(indexTarget.getMax());
	//surfingIndexRandomizer.setBoolGui(bGui_Index);
	////surfingIndexRandomizer.setBoolGuiPtr(bGui_Index);

	surfingIndexRandomizer.setPath(path_Global);
	surfingIndexRandomizer.setup(indexTarget, bGui_Index);

	//-

	setupEditor(group); // Build the editor for all the params
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_RangeEditor() {

	// window editor
	ImGuiWindowFlags _flagsw;
	std::string n;

	bool bOpen;
	ImGuiColorEditFlags _flagc;

	// widgets sizes
	float _w100;
	float _w50;
	float _w33;
	float _w25;
	float _h;

	_flagsw = ImGuiWindowFlags_None;

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
#endif

	//--

	if (bGui_Editor)
	{
		guiManager.beginWindow(bGui_Editor, _flagsw);
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

			ofxImGuiSurfing::AddToggleRoundedButton(bMinimal);

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
			flags |= ImGuiTreeNodeFlags_Framed;

			//ofxImGuiSurfing::AddGroup(params_EditorGroups, flags);

			bool bOpen = true;
			ImGuiColorEditFlags _flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
			_flagw |= ImGuiTreeNodeFlags_Framed;

			// sliders props

			const int _ww1 = 60;
			const int _ww2 = 200;

			ImGuiSliderFlags sflag = ImGuiSliderFlags_None;
			//sflag |= ImGuiSliderFlags_ReadOnly;

			std::string spcl = "    ";//space between min-max range slider label

			//--

			bOpen = true;
			_flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
			_flagw |= ImGuiTreeNodeFlags_Framed;

			if (ImGui::TreeNodeEx("RANGES", _flagw))
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

				//for (int i = 0; i < params_EditorGroups.size(); i++) 
				for (int i = 0; i < enablersForParams.size(); i++)
				{
					if (enablersForParams[i].get() == false) continue;//akip and don't draw disbled params

					//numerize
					std::string tag;//to push ids
					std::string n = "#" + ofToString(i < 10 ? "0" : "") + ofToString(i);
					//ImGui::Dummy(ImVec2(0,10));

					ImGui::AlignTextToFramePadding();
					ImGui::Text(n.c_str());
					ImGui::SameLine();

					std::string _name = enablersForParams[i].getName();
					auto g = params_EditorGroups.getGroup(_name);

					auto &p0 = g.get(_name);

					auto type = p0.type();
					bool isFloat = type == typeid(ofParameter<float>).name();
					bool isInt = type == typeid(ofParameter<int>).name();
					bool isBool = type == typeid(ofParameter<bool>).name();

					//-

					if (isFloat)
					{
						auto &_p0 = g.getFloat(_name);
						auto &pmin = g.getFloat("Min");
						auto &pmax = g.getFloat("Max");
						float speed = (pmax - pmin) / 100.f;

						// 0. button random
						tag = n + "random";
						ImGui::PushID(tag.c_str());
						//required to allow same name in all buttons
						if (ImGui::Button("RND"))
						{
							doRandomize(i, true);
						}
						ImGui::PopID();
						ImGui::SameLine();

						// 1. value
						tag = n + "value";
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(_ww1);
						ImGui::DragFloat("", (float*)&_p0.get(), speed, pmin.get(), pmax.get(), "%.3f", sflag);
						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::SameLine();

						//ImGui::PushItemWidth(_ww2);
						//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();

						//-

						// get from min max

						//ImGui::Dummy(ImVec2(10, 0));

						if (!bMinimal) {
							ImGui::AlignTextToFramePadding();
							//ImGui::Text("GET");
							//ImGui::SameLine();
							// 0. MIN-MAX
							tag = n + "getMIN";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("GETMIN"))
							{
								_p0 = pmin;
							}
							ImGui::PopID();
							ImGui::SameLine(0, 1);
							tag = n + "getMAX";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("GETMAX"))
							{
								_p0 = pmax;
							}
							ImGui::PopID();
						}

						ImGui::SameLine();

						//-

						// set to min max

						//ImGui::Dummy(ImVec2(10, 0));
						if (!bMinimal) {

							ImGui::AlignTextToFramePadding();
							//ImGui::Text("SET");
							//ImGui::SameLine();
							// 0. MIN-MAX
							tag = n + "setMIN";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("SETMIN"))
							{
								pmin = _p0;
							}
							ImGui::PopID();
							ImGui::SameLine(0, 1);

							tag = n + "setMAX";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("SETMAX"))
							{
								pmax = _p0;
							}
							ImGui::PopID();
						}


						if (bMinimal) {
							ImGui::SameLine();
							ImGui::Text("RANGE");
						}
						ImGui::SameLine();

						// 2. Range
						//vanilla
						tag = n + "dragMin";
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(_ww1 * 2);
						ImGui::DragFloatRange2(""/*_name.c_str()*/, (float*)&pmin.get(), (float*)&pmax.get(), speed, pmin.getMin(), pmin.getMax(), "%.3f", "%.3f");
						ImGui::PopItemWidth();
						ImGui::PopID();

						if (!bMinimal) {
							ImGui::SameLine();
							//ImGui::SameLine(0, 0);//not works

							//widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(_ww2);
							std::string ss = "%.3f" + spcl + "%.3f";
							ofxImGuiSurfing::AddRangeParam(_name, pmin, pmax, ss.c_str(), power);
							//ofxImGuiSurfing::AddRangeParam("%.3f   %.3f", pmin, pmax, ss.c_str(), power);
							ImGui::PopItemWidth();
							ImGui::PopID();
						}

						//-

						//ImGui::PushItemWidth(_ww2);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();
						//ImGui::Columns(1);
					}

					//-

					else if (isInt)
					{
						auto &_p0 = g.getInt(_name);
						auto &pmin = g.getInt("Min");
						auto &pmax = g.getInt("Max");
						float speed = 1;

						// 0. button random
						tag = n + "random";
						ImGui::PushID(tag.c_str());
						if (ImGui::Button("RND"))
						{
							doRandomize(i, true);
						}
						ImGui::PopID();
						ImGui::SameLine();

						// 1. value
						tag = n + "value";
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(_ww1);
						ImGui::DragInt("", (int*)&_p0.get(), speed, pmin.get(), pmax.get(), "%.0f", sflag);
						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::SameLine();

						//ImGui::PushItemWidth(_ww2);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();
						//ImGui::SameLine();

						//-

						// get from min max

						if (!bMinimal) {
							ImGui::AlignTextToFramePadding();
							//ImGui::Text("GET");
							//ImGui::SameLine();

							// 0. MIN-MAX
							tag = n + "getMIN";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("GETMIN"))
							{
								_p0 = pmin;
							}
							ImGui::PopID();
							ImGui::SameLine(0, 1);

							tag = n + "getMAX";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("GETMAX"))
							{
								_p0 = pmax;
							}
							ImGui::PopID();
						}

						ImGui::SameLine();

						//-

						// set to min max
						if (!bMinimal) {
							ImGui::AlignTextToFramePadding();
							//ImGui::Text("SET");
							//ImGui::SameLine();

							// 0. MIN-MAX
							tag = n + "setMIN";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("SETMIN"))
							{
								pmin = _p0;
							}
							ImGui::PopID();
							ImGui::SameLine(0, 1);

							tag = n + "setMAX";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("SETMAX"))
							{
								pmax = _p0;
							}
							ImGui::PopID();
						}

						if (bMinimal) {
							ImGui::Text("RANGE");
							ImGui::SameLine();
						}


						// 2. Range
						// vanilla
						ImGui::SameLine();
						tag = n + "dragMin";
						ImGui::NextColumn();
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(_ww1 * 2);
						ImGui::DragIntRange2(""/*_name.c_str()*/, (int*)&pmin.get(), (int*)&pmax.get(), speed, pmin.getMin(), pmin.getMax(), "%.0f", "%.0f");
						ImGui::PopItemWidth();
						ImGui::PopID();

						if (!bMinimal) {
							ImGui::SameLine();
							//ImGui::SameLine(0, 0);
							
							//widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(_ww2);
							std::string ss = "%.0f" + spcl + "%.0f";
							ofxImGuiSurfing::AddRangeParam(_name, pmin, pmax, ss.c_str(), power);
							//ofxImGuiSurfing::AddRangeParam("%.0f   %.0f", pmin, pmax, ss.c_str(), power);
							ImGui::PopItemWidth();
							ImGui::PopID();
						}

						//-

						//ImGui::PushItemWidth(_ww2);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();
						//ImGui::Columns(1);
					}

					//-

					else if (isBool)
					{
						// 0. button random
						tag = n + "random";
						ImGui::NextColumn();
						ImGui::PushID(tag.c_str());
						if (ImGui::Button("RND"))
						{
							doRandomize(i, true);
						}
						ImGui::PopID();
						ImGui::SameLine();

						auto &_p0 = g.getBool(_name);
						ofxImGuiSurfing::AddParameter(_p0);
					}
				}

				//-

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
				if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
#endif

				ImGui::TreePop();
			}

			//--

			// Resets

			if (!bMinimal) {
				bOpen = false;
				_flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagw |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::TreeNodeEx("RESETS", _flagw))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					if (ImGui::Button("PARAMS TO PARAM MIN", ImVec2(_w50, _h)))
					{
						doResetParams(RESET_PARAM_MIN);
					}
					ImGui::SameLine();
					if (ImGui::Button("PARAMS TO PARAM MAX", ImVec2(_w50, _h)))
					{
						doResetParams(RESET_PARAM_MAX);
					}

					if (ImGui::Button("PARAMS TO RANGE MIN", ImVec2(_w50, _h)))
					{
						doResetParams(RESET_RANGE_MIN);
					}
					ImGui::SameLine();
					if (ImGui::Button("PARAMS TO RANGE MAX", ImVec2(_w50, _h)))
					{
						doResetParams(RESET_RANGE_MAX);
					}

					if (ImGui::Button("RANGES TO PARAMS LIMITS FULL", ImVec2(_w50, _h)))
					{
						doResetRangesFull();
					}
					ImGui::SameLine();
					if (ImGui::Button("RANGES TO PARAMS LIMITS HALF", ImVec2(_w50, _h)))
					{
						doResetRangesHalf();
					}

					ImGui::TreePop();
				}
			}
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Widgets() {

	if (!bGui) return;

	{
		drawImGui_Main();
		drawImGui_RangeEditor();
		drawImGui_Params();
		drawImGui_Index();

#ifdef INCLUDE__OFX_UNDO_ENGINE
		undoManger.drawImGui();
#endif
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Main() {

	ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
	std::string n;

	bool bOpen;
	ImGuiColorEditFlags _flagc;

	// widgets sizes
	float _w100;
	float _w50;
	float _w33;
	float _w25;
	float _h;
	float _spcx = ImGui::GetStyle().ItemSpacing.x;

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	if (guiManager.bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
#endif

	//----

	// main panel

	if (bGui)
	{
		guiManager.beginWindow(bGui, _flagsw);
		{
			//-

			// commands
			{
				bOpen = true;
				_flagc = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
				if (ImGui::CollapsingHeader("COMMANDS", _flagc))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					if (ImGui::Button("RND PARAMS", ImVec2(_w100, 3 * _h)))
					{
						doRandomize();
					}
					if (ImGui::Button("RND INDEX", ImVec2(_w100, 3 * _h)))
					{
						surfingIndexRandomizer.doRandom();
					}

					ImGui::Spacing();

					//-

					// tester

					bOpen = false;
					ImGuiColorEditFlags _flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
					_flagw |= ImGuiTreeNodeFlags_Framed;
					ImGui::Indent();
					if (ImGui::TreeNodeEx("TESTER", _flagw))
					{
						ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

						ofxImGuiSurfing::AddBigToggleNamed(bTarget, _w100, _h, "Params", "Index");
						ofxImGuiSurfing::AddBigToggleNamed(bPLAY, _w100, 2 * _h, "TESTING RND", "TEST RND", true, 1 - tn);

						//if (ImGui::Button("RANDOMIZE", ImVec2(_w100, _h))) {
						//	doRandomize();
						//}

						if (bPLAY)
						{
							ofxImGuiSurfing::ProgressBar2(tn);
							ImGui::PushItemWidth(_w50);
							ofxImGuiSurfing::AddParameter(playSpeed);
							//ImGui::SliderFloat("Speed", &playSpeed, 0, 1);
							ImGui::PopItemWidth();
						}

						ImGui::TreePop();
					}
					ImGui::Unindent();
				}
			}

			//-

			// Panels
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

				bOpen = true;
				_flagc = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
				if (ImGui::CollapsingHeader("PANELS", _flagc))
				{
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);
					ImGui::Indent();
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Params);
					//if(bGui_Editor) ofxImGuiSurfing::AddToggleRoundedButton(bGui_Params);
					ImGui::Unindent();

					ImGui::Separator();

					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Index);
					//if (bGui_Index) ofxImGuiSurfing::AddParameter(indexTarget);
					ofxImGuiSurfing::AddParameter(indexTarget);
					ofxImGuiSurfing::AddBigToggleNamed(surfingIndexRandomizer.bPLAY, _w100, _h, "PLAYING RND", "PLAY RND", true, 1 - surfingIndexRandomizer.getPlayerPct());

#ifdef INCLUDE__OFX_UNDO_ENGINE
					ofxImGuiSurfing::AddToggleRoundedButton(undoManger.bGui_UndoEngine);
#endif
				}
			}

			//-

			// enable toggles
			{
				bOpen = true;
				_flagc = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);

				if (ImGui::CollapsingHeader("ENABLE PARAMS", _flagc))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					static bool bNone, bAll;
					if (ImGui::Button("NONE", ImVec2(_w50, _h)))
					{
						doDisableAll();
					}
					ImGui::SameLine();
					if (ImGui::Button("ALL", ImVec2(_w50, _h)))
					{
						doEnableAll();
					}
					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					for (int i = 0; i < params_EditorEnablers.size(); i++)
					{
						auto &p = params_EditorEnablers[i];// ofAbstractParameter
						auto type = p.type();
						bool isBool = type == typeid(ofParameter<bool>).name();
						//bool isGroup = type == typeid(ofParameterGroup).name();
						//bool isFloat = type == typeid(ofParameter<float>).name();
						//bool isInt = type == typeid(ofParameter<int>).name();
						std::string name = p.getName();

						if (isBool)//just in case... 
						{
							// 1. toggle enable
							ofParameter<bool> pb = p.cast<bool>();
							float _w75 = 3 * (_w100 / 4 - _spcx / 4);
							ofxImGuiSurfing::AddBigToggle(pb, _w75, _h, false);
							ImGui::SameLine();
							// 2. button random
							ImGui::PushID(i);//required to allow same name in all buttons
							if (ImGui::Button("RND", ImVec2(_w25, _h)))
							{
								doRandomize(i, true);
							}
							ImGui::PopID();

							//TODO:
							//auto tmpRef = pb.get();
							//const auto& info = typeid(ParameterType); 
							//if (ImGui::Checkbox(GetUniqueName(parameter), (bool *)&tmpRef))
							//{
							//	parameter.set(tmpRef);
							//	return true;
							//}
						}
					}
				}
			}

			//-

			// TOOLS
			{
				bOpen = false;
				ImGuiColorEditFlags _flagw;

				bOpen = false;
				_flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagw |= ImGuiTreeNodeFlags_Framed;

				// State memory
				if (ImGui::TreeNodeEx("MEMORY", _flagw))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					if (ImGui::Button("STORE", ImVec2(_w50, _h)))
					{
						doSaveState();
					}
					ImGui::SameLine();
					if (ImGui::Button("RECALL", ImVec2(_w50, _h)))
					{
						doLoadState();
					}

					ImGui::TreePop();
				}
			}

			//-

			//TODO:
			//// simple ranges
			//for (int i = 0; i < params_EditorEnablers.size(); i++)
			//{
			//	auto &p = params_EditorEnablers[i];// ofAbstractParameter
			//	auto type = p.type();
			//	bool isBool = type == typeid(ofParameter<bool>).name();
			//	std::string name =
			//		ofxImGui::AddRange("range", parameterMin, parameterMax, speed);
			//}

			//-

			ofxImGuiSurfing::AddToggleRoundedButton(bKeys);
			ofxImGuiSurfing::AddToggleRoundedButton(bHelp);

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
			ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);
			guiManager.drawAdvancedSubPanel(false);
#endif
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::update(ofEventArgs & args) {

	// Tester
	// Play timed randoms
	static const int _secs = 2;
	if (bPLAY)
	{
		//int max = 60 * _secs;
		int max = ofMap(playSpeed, 0, 1, 60, 5) * _secs;
		tf = ofGetFrameNum() % max;
		tn = ofMap(tf, 0, max, 0, 1);
		if (tf == 0)
		{
			// Target Selector
			if (!bTarget) surfingIndexRandomizer.doRandom();
			else doRandomize();
		}
	}

	//-

	// Randomizers group
	surfingIndexRandomizer.update();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::draw_ImGui() {
	if (!bGui) return;

	//----

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	guiManager.begin();
	{
		drawImGui_Widgets();
	}
	guiManager.end();
#endif
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::draw(ofEventArgs & args) {

	if (!bGui) return;

	if (bHelp) drawHelp();

	//drawImGui();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawHelp() {

	//ofxSurfingHelpers::drawTextBoxed();
	//ofDrawBitmapStringHighlight(helpInfo, 0, 0);
	textBoxWidget.draw();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Index() {
	ImGuiWindowFlags _flagsw;
	_flagsw = ImGuiWindowFlags_None;

	if (bGui_Index)
	{
		surfingIndexRandomizer.drawImGui();

		//guiManager.beginWindow(bGui_Index, _flagsw);
		//{
		//	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		//	flags |= ImGuiTreeNodeFlags_DefaultOpen;
		//	flags |= ImGuiTreeNodeFlags_Framed;
		//	//ofxImGuiSurfing::AddGroup(params, flags);
		//	{
		//		surfingIndexRandomizer.drawImGui();
		//	}
		//}
		//guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Params() {
	ImGuiWindowFlags _flagsw;
	_flagsw = ImGuiWindowFlags_None;

	static ofParameter<bool> _bAutoResize{ "Auto Resize",true };
	if (_bAutoResize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

	if (bGui_Params)
	{
		guiManager.beginWindow(bGui_Params, _flagsw);
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
			flags |= ImGuiTreeNodeFlags_Framed;
			ofxImGuiSurfing::AddGroup(params, flags);

			ImGui::Dummy(ImVec2(0.0f, 2.0f));
			ofxImGuiSurfing::AddToggleRoundedButton(_bAutoResize);
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doSetAll(bool b) {
	ofLogNotice(__FUNCTION__) << b;

	for (int i = 0; i < params_EditorEnablers.size(); i++)
	{
		auto &p = params_EditorEnablers[i];//ofAbstractParameter
		auto type = p.type();
		bool isBool = type == typeid(ofParameter<bool>).name();
		std::string name = p.getName();

		if (isBool) {
			ofParameter<bool> pb = p.cast<bool>();
			pb.set(b);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doDisableAll() {
	doSetAll(false);
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doEnableAll() {
	doSetAll(true);
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doResetParams(ResetPramsType type) {
	ofLogNotice(__FUNCTION__);

	for (auto p : enablersForParams)
	{
		if (!p.get()) continue;//only reset this param if it's enabled

		//-

		std::string name = p.getName();//name
		auto &g = params_EditorGroups.getGroup(name);//ofParameterGroup
		auto &e = g.get(name);//ofAbstractParameter

		auto ptype = e.type();
		bool isFloat = ptype == typeid(ofParameter<float>).name();
		bool isInt = ptype == typeid(ofParameter<int>).name();

		if (isFloat)
		{
			auto pmin = g.getFloat("Min").get();
			auto pmax = g.getFloat("Max").get();
			ofParameter<float> p0 = e.cast<float>();

			if (0) {}
			else if (type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
			else if (type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
			else if (type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
			else if (type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
		}

		else if (isInt)
		{
			auto pmin = g.getInt("Min").get();
			auto pmax = g.getInt("Max").get();
			ofParameter<int> p0 = e.cast<int>();

			if (0) {}
			else if (type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
			else if (type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
			else if (type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
			else if (type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doResetRangesFull() {
	ofLogNotice(__FUNCTION__);

	for (auto p : enablersForParams)
	{
		if (!p.get()) continue;//only reset this param if it's enabled

		//-

		std::string name = p.getName();//name
		auto &g = params_EditorGroups.getGroup(name);//ofParameterGroup
		auto &e = g.get(name);//ofAbstractParameter

		auto type = e.type();
		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();

		if (isFloat)
		{
			ofParameter<float> p0 = e.cast<float>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<float>();
			auto _pmax = g.get("Max").cast<float>();
			//reset to abs min
			_pmin.set(pmin);
			_pmax.set(pmax);
		}

		else if (isInt)
		{
			ofParameter<int> p0 = e.cast<int>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<int>();
			auto _pmax = g.get("Max").cast<int>();
			//reset to abs min
			_pmin.set(pmin);
			_pmax.set(pmax);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doResetRangesHalf() {
	ofLogNotice(__FUNCTION__);

	for (auto p : enablersForParams)
	{
		if (!p.get()) continue;//only reset this param if it's enabled

		float _ratio = 0.25f;//to close a bit

		//-

		std::string name = p.getName();//name
		auto &g = params_EditorGroups.getGroup(name);//ofParameterGroup
		auto &e = g.get(name);//ofAbstractParameter

		auto type = e.type();
		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();

		if (isFloat)
		{
			ofParameter<float> p0 = e.cast<float>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<float>();
			auto _pmax = g.get("Max").cast<float>();
			//reset to abs min a bit closed
			float w = (pmax - pmin) * _ratio;
			_pmin.set(pmin + w);
			_pmax.set(pmax - w);
		}

		else if (isInt)
		{
			ofParameter<int> p0 = e.cast<int>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<int>();
			auto _pmax = g.get("Max").cast<int>();
			//reset to abs min a bit closed
			float w = (pmax - pmin) * _ratio;
			_pmin.set(pmin + w);
			_pmax.set(pmax - w);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doRandomize() {
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < enablersForParams.size(); i++)
	{
		doRandomize(i, false);
	}

	//--

#ifdef INCLUDE__OFX_UNDO_ENGINE
	// worfklow
	// store current point to undo history
	undoManger.doStoreUndoWhenAuto();
	//if (undoManger.bAutoStore) undoManger.doStoreUndo();
#endif

}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doRandomize(int index, bool bForce) {
	ofLogVerbose(__FUNCTION__) << index;

	int i = index;

	//for (auto p : enablersForParams)
	//for (int i = 0; i<enablersForParams.size(); i++)
	{
		auto p = enablersForParams[i];

		if (!bForce)
			if (!p.get()) return;//only reset this param if it's enabled

		//-

		std::string name = p.getName();//name
		auto &g = params_EditorGroups.getGroup(name);//ofParameterGroup
		auto &e = g.get(name);//ofAbstractParameter

		auto type = e.type();
		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();
		bool isBool = type == typeid(ofParameter<bool>).name();

		if (isFloat)
		{
			auto pmin = g.getFloat("Min").get();
			auto pmax = g.getFloat("Max").get();
			ofParameter<float> p0 = e.cast<float>();
			p0.set((float)ofRandom(pmin, pmax));//random
		}

		else if (isInt)
		{
			auto pmin = g.getInt("Min").get();
			auto pmax = g.getInt("Max").get();
			ofParameter<int> p0 = e.cast<int>();
			p0.set((int)ofRandom(pmin, pmax + 1));//random
		}

		else if (isBool)
		{
			bool b = (ofRandom(0, 2) >= 1);
			ofParameter<bool> p0 = e.cast<bool>();
			p0.set(b);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doSaveState() {
	ofLogNotice(__FUNCTION__);
	ofxSurfingHelpers::saveGroup(params, path_MemoryState);
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doLoadState() {
	ofLogNotice(__FUNCTION__);
	ofxSurfingHelpers::loadGroup(params, path_MemoryState);
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::exit() {

	// settings
	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);
	ofxSurfingHelpers::saveGroup(params_Editor, path_Editor);

	//--

//#ifdef INCLUDE__OFX_UNDO_ENGINE
//	undoManger.exit();
//#endif
}

//--

//--------------------------------------------------------------
void ofxSurfingRandomizer::setupEditor(ofParameterGroup& group)
{
	// Clear
	params_EditorGroups.clear();// a group for each param
	params_EditorGroups.setName(params.getName());

	enablersForParams.clear();// an enabler toggler for each param
	params_EditorEnablers.clear();// an enabler toggler for each param
	params_EditorEnablers.setName("Params");

	// Initiate
	addGroup(group);

	//-

	// Settings
	params_Editor.setName("Ranges Editor");
	params_Editor.add(params_EditorEnablers);
	params_Editor.add(params_EditorGroups);
	ofxSurfingHelpers::loadGroup(params_Editor, path_Editor);

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	//-

	// Help
	std::string helpInfo = "";
	helpInfo += "ofxSurfingRandomizer\n";
	helpInfo += "\n";
	helpInfo += "HELP\n";
	helpInfo += "KEY COMMANDS\n";
	helpInfo += "\n";
	helpInfo += "SPACE      : Rnd Params\n";
	helpInfo += "BACKSPACE  : Reset params\n";
	helpInfo += "Ctrl+SPACE : Rnd Index\n";
	helpInfo += "LEFT-RIGHT : Browse Index\n";
	helpInfo += "RETURN     : Play\n";
	helpInfo = ofToUpper(helpInfo);//make uppercase

	textBoxWidget.setText(helpInfo);
	textBoxWidget.setup();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::addGroup(ofParameterGroup& group) {

	for (auto pa : group)//ofAbstractParameter
	{
		//// exclude params not marked as serializable
		//if (!pa->isSerializable()) continue;
		////if (!parameter->isSerializable()) return;

		//--

		// different types
		auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(pa);// group
		auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(pa);
		auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(pa);
		auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(pa);

		//auto type = pa->type();
		//bool isFloat = type == typeid(ofParameter<float>).name();
		//bool isInt = type == typeid(ofParameter<int>).name();
		//bool isBool = type == typeid(ofParameter<bool>).name();

		//---

		//if (false) {}

		// group
		if (parameterGroup)
		{
			//if (!parameterGroup->isSerializable()) continue;
			////if (!parameterGroup->isSerializable()) return;

			//TODO:
			//bug: multiple nested..
			addGroup(*parameterGroup);

			//auto g0 = group.getGroup(parameterGroup->getName());
			//addGroup(g0);

			continue;
		}

		//-

		// float
		else if (parameterFloat)
		{
			std::string _name = parameterFloat->getName();

			float vtot = parameterFloat->getMax() - parameterFloat->getMin();
			float vmin = parameterFloat->getMin() + DEFAULT_MIN_PCT * vtot;
			float vmax = parameterFloat->getMin() + DEFAULT_MAX_PCT * vtot;

			ofParameterGroup _g{ _name };
			//ofParameter<bool> b{ "Enable " + _name, false };
			ofParameter<float> p0 = group.getFloat(_name);
			ofParameter<float> pmin{ "Min", vmin, parameterFloat->getMin() , parameterFloat->getMax() };
			ofParameter<float> pmax{ "Max", vmax, parameterFloat->getMin() , parameterFloat->getMax() };

			//_g.add(b);
			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);

			//-

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			continue;
		}

		//-

		// int
		else if (parameterInt)
		{
			std::string _name = parameterInt->getName();

			int vtot = parameterInt->getMax() - parameterInt->getMin();
			int vmin = parameterInt->getMin() + DEFAULT_MIN_PCT * vtot;
			int vmax = parameterInt->getMin() + DEFAULT_MAX_PCT * vtot;

			ofParameterGroup _g{ _name };
			//ofParameter<bool> b{ "Enable " + _name, false };
			ofParameter<int> p0 = group.getInt(_name);
			ofParameter<int> pmin{ "Min", vmin, parameterInt->getMin() , parameterInt->getMax() };
			ofParameter<int> pmax{ "Max", vmax, parameterInt->getMin() , parameterInt->getMax() };

			//_g.add(b);
			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);

			//-

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			continue;
		}

		//-

		// bool
		else if (parameterBool && 1)
		{
			std::string _name = parameterBool->getName();

			ofParameterGroup _g{ _name };
			//ofParameter<bool> b{ "Enable " + _name, false };
			ofParameter<bool> p0 = group.getBool(_name);

			//_g.add(b);
			_g.add(p0);

			params_EditorGroups.add(_g);

			//-

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			continue;
		}

		ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << pa->getName();
	}

	//----

	//TODO:
	//// create an enabler bool/toggle for each parameter
	//for (auto pb : enablersForParams)
	//{
	//	params_EditorEnablers.add(pb);
	//}
}


//--------------------------------------------------------------
void ofxSurfingRandomizer::keyPressed(ofKeyEventArgs &eventArgs) {

	if (!bKeys) return;

	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	if (key == 'g') bGui = !bGui;

	// randomizer
	if (!mod_CONTROL && key == ' ') { doRandomize(); }
	if (mod_CONTROL && key == ' ') { (surfingIndexRandomizer.doRandom()); }
	if (key == OF_KEY_BACKSPACE) { doResetParams(); }
	if (key == OF_KEY_RETURN) bPLAY = !bPLAY;

	if (key == OF_KEY_LEFT) {
		if (indexTarget > 0) indexTarget.set(indexTarget.get() - 1);
		else if (indexTarget <= 0) indexTarget.set(indexTarget.getMax());
	}
	if (key == OF_KEY_RIGHT) {
		indexTarget.setWithoutEventNotifications(indexTarget.get() + 1);
		indexTarget = indexTarget % (indexTarget.getMax() + 1);
	}

	//--

	// Index randomizer
	surfingIndexRandomizer.keyPressed(key);

	//----

	// TODO: not working on windows.. We need to add int code
#ifdef INCLUDE__OFX_UNDO_ENGINE
	undoManger.keyPressed(eventArgs);
#endif

	//----
}
