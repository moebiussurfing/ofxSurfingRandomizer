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
	bGui_RangesEditor.set("RND RANGES", true);
	bGui_Params.set("RND PARAMS", true);
	bGui_Index.set("RND INDEX", true);

	params_AppState.setName("ofxSurfingRandomizer");
	params_AppState.add(bGui);
	params_AppState.add(bGui_Params);
	params_AppState.add(bGui_RangesEditor);
	params_AppState.add(bGui_Index);
	params_AppState.add(bMinimal);
	params_AppState.add(bPLAY.set("PLAY", false));
	params_AppState.add(bKeys.set("Keys", true));
	params_AppState.add(bHelp.set("Help", false));
	params_AppState.add(bTarget.set("Target", false));
	params_AppState.add(playSpeed.set("Speed", 0.5, 0, 1));
	params_AppState.add(surfingIndexRandomizer.params_Clicker);

	// exclude
	//bPLAY.setSerializable(false);
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

	ofxSurfingHelpers::CheckFolder(path_Global);

	params = group; // store the external target params

	//--

#ifdef INCLUDE__OFX_UNDO_ENGINE
	undoManger.setPathGlobal(path_Global);
	undoManger.setup(params);
	//params_AppState.add(undoManger.getParamsAppState());
#endif

	//--

	// Gui

	guiManager.setSettingsPathLabel("ofxSurfingRandomizer");
	guiManager.setup(IM_GUI_MODE_INSTANTIATED);

	//--

	// Index randomizer

	if (!bCustomIndex) {
		const int NUM_INDEX_ITEMS = 9;
		indexTarget.set("Index", 0, 0, NUM_INDEX_ITEMS - 1);

		surfingIndexRandomizer.setPath(path_Global);
		surfingIndexRandomizer.setup(indexTarget, bGui_Index);
	}

	//-

	setupEditor(group); // Build the range editor for all the params.

	//-

	startup();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_RangeEditor() {

	std::string n;

	bool bOpen;
	ImGuiColorEditFlags _flagc;
	ImGuiColorEditFlags _flagw;

	// widgets sizes
	float _w100;
	float _w50;
	float _w33;
	float _w25;
	float _h;

	//--

	if (bGui_RangesEditor)
	{
		guiManager.beginWindow(bGui_RangesEditor);
		{
			ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

			ofxImGuiSurfing::AddToggleRoundedButton(bMinimal);

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
			flags |= ImGuiTreeNodeFlags_Framed;

			//ofxImGuiSurfing::AddGroup(params_EditorGroups, flags);

			bool bOpen = true;
			_flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
			_flagw |= ImGuiTreeNodeFlags_Framed;

			// Sliders props

			ImGuiSliderFlags sflag = ImGuiSliderFlags_None;

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
					if (enablersForParams[i].get() == false) continue; // Skip and don't draw disbled (false) params

					//numerize
					std::string tag;//to push ids
					std::string n = "#" + ofToString(i < 10 ? "0" : "") + ofToString(i);

					ImGui::AlignTextToFramePadding();
					//ImGui::Text(n.c_str());
					//ImGui::SameLine();

					std::string _name = enablersForParams[i].getName();
					auto g = params_EditorGroups.getGroup(_name);

					auto &p0 = g.get(_name);

					auto type = p0.type();
					bool isFloat = type == typeid(ofParameter<float>).name();
					bool isInt = type == typeid(ofParameter<int>).name();
					bool isBool = type == typeid(ofParameter<bool>).name();

					bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
					bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
					bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

					//-

					if (0) {}

					else if (isBool)
					{
						ImGui::Text(n.c_str());
						ImGui::SameLine();

						// 0. Button random
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

					//-

					else if (isInt)
					{
						ImGui::Text(n.c_str());
						ImGui::SameLine();

						auto &_p0 = g.getInt(_name);
						auto &pmin = g.getInt("Min");
						auto &pmax = g.getInt("Max");
						float speed = 1;

						// 0. Button random
						tag = n + "random";
						ImGui::PushID(tag.c_str());
						if (ImGui::Button("RND"))
						{
							doRandomize(i, true);
						}
						ImGui::PopID();
						ImGui::SameLine();

						// 1. Value
						tag = n + "value";
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(WIDGET_R_DRAG);
						ImGui::DragInt("", (int*)&_p0.get(), speed, pmin.get(), pmax.get(), "%.0f", sflag);
						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::SameLine();

						//ImGui::PushItemWidth(WIDGET_R_SLIDER);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();
						//ImGui::SameLine();

						//-

						// Get from min max

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

						// Set to min max
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


						// 2. Range vanilla
						ImGui::SameLine();
						tag = n + "dragMin";
						ImGui::NextColumn();
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(WIDGET_R_DRAG * 2);
						ImGui::DragIntRange2(""/*_name.c_str()*/, (int*)&pmin.get(), (int*)&pmax.get(), speed, pmin.getMin(), pmin.getMax(), "%.0f", "%.0f");
						ImGui::PopItemWidth();
						ImGui::PopID();

						if (!bMinimal) {
							ImGui::SameLine();

							// 3. Range Surfing widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(WIDGET_R_SLIDER);
							std::string ss = "%.0f" + spcl + "%.0f";
							ofxImGuiSurfing::AddRangeParam(_name, pmin, pmax, ss.c_str(), power);
							//ofxImGuiSurfing::AddRangeParam("%.0f   %.0f", pmin, pmax, ss.c_str(), power);
							ImGui::PopItemWidth();
							ImGui::PopID();
						}

						//-

						//ImGui::PushItemWidth(WIDGET_R_SLIDER);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();
						//ImGui::Columns(1);
					}

					//-

					else if (isFloat)
					{
						ImGui::Text(n.c_str());
						ImGui::SameLine();

						auto &_p0 = g.getFloat(_name);
						auto &pmin = g.getFloat("Min");
						auto &pmax = g.getFloat("Max");
						float speed = (pmax - pmin) / 100.f;

						// 0. Button random
						tag = n + "random";
						ImGui::PushID(tag.c_str());
						//required to allow same name in all buttons
						if (ImGui::Button("RND"))
						{
							doRandomize(i, true);
						}
						ImGui::PopID();
						ImGui::SameLine();

						// 1. Value
						tag = n + "value";
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(WIDGET_R_DRAG);
						ImGui::DragFloat("", (float*)&_p0.get(), speed, pmin.get(), pmax.get(), "%.3f", sflag);
						ImGui::PopItemWidth();
						ImGui::PopID();
						ImGui::SameLine();

						//ImGui::PushItemWidth(WIDGET_R_SLIDER);
						//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();

						//-

						// Get from min max

						if (!bMinimal)
						{
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

						// Set to min max

						if (!bMinimal)
						{
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

						// 2. Range vanilla
						tag = n + "dragMin";
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(WIDGET_R_DRAG * 2);
						ImGui::DragFloatRange2(""/*_name.c_str()*/, (float*)&pmin.get(), (float*)&pmax.get(), speed, pmin.getMin(), pmin.getMax(), "%.3f", "%.3f");
						ImGui::PopItemWidth();
						ImGui::PopID();

						if (!bMinimal) {
							ImGui::SameLine();

							// 3. Range Surfing widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(WIDGET_R_SLIDER);
							std::string ss = "%.3f" + spcl + "%.3f";
							ofxImGuiSurfing::AddRangeParam(_name, pmin, pmax, ss.c_str(), power);
							//ofxImGuiSurfing::AddRangeParam("%.3f   %.3f", pmin, pmax, ss.c_str(), power);
							ImGui::PopItemWidth();
							ImGui::PopID();
						}

						//-

						//ImGui::PushItemWidth(WIDGET_R_SLIDER);
						//ofxImGuiSurfing::AddParameter(_p0);
						//ImGui::PopItemWidth();
						//ImGui::Columns(1);
					}

					//-

					else if (isVec2)
					{
						drawImGui_RangeEditorVecRow(i, 2);
						//i += 2;
					}

					else if (isVec3)
					{
						drawImGui_RangeEditorVecRow(i, 3);
						//i += 3;
					}

					else if (isVec4)
					{
						drawImGui_RangeEditorVecRow(i, 4);
						//i += 4;
					}
				}

				//-

				ImGui::TreePop();
			}

			//--

			// Resets

			if (!bMinimal)
			{
				drawImGui_RangeEditorResets();
			}
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_RangeEditorResets()
{
	bool bOpen;
	ImGuiColorEditFlags _flagc;
	ImGuiColorEditFlags _flagw;

	float _w100;
	float _w50;
	float _w33;
	float _w25;
	float _h;

	bOpen = false;
	_flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagw |= ImGuiTreeNodeFlags_Framed;

	if (ImGui::TreeNodeEx("RESETS", _flagw))
	{
		ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

		//-

		if (ImGui::Button("PARAMS TO PARAMS MIN", ImVec2(_w33, _h)))
		{
			doResetParamsFull(RESET_PARAM_MIN);
		}
		ImGui::SameLine();
		if (ImGui::Button("PARAMS TO PARAMS CENTER", ImVec2(_w33, _h)))
		{
			doResetParamsFull(RESET_PARAM_FULL_CENTER);
		}
		ImGui::SameLine();
		if (ImGui::Button("PARAMS TO PARAMS MAX", ImVec2(_w33, _h)))
		{
			doResetParamsFull(RESET_PARAM_MAX);
		}

		//-

		if (ImGui::Button("PARAMS TO RANGES MIN", ImVec2(_w33, _h)))
		{
			doResetParamsFull(RESET_RANGE_MIN);
		}
		ImGui::SameLine();
		if (ImGui::Button("PARAMS TO RANGES CENTER", ImVec2(_w33, _h)))
		{
			doResetParamsFull(RESET_PARAM_RANGE_CENTER);
		}
		ImGui::SameLine();
		if (ImGui::Button("PARAMS TO RANGES MAX", ImVec2(_w33, _h)))
		{
			doResetParamsFull(RESET_RANGE_MAX);
		}

		//-

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

	//----

	// Main panel

	if (bGui)
	{
		guiManager.beginWindow(bGui);
		{
			//-

			// Commands
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

					// Tester

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
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_RangesEditor);
					ImGui::Indent();
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Params);
					ImGui::Unindent();

					ImGui::Separator();

					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Index);
					ofxImGuiSurfing::AddParameter(indexTarget);
					ofxImGuiSurfing::AddBigToggleNamed(surfingIndexRandomizer.bPLAY, _w100, _h, "PLAYING RND", "PLAY RND", true, 1 - surfingIndexRandomizer.getPlayerPct());

#ifdef INCLUDE__OFX_UNDO_ENGINE
					ofxImGuiSurfing::AddToggleRoundedButton(undoManger.bGui_UndoEngine);
#endif
				}
			}

			//-

			// Enable toggles params

			if (bGui_RangesEditor)
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

					ImGui::Spacing();

					for (int i = 0; i < params_EditorEnablers.size(); i++)
					{
						auto &p = params_EditorEnablers[i]; // ofAbstractParameter
						auto type = p.type();
						bool isBool = type == typeid(ofParameter<bool>).name();

						std::string name = p.getName();

						if (isBool) // Just in case... 
						{
							//-

							// 1. Toggle enable

							ofParameter<bool> pb = p.cast<bool>();
							float _w75 = 3 * (_w100 / 4 - _spcx / 4);
							ofxImGuiSurfing::AddBigToggle(pb, _w75, _h, false);

							ImGui::SameLine();

							//-

							// 2. Random button 

							ImGui::PushID(i);//required to allow same name in all buttons
							if (ImGui::Button("RND", ImVec2(_w25, _h)))
							{
								doRandomize(i, true);
							}
							ImGui::PopID();

							//-

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

			// Tools
			{
				bOpen = false;
				ImGuiColorEditFlags _flagw;
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

			guiManager.drawAdvanced();
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

	guiManager.begin();
	{
		drawImGui_Widgets();
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::draw(ofEventArgs & args) {

	if (!bGui) return;

	if (bHelp) drawHelp();

	//drawImGui();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawHelp() {

	textBoxWidget.draw();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Index() {

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

	if (bGui_Params)
	{
		guiManager.beginWindow(bGui_Params);
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
			flags |= ImGuiTreeNodeFlags_Framed;

			ofxImGuiSurfing::AddGroup(params, flags);
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

//--

// Resets

//--------------------------------------------------------------
void ofxSurfingRandomizer::doResetParamsFull(ResetPramsType MS_type) {
	ofLogNotice(__FUNCTION__);

	
	for (int i = 0; i < enablersForParams.size(); i++)
		//for (auto p : enablersForParams)
	{
		auto p = enablersForParams[i];
		if (!p.get()) continue;//only reset this iterated param if it's enabled

		//-

		std::string name = p.getName();//name
		auto &g = params_EditorGroups.getGroup(name);//ofParameterGroup
		auto &e = g.get(name);//ofAbstractParameter

		auto type = e.type();
		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();

		bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
		bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
		bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

		if (isFloat)
		{
			auto pmin = g.getFloat("Min").get();
			auto pmax = g.getFloat("Max").get();
			ofParameter<float> p0 = e.cast<float>();

			if (0) {}
			else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
			else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
			else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
			else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
			else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
			else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
		}

		else if (isInt)
		{
			auto pmin = g.getInt("Min").get();
			auto pmax = g.getInt("Max").get();
			ofParameter<int> p0 = e.cast<int>();

			if (0) {}
			else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
			else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
			else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
			else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
			else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
			else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
		}

		else if (isVec2)
		{
			auto pmin = g.getVec2f("Min").get();
			auto pmax = g.getVec2f("Max").get();
			ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();

			if (0) {}
			else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
			else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
			else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
			else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
			else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
			else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

			//for (int dim = 0; dim < 2; dim++) {
			//	if (dim == 0) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.getMin().x, p0.get().y));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.getMax().x - p0.getMin().x / 2, p0.get().y));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(pmax.x - pmin.x / 2, p0.get().y));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(pmin.x, p0.get().y));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(pmax.x, p0.get().y));//reset to range max
			//	}
			//	else if (dim == 1) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.get().x, p0.getMin().y));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.get().x,p0.getMax().y - p0.getMin().y / 2));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(p0.get().x,pmax.y - pmin.y / 2));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(p0.get().x, pmin.y));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(p0.get().x, pmax.y));//reset to range max
			//	}
			//}
		}
		else if (isVec3)
		{
			auto pmin = g.getVec3f("Min").get();
			auto pmax = g.getVec3f("Max").get();
			ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();

			if (0) {}
			else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
			else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
			else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
			else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
			else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
			else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

			//for (int dim = 0; dim < 3; dim++) {
			//	if (dim == 0) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.getMin().x, p0.get().y, p0.get().z));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.getMax().x - p0.getMin().x / 2, p0.get().y, p0.get().z));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(pmax.x - pmin.x / 2, p0.get().y, p0.get().z));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(pmin.x, p0.get().y, p0.get().z));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(pmax.x, p0.get().y, p0.get().z));//reset to range max
			//	}
			//	else if (dim == 1) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.getMin().y, p0.getMin().z));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.getMax().y - p0.getMin().y / 2, p0.getMin().z));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, pmax.y - pmin.y / 2, p0.getMin().z));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.getMin().z));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, pmin.y, p0.getMin().z));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, pmax.y, p0.getMin().z));//reset to range max
			//	}
			//	else if (dim == 2) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMin().z));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMax().z - p0.getMin().z / 2));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z - pmin.z / 2));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, pmin.z));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z));//reset to range max
			//	}
			//}
		}
		else if (isVec4)
		{
			auto pmin = g.getVec4f("Min").get();
			auto pmax = g.getVec4f("Max").get();
			ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();

			if (0) {}
			else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
			else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
			else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
			else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
			else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
			else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

			//for (int dim = 0; dim < 4; dim++) {
			//	if (dim == 0) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.getMin().x, p0.get().y, p0.get().z, p0.get().w));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4((p0.getMax().x - p0.getMin().x) / 2, p0.get().y, p0.get().z, p0.get().w));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4((pmax.x - pmin.x) / 2, p0.get().y, p0.get().z, p0.get().w));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(pmin.x, p0.get().y, p0.get().z, p0.get().w));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(pmax.x, p0.get().y, p0.get().z, p0.get().w));//reset to range max
			//	}
			//	else if (dim == 1) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.getMin().y, p0.get().z, p0.get().w));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, (p0.getMax().y - p0.getMin().y) / 2, p0.get().z, p0.get().w));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, (pmax.y - pmin.y) / 2, p0.get().z, p0.get().w));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, pmin.y, p0.get().z, p0.get().w));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, pmax.y, p0.get().z, p0.get().w));//reset to range max
			//	}
			//	else if (dim == 2) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.getMin().z, p0.get().w));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (p0.getMax().z - p0.getMin().z) / 2, p0.get().w));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (pmax.z - pmin.z) / 2, p0.get().w));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, pmin.z, p0.get().w));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, pmax.z, p0.get().w));//reset to range max
			//	}
			//	else if (dim == 3) {
			//		if (0) {}
			//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMin()));//reset to param min
			//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (p0.getMax().w - p0.getMin().w) / 2));
			//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (pmax.w - pmin.w) / 2));
			//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMax().w));//reset to param max
			//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmin));//reset to range min
			//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmax));//reset to range max
			//	}
			//}
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

		bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
		bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
		bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

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

		else if (isVec2)
		{
			ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<glm::vec2>();
			auto _pmax = g.get("Max").cast<glm::vec2>();
			//reset to abs min
			_pmin.set(pmin);
			_pmax.set(pmax);
		}
		else if (isVec3)
		{
			ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<glm::vec3>();
			auto _pmax = g.get("Max").cast<glm::vec3>();
			//reset to abs min
			_pmin.set(pmin);
			_pmax.set(pmax);
		}
		else if (isVec4)
		{
			ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<glm::vec4>();
			auto _pmax = g.get("Max").cast<glm::vec4>();
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

		bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
		bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
		bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

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

		else if (isVec2)
		{
			ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<glm::vec2>();
			auto _pmax = g.get("Max").cast<glm::vec2>();
			//reset to abs min a bit closed
			float wx = (pmax.x - pmin.x) * _ratio;
			float wy = (pmax.y - pmin.y) * _ratio;
			_pmin.set(glm::vec2(pmin.x + wx, pmin.y + wy));
			_pmax.set(glm::vec2(pmax.x - wx, pmax.y - wy));
		}
		else if (isVec3)
		{
			ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<glm::vec3>();
			auto _pmax = g.get("Max").cast<glm::vec3>();
			//reset to abs min a bit closed
			float wx = (pmax.x - pmin.x) * _ratio;
			float wy = (pmax.y - pmin.y) * _ratio;
			float wz = (pmax.z - pmin.z) * _ratio;
			_pmin.set(glm::vec3(pmin.x + wx, pmin.y + wy, pmin.z + wz));
			_pmax.set(glm::vec3(pmax.x - wx, pmax.y - wy, pmax.z - wz));
		}
		else if (isVec4)
		{
			ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();
			auto pmin = p0.getMin();
			auto pmax = p0.getMax();
			auto _pmin = g.get("Min").cast<glm::vec4>();
			auto _pmax = g.get("Max").cast<glm::vec4>();
			//reset to abs min a bit closed
			float wx = (pmax.x - pmin.x) * _ratio;
			float wy = (pmax.y - pmin.y) * _ratio;
			float wz = (pmax.z - pmin.z) * _ratio;
			float ww = (pmax.w - pmin.w) * _ratio;
			_pmin.set(glm::vec4(pmin.x + wx, pmin.y + wy, pmin.z + wz, pmin.w + ww));
			_pmax.set(glm::vec4(pmax.x - wx, pmax.y - wy, pmax.z - wz, pmax.w - ww));
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doRandomize() {
	static int count = 0;
	ofLogNotice(__FUNCTION__) << count++;

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

		bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
		bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
		bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

		////TODO:
		//bool isiVec2 = type == typeid(ofParameter<glm::ivec2>).name();
		//bool isiVec3 = type == typeid(ofParameter<glm::ivec3>).name();
		//bool isiVec4 = type == typeid(ofParameter<glm::ivec4>).name();

		if (0) {}

		else if (isBool)
		{
			bool b = (ofRandom(0, 2) >= 1);
			ofParameter<bool> p0 = e.cast<bool>();
			p0.set(b);
		}

		else if (isInt)
		{
			auto pmin = g.getInt("Min").get();
			auto pmax = g.getInt("Max").get();
			ofParameter<int> p0 = e.cast<int>();
			p0.set((int)ofRandom(pmin, pmax + 1));//random
		}

		else if (isFloat)
		{
			auto pmin = g.getFloat("Min").get();
			auto pmax = g.getFloat("Max").get();
			ofParameter<float> p0 = e.cast<float>();
			p0.set((float)ofRandom(pmin, pmax));//random
		}

		else if (isVec2)
		{
			auto pvmin = g.getVec2f("Min").get();
			auto pvmax = g.getVec2f("Max").get();
			ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();

			float x = (float)ofRandom(pvmin.x, pvmax.x);
			float y = (float)ofRandom(pvmin.y, pvmax.y);
			//float x = p0.get().x;
			//float y = p0.get().y;
			//if (enablersForParams[i]) x = (float)ofRandom(pvmin.x, pvmax.x);
			//if (enablersForParams[i + 1]) y = (float)ofRandom(pvmin.y, pvmax.y);
			p0.set(glm::vec2(x, y));
		}
		else if (isVec3)
		{
			auto pvmin = g.getVec3f("Min").get();
			auto pvmax = g.getVec3f("Max").get();
			ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();

			float x = (float)ofRandom(pvmin.x, pvmax.x);
			float y = (float)ofRandom(pvmin.y, pvmax.y);
			float z = (float)ofRandom(pvmin.z, pvmax.z);
			//float x = p0.get().x;
			//float y = p0.get().y;
			//float z = p0.get().z;
			//if (enablersForParams[i]) x = (float)ofRandom(pvmin.x, pvmax.x);
			//if (enablersForParams[i + 1]) y = (float)ofRandom(pvmin.y, pvmax.y);
			//if (enablersForParams[i + 2]) z = (float)ofRandom(pvmin.z, pvmax.z);
			p0.set(glm::vec3(x, y, z));
		}
		else if (isVec4)
		{
			auto pvmin = g.getVec4f("Min").get();
			auto pvmax = g.getVec4f("Max").get();
			ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();
			
			float x = (float)ofRandom(pvmin.x, pvmax.x);
			float y = (float)ofRandom(pvmin.y, pvmax.y);
			float z = (float)ofRandom(pvmin.z, pvmax.z);
			float w = (float)ofRandom(pvmin.w, pvmax.w);

			//float x = p0.get().x;
			//float y = p0.get().y;
			//float z = p0.get().z;
			//float w = p0.get().w;
			//if (enablersForParams[i]) x = (float)ofRandom(pvmin.x, pvmax.x);
			//if (enablersForParams[i + 1]) y = (float)ofRandom(pvmin.y, pvmax.y);
			//if (enablersForParams[i + 2]) z = (float)ofRandom(pvmin.z, pvmax.z);
			//if (enablersForParams[i + 3]) w = (float)ofRandom(pvmin.w, pvmax.w);
			p0.set(glm::vec4(x, y, z, w));
		}

		//else if (isiVec2)
		//{
		//	auto pvmin = g.getVec2f("Min").get();
		//	auto pvmax = g.getVec2f("Max").get();
		//	ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();
		//	float x = (float)ofRandom(pvmin.x, pvmax.x);
		//	float y = (float)ofRandom(pvmin.y, pvmax.y);
		//	p0.set(glm::vec2(x, y));
		//}
		//else if (isVec3)
		//{
		//	auto pvmin = g.getVec3f("Min").get();
		//	auto pvmax = g.getVec3f("Max").get();
		//	ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();
		//	float x = (float)ofRandom(pvmin.x, pvmax.x);
		//	float y = (float)ofRandom(pvmin.y, pvmax.y);
		//	float z = (float)ofRandom(pvmin.z, pvmax.z);
		//	p0.set(glm::vec3(x, y, z));
		//}
		//else if (isVec4)
		//{
		//	auto pvmin = g.getVec4f("Min").get();
		//	auto pvmax = g.getVec4f("Max").get();
		//	ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();
		//	float x = (float)ofRandom(pvmin.x, pvmax.x);
		//	float y = (float)ofRandom(pvmin.y, pvmax.y);
		//	float z = (float)ofRandom(pvmin.z, pvmax.z);
		//	float w = (float)ofRandom(pvmin.w, pvmax.w);
		//	p0.set(glm::vec4(x, y, z, w));
		//}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_RangeEditorVecRow(int indexParam, int dimParamdim) {

	int i = indexParam;
	const int dim = dimParamdim;

	std::string tag;//to push ids
	ImGuiSliderFlags flagSld = ImGuiSliderFlags_None;

	// All the added params have a bool param as a toggle  to enable or not the randomizator to them.
	std::string _name = enablersForParams[i].getName();

	// Each added param has a group with their min/max range params, with the same name that the orginal param
	auto g = params_EditorGroups.getGroup(_name);

	// First column label #01..
	std::string n = "#" + ofToString(i < 10 ? "0" : "") + ofToString(i);//same for each coord (xyzw)


	// Ugly workaround

	ofParameter<ofDefaultVec2> &p2 = g.getVec2f(_name);
	ofParameter<ofDefaultVec2> &pv2min = g.getVec2f("Min");
	ofParameter<ofDefaultVec2> &pv2max = g.getVec2f("Max");

	ofParameter<ofDefaultVec3> &p3 = g.getVec3f(_name);
	ofParameter<ofDefaultVec3> &pv3min = g.getVec3f("Min");
	ofParameter<ofDefaultVec3> &pv3max = g.getVec3f("Max");

	ofParameter<ofDefaultVec4> &p4 = g.getVec4f(_name);
	ofParameter<ofDefaultVec4> &pv4min = g.getVec4f("Min");
	ofParameter<ofDefaultVec4> &pv4max = g.getVec4f("Max");

	for (int id = 0; id < dim; id++)// Iterate each coord (xyzw)
	{
		// The ranges are delimited by floats
		float f;
		float fmin;
		float fmax;

		string _namev = _name;

		// First column label #01..
		ImGui::Text(n.c_str());

		ImGui::SameLine();

		if (id == 0)//x
		{
			if (dim == 2) {
				f = p2.get().x;
				fmin = pv2min.get().x;
				fmax = pv2max.get().x;
			}
			else if (dim == 3) {
				f = p3.get().x;
				fmin = pv3min.get().x;
				fmax = pv3max.get().x;
			}
			else if (dim == 4) {
				f = p4.get().x;
				fmin = pv4min.get().x;
				fmax = pv4max.get().x;
			}
			_namev += " X";
		}

		else if (id == 1)//y
		{
			if (dim == 2) {
				f = p2.get().y;
				fmin = pv2min.get().y;
				fmax = pv2max.get().y;
			}
			else if (dim == 3) {
				f = p3.get().y;
				fmin = pv3min.get().y;
				fmax = pv3max.get().y;
			}
			else if (dim == 4) {
				f = p4.get().y;
				fmin = pv4min.get().y;
				fmax = pv4max.get().y;
			}
			_namev += " Y";
		}

		else if (id == 2)//z
		{
			if (dim == 3) {
				f = p3.get().z;
				fmin = pv3min.get().z;
				fmax = pv3max.get().z;
			}
			else if (dim == 4) {
				f = p4.get().z;
				fmin = pv4min.get().z;
				fmax = pv4max.get().z;
			}
			_namev += " Z";
		}

		else if (id == 3)//w
		{
			if (dim == 4) {
				f = p4.get().w;
				fmin = pv4min.get().w;
				fmax = pv4max.get().w;
			}
			_namev += " W";
		}

		//-

		// Power
		float speed = (fmax - fmin) / 100.f;

		//-

		// 0. Random Button

		tag = n + _namev + "random" + ofToString(id);
		ImGui::PushID(tag.c_str());
		if (ImGui::Button("RND"))
		{
			doRandomize(i, true);//TODO: we randomize all the coords (xyzw). we can pick one of them..
		}
		ImGui::PopID();

		ImGui::SameLine();

		//-

		// 1. Value

		tag = _namev + "value" + ofToString(id);
		ImGui::PushID(tag.c_str());
		ImGui::PushItemWidth(WIDGET_R_DRAG);
		if (ImGui::DragFloat("", &f, speed, fmin, fmax, "%.3f", flagSld))
		{
			if (id == 0)//x
			{
				if (dim == 2) p2.set(glm::vec2(f, p2.get().y));
				else if (dim == 3) p3.set(glm::vec3(f, p3.get().y, p3.get().z));
				else if (dim == 4) p4.set(glm::vec4(f, p4.get().y, p4.get().z, p4.get().w));
			}
			else if (id == 1)//y
			{
				if (dim == 2) p2.set(glm::vec2(p2.get().x, f));
				else if (dim == 3) p3.set(glm::vec3(p3.get().x, f, p3.get().z));
				else if (dim == 4) p4.set(glm::vec4(p4.get().x, f, p4.get().z, p4.get().w));
			}
			else if (id == 2)//z
			{
				if (dim == 3) p3.set(glm::vec3(p3.get().x, p3.get().y, f));
				else if (dim == 4) p4.set(glm::vec4(p4.get().x, p4.get().y, f, p4.get().w));
			}
			else if (id == 3)//w
			{
				if (dim == 4) p4.set(glm::vec4(p4.get().x, p4.get().y, p4.get().z, f));
			}
		}
		ImGui::PopItemWidth();
		ImGui::PopID();

		ImGui::SameLine();

		//-

		// 1. Get from min max

		if (!bMinimal)
		{
			ImGui::AlignTextToFramePadding();
			tag = n + _namev + "getMIN" + ofToString(id);
			ImGui::PushID(tag.c_str());
			if (ImGui::Button("GETMIN"))
			{
				if (dim == 2) p2.set(glm::vec2(fmin, p2.get().y));
				else if (dim == 3) p3.set(glm::vec3(fmin, p3.get().y, p3.get().z));
				else if (dim == 4) p4.set(glm::vec4(fmin, p4.get().y, p4.get().z, p4.get().w));
			}
			ImGui::PopID();

			ImGui::SameLine(0, 1);

			tag = n + _namev + "getMAX" + ofToString(id);
			ImGui::PushID(tag.c_str());
			if (ImGui::Button("GETMAX"))
			{
				if (dim == 2) p2.set(glm::vec2(fmax, p2.get().y));
				else if (dim == 3) p3.set(glm::vec3(fmax, p3.get().y, p3.get().z));
				else if (dim == 4) p4.set(glm::vec4(fmax, p4.get().y, p4.get().z, p4.get().w));
			}
			ImGui::PopID();
		}

		ImGui::SameLine();

		//-

		// 2. Set to min max

		if (!bMinimal)
		{
			ImGui::AlignTextToFramePadding();
			tag = n + _namev + "setMIN" + ofToString(id);

			ImGui::PushID(tag.c_str());
			if (ImGui::Button("SETMIN"))
			{
				if (dim == 2) pv2min.set(glm::vec2(f, p2.get().y));
				else if (dim == 3) pv3min.set(glm::vec3(f, p3.get().y, p3.get().z));
				else if (dim == 4) pv4min.set(glm::vec4(f, p4.get().y, p4.get().z, p4.get().w));
			}
			ImGui::PopID();

			ImGui::SameLine(0, 1);

			tag = n + _namev + "setMAX" + ofToString(id);

			ImGui::PushID(tag.c_str());
			if (ImGui::Button("SETMAX"))
			{
				if (dim == 2) pv2max.set(glm::vec2(f, p2.get().y));
				else if (dim == 3) pv2max.set(glm::vec3(f, p3.get().y, p3.get().z));
				else if (dim == 4) pv2max.set(glm::vec4(f, p4.get().y, p4.get().z, p4.get().w));
			}
			ImGui::PopID();
		}

		if (bMinimal)
		{
			ImGui::SameLine();
			ImGui::Text("RANGE");
		}

		ImGui::SameLine();

		//-

		// 3. Range vanilla

		tag = "##" + n + _namev + "dragMin" + ofToString(id);
		ImGui::PushID(tag.c_str());
		ImGui::PushItemWidth(WIDGET_R_DRAG * 2);

		if (ImGui::DragFloatRange2("", &fmin, &fmax, speed, fmin, fmax, "%.3f", "%.3f"))
		{
			if (id == 0)//x
			{
				if (dim == 2) {
					pv2min.set(glm::vec2(fmin, p2.get().y));
					pv2max.set(glm::vec2(fmax, p2.get().y));
				}
				else if (dim == 3) {
					pv3min.set(glm::vec3(fmin, p3.get().y, p3.get().z));
					pv3max.set(glm::vec3(fmax, p3.get().y, p3.get().z));
				}
				else if (dim == 4) {
					pv4min.set(glm::vec4(fmin, p4.get().y, p4.get().z, p4.get().w));
					pv4max.set(glm::vec4(fmax, p4.get().y, p4.get().z, p4.get().w));
				}
			}
			else if (id == 1)//y
			{
				if (dim == 2) {
					pv2min.set(glm::vec2(p2.get().x, fmin));
					pv2max.set(glm::vec2(p2.get().x, fmax));
				}
				else if (dim == 3) {
					pv3min.set(glm::vec3(p3.get().x, fmin, p3.get().z));
					pv3max.set(glm::vec3(p3.get().x, fmax, p3.get().z));
				}
				else if (dim == 4) {
					pv4min.set(glm::vec4(p4.get().x, fmin, p4.get().z, p4.get().w));
					pv4max.set(glm::vec4(p4.get().x, fmax, p4.get().z, p4.get().w));
				}
			}
			else if (id == 2)//z
			{
				if (dim == 3) {
					pv3min.set(glm::vec3(p3.get().x, p3.get().y, fmin));
					pv3max.set(glm::vec3(p3.get().x, p3.get().y, fmax));
				}
				else if (dim == 4) {
					pv4min.set(glm::vec4(p4.get().x, p4.get().y, fmin, p4.get().w));
					pv4max.set(glm::vec4(p4.get().x, p4.get().y, fmax, p4.get().w));
				}
			}
			else if (id == 3)//w
			{
				if (dim == 4) {
					pv4min.set(glm::vec4(p4.get().x, p4.get().y, p4.get().z, fmin));
					pv4max.set(glm::vec4(p4.get().x, p4.get().y, p4.get().z, fmax));
				}
			}
		}

		ImGui::PopItemWidth();

		ImGui::PopID();

		//-

		// 4. Range Surfing

		if (!bMinimal)
		{
			ImGui::SameLine();

			float power = 1;

			if (id == 0) tag = "##" + n + _namev + "dragMax" + "X";
			else if (id == 1) tag = "##" + n + _namev + "dragMax" + "Y";
			else if (id == 2) tag = "##" + n + _namev + "dragMax" + "Z";
			else if (id == 3) tag = "##" + n + _namev + "dragMax" + "W";

			ImGui::PushID(tag.c_str());
			ImGui::PushItemWidth(WIDGET_R_SLIDER);

			std::string ss = "%.3f" + spcl + "%.3f";

			if (id == 0) {//x
				ofParameter<float> pfmin;
				ofParameter<float> pfmax;

				if (dim == 2) {
					pfmin.set("min", pv2min.get().x, pv2min.getMin().x, pv2min.getMax().x);
					pfmax.set("max", pv2max.get().x, pv2max.getMin().x, pv2max.getMax().x);
				}
				else if (dim == 3) {
					pfmin.set("min", pv3min.get().x, pv3min.getMin().x, pv3min.getMax().x);
					pfmax.set("max", pv3max.get().x, pv3max.getMin().x, pv3max.getMax().x);
				}
				else if (dim == 4) {
					pfmin.set("min", pv4min.get().x, pv4min.getMin().x, pv4min.getMax().x);
					pfmax.set("max", pv4max.get().x, pv4max.getMin().x, pv4max.getMax().x);
				}
				else { continue; };

				if (ofxImGuiSurfing::AddRangeParam(_namev, pfmin, pfmax, ss.c_str(), power))
				{
					if (dim == 2) {
						pv2min.set(glm::vec2(pfmin, pv2min.get().y));
						pv2max.set(glm::vec2(pfmax, pv2max.get().y));
					}
					else if (dim == 3) {
						pv3min.set(glm::vec3(pfmin, pv3min.get().y, pv3min.get().z));
						pv3max.set(glm::vec3(pfmax, pv3max.get().y, pv3min.get().z));
					}
					else if (dim == 4) {
						pv4min.set(glm::vec4(pfmin, pv4min.get().y, pv4min.get().z, pv4min.get().w));
						pv4max.set(glm::vec4(pfmax, pv4max.get().y, pv4min.get().z, pv4min.get().w));
					}
				}
			}
			else if (id == 1) {//y
				ofParameter<float> pfmin;
				ofParameter<float> pfmax;

				if (dim == 2) {
					pfmin.set("min", pv2min.get().y, pv2min.getMin().y, pv2min.getMax().y);
					pfmax.set("max", pv2max.get().y, pv2max.getMin().y, pv2max.getMax().y);
				}
				else if (dim == 3) {
					pfmin.set("min", pv3min.get().y, pv3min.getMin().y, pv3min.getMax().y);
					pfmax.set("max", pv3max.get().y, pv3max.getMin().y, pv3max.getMax().y);
				}
				else if (dim == 4) {
					pfmin.set("min", pv4min.get().y, pv4min.getMin().y, pv4min.getMax().y);
					pfmax.set("max", pv4max.get().y, pv4max.getMin().y, pv4max.getMax().y);
				}
				else { continue; };

				if (ofxImGuiSurfing::AddRangeParam(_namev, pfmin, pfmax, ss.c_str(), power))
				{
					if (dim == 2) {
						pv2min.set(glm::vec2(pv2min.get().x, pfmin));
						pv2max.set(glm::vec2(pv2max.get().x, pfmax));
					}
					else if (dim == 3) {
						pv3min.set(glm::vec3(pv3min.get().x, pfmin, pv3max.get().z));
						pv3max.set(glm::vec3(pv3max.get().x, pfmax, pv3max.get().z));
					}
					else if (dim == 4) {
						pv4min.set(glm::vec4(pv4min.get().x, pfmin, pv4max.get().z, pv4max.get().w));
						pv4max.set(glm::vec4(pv4max.get().x, pfmax, pv4max.get().z, pv4max.get().w));
					}
				}
			}
			else if (id == 2) {//z
				ofParameter<float> pfmin;
				ofParameter<float> pfmax;

				if (dim == 3) {
					pfmin.set("min", pv3min.get().z, pv3min.getMin().z, pv3min.getMax().z);
					pfmax.set("max", pv3max.get().z, pv3max.getMin().z, pv3max.getMax().z);
				}
				else if (dim == 4) {
					pfmin.set("min", pv4min.get().z, pv4min.getMin().z, pv4min.getMax().z);
					pfmax.set("max", pv4max.get().z, pv4max.getMin().z, pv4max.getMax().z);
				}
				else { continue; };

				if (ofxImGuiSurfing::AddRangeParam(_namev, pfmin, pfmax, ss.c_str(), power))
				{
					if (dim == 3) {
						pv3min.set(glm::vec3(pv3min.get().x, pv3max.get().y, pfmin));
						pv3max.set(glm::vec3(pv3max.get().x, pv3max.get().y, pfmax));
					}
					else if (dim == 4) {
						pv4min.set(glm::vec4(pv4min.get().x, pv4max.get().y, pfmin, pv4max.get().w));
						pv4max.set(glm::vec4(pv4max.get().x, pv4max.get().y, pfmax, pv4max.get().w));
					}
				}
			}
			else if (id == 3) {//w
				ofParameter<float> pfmin;
				ofParameter<float> pfmax;

				if (dim == 4) {
					pfmin.set("min", pv4min.get().w, pv4min.getMin().w, pv4min.getMax().w);
					pfmax.set("max", pv4max.get().w, pv4max.getMin().w, pv4max.getMax().w);
				}
				else { continue; };

				if (ofxImGuiSurfing::AddRangeParam(_namev, pfmin, pfmax, ss.c_str(), power))
				{
					if (dim == 4) {
						pv4min.set(glm::vec4(pv4min.get().x, pv4max.get().y, pv4max.get().z, pfmin));
						pv4max.set(glm::vec4(pv4max.get().x, pv4max.get().y, pv4max.get().z, pfmax));
					}
				}
			}

			ImGui::PopItemWidth();
			ImGui::PopID();
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

	// Help
	std::string helpInfo = "";
	helpInfo += "Surfing Randomizer\n";
	helpInfo += "\n";
	helpInfo += "HELP\n";
	helpInfo += "KEY COMMANDS\n";
	helpInfo += "\n";
	helpInfo += "SPACE          : Params Rnd\n";
	helpInfo += "BACKSPACE      : Params Reset Ranged\n";
	helpInfo += "Ctrl+BACKSPACE : Params Reset\n";
	helpInfo += "Ctrl+SPACE     : Index Rnd\n";
	helpInfo += "LEFT-RIGHT     : Index Browse\n";
	helpInfo += "RETURN         : Play Test\n";
	helpInfo += "Ctrl+RETURN    : Play Test Index\n";
	helpInfo = ofToUpper(helpInfo);//make uppercase

	textBoxWidget.setText(helpInfo);
	textBoxWidget.setup();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::startup() {

	// Link advanced params
	bHelp.makeReferenceTo(guiManager.bHelp);
	bKeys.makeReferenceTo(guiManager.bKeys);

	//-

	// Settings
	params_Editor.setName("Ranges Editor");
	params_Editor.add(params_EditorEnablers);
	params_Editor.add(params_EditorGroups);

	ofxSurfingHelpers::loadGroup(params_Editor, path_Editor);

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::addGroup(ofParameterGroup& group) {

	for (auto p : group) // ofAbstractParameter
	{
		//// exclude params not marked as serializable
		//if (!p->isSerializable()) continue;
		////if (!parameter->isSerializable()) return;

		//--

		// Filter different types

		auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(p);// group
		auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(p);
		auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(p);
		auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(p);

		auto parameterVec2 = std::dynamic_pointer_cast<ofParameter<glm::vec2>>(p);
		auto parameterVec3 = std::dynamic_pointer_cast<ofParameter<glm::vec3>>(p);
		auto parameterVec4 = std::dynamic_pointer_cast<ofParameter<glm::vec4>>(p);

		//TODO:
		//auto parameteriVec2 = std::dynamic_pointer_cast<ofParameter<glm::ivec2>>(p);
		//auto parameteriVec3 = std::dynamic_pointer_cast<ofParameter<glm::ivec3>>(p);
		//auto parameteriVec4 = std::dynamic_pointer_cast<ofParameter<glm::ivec4>>(p);

		//---

		// Group Type

		if (parameterGroup)
		{
			// Use only the serializable params of the group
			//if (!parameterGroup->isSerializable()) continue;
			////if (!parameterGroup->isSerializable()) return;

			//TODO:
			//BUG: multiple nested..
			addGroup(*parameterGroup);

			//auto g0 = group.getGroup(parameterGroup->getName());
			//addGroup(g0);

			continue;
		}

		//-

		// Common Types

		//-

		// Bool
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

		//-

		// Int
		else if (parameterInt)
		{
			std::string _name = parameterInt->getName();

			int vtot = parameterInt->getMax() - parameterInt->getMin();
			int vmin = parameterInt->getMin() + DEFAULT_MIN_PCT * vtot;
			int vmax = parameterInt->getMin() + DEFAULT_MAX_PCT * vtot;

			ofParameterGroup _g{ _name };
			ofParameter<int> p0 = group.getInt(_name);
			ofParameter<int> pmin{ "Min", vmin, parameterInt->getMin() , parameterInt->getMax() };
			ofParameter<int> pmax{ "Max", vmax, parameterInt->getMin() , parameterInt->getMax() };

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

		// Float
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

		// Vec2
		else if (parameterVec2)
		{
			auto vtot = parameterVec2->getMax() - parameterVec2->getMin();
			auto vmin = parameterVec2->getMin() + DEFAULT_MAX_PCT * vtot;
			auto vmax = parameterVec2->getMax() - (1 - DEFAULT_MAX_PCT) * vtot;

			std::string _name = parameterVec2->getName();
			ofParameterGroup _g{ _name };
			ofParameter<glm::vec2> p0 = group.getVec2f(_name);
			ofParameter<glm::vec2> pmin{ "Min", vmin, parameterVec2->getMin() , parameterVec2->getMax() };
			ofParameter<glm::vec2> pmax{ "Max", vmax, parameterVec2->getMin() , parameterVec2->getMax() };

			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			//ofParameter<bool> b0{ _name, false };//-> The global toggle for all the coords
			//ofParameter<bool> b0_x{ _name + " X", false };
			//ofParameter<bool> b0_y{ _name + " Y", false };
			//enablersForParams.push_back(b0);
			//params_EditorEnablers.add(b0);
			//enablersForParams.push_back(b0_x);
			//enablersForParams.push_back(b0_y);
			//params_EditorEnablers.add(b0_x);
			//params_EditorEnablers.add(b0_y);

			continue;
		}

		// Vec3
		else if (parameterVec3)
		{
			auto vtot = parameterVec3->getMax() - parameterVec3->getMin();
			auto vmin = parameterVec3->getMin() + DEFAULT_MAX_PCT * vtot;
			auto vmax = parameterVec3->getMax() - (1 - DEFAULT_MAX_PCT) * vtot;

			std::string _name = parameterVec3->getName();
			ofParameterGroup _g{ _name };
			ofParameter<glm::vec3> p0 = group.getVec3f(_name);
			ofParameter<glm::vec3> pmin{ "Min", vmin, parameterVec3->getMin() , parameterVec3->getMax() };
			ofParameter<glm::vec3> pmax{ "Max", vmax, parameterVec3->getMin() , parameterVec3->getMax() };

			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			//ofParameter<bool> b0{ _name, false };
			//ofParameter<bool> b0_x{ _name + " X", false };
			//ofParameter<bool> b0_y{ _name + " Y", false };
			//ofParameter<bool> b0_z{ _name + " Z", false };
			//enablersForParams.push_back(b0);
			//params_EditorEnablers.add(b0);
			//enablersForParams.push_back(b0_x);
			//enablersForParams.push_back(b0_y);
			//enablersForParams.push_back(b0_z);
			//params_EditorEnablers.add(b0_x);
			//params_EditorEnablers.add(b0_y);
			//params_EditorEnablers.add(b0_z);

			continue;
		}

		// Vec4
		else if (parameterVec4)
		{
			auto vtot = parameterVec4->getMax() - parameterVec4->getMin();
			auto vmin = parameterVec4->getMin() + DEFAULT_MAX_PCT * vtot;
			auto vmax = parameterVec4->getMax() - (1 - DEFAULT_MAX_PCT) * vtot;

			std::string _name = parameterVec4->getName();
			ofParameterGroup _g{ _name };
			ofParameter<glm::vec4> p0 = group.getVec4f(_name);
			ofParameter<glm::vec4> pmin{ "Min", vmin, parameterVec4->getMin() , parameterVec4->getMax() };
			ofParameter<glm::vec4> pmax{ "Max", vmax, parameterVec4->getMin() , parameterVec4->getMax() };

			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);
			
			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			//ofParameter<bool> b0{ _name, false };
			//ofParameter<bool> b0_x{ _name + " X", false };
			//ofParameter<bool> b0_y{ _name + " Y", false };
			//ofParameter<bool> b0_z{ _name + " Z", false };
			//ofParameter<bool> b0_w{ _name + " W", false };
			//enablersForParams.push_back(b0);
			//params_EditorEnablers.add(b0);
			//enablersForParams.push_back(b0_x);
			//enablersForParams.push_back(b0_y);
			//enablersForParams.push_back(b0_z);
			//enablersForParams.push_back(b0_w);
			//params_EditorEnablers.add(b0_x);
			//params_EditorEnablers.add(b0_y);
			//params_EditorEnablers.add(b0_z);
			//params_EditorEnablers.add(b0_w);

			continue;
		}

		//-

		//TODO:

		/*
		// iVec2
		else if (parameteriVec2)
		{
			auto vtot = parameteriVec2->getMax() - parameteriVec2->getMin();
			auto vmin = parameteriVec2->getMin();
			auto vmax = parameteriVec2->getMax();

			std::string _name = parameteriVec2->getName();
			ofParameterGroup _g{ _name };
			ofParameter<glm::vec2> p0 = group.getVec2f(_name);
			ofParameter<glm::vec2> pmin{ "Min", vmin, parameteriVec2->getMin() , parameteriVec2->getMax() };
			ofParameter<glm::vec2> pmax{ "Max", vmax, parameteriVec2->getMin() , parameteriVec2->getMax() };

			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			continue;
		}

		// iVec3
		else if (parameteriVec3)
		{
			auto vtot = parameteriVec3->getMax() - parameteriVec3->getMin();
			auto vmin = parameteriVec3->getMin();
			auto vmax = parameteriVec3->getMax();

			std::string _name = parameteriVec3->getName();
			ofParameterGroup _g{ _name };
			ofParameter<glm::vec3> p0 = group.getVec3f(_name);
			ofParameter<glm::vec3> pmin{ "Min", vmin, parameteriVec3->getMin() , parameteriVec3->getMax() };
			ofParameter<glm::vec3> pmax{ "Max", vmax, parameteriVec3->getMin() , parameteriVec3->getMax() };

			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			continue;
		}

		// iVec4
		else if (parameteriVec4)
		{
			auto vtot = parameteriVec4->getMax() - parameteriVec4->getMin();
			auto vmin = parameteriVec4->getMin();
			auto vmax = parameteriVec4->getMax();

			std::string _name = parameteriVec4->getName();
			ofParameterGroup _g{ _name };
			ofParameter<glm::vec4> p0 = group.getVec4f(_name);
			ofParameter<glm::vec4> pmin{ "Min", vmin, parameteriVec4->getMin() , parameteriVec4->getMax() };
			ofParameter<glm::vec4> pmax{ "Max", vmax, parameteriVec4->getMin() , parameteriVec4->getMax() };

			_g.add(p0);
			_g.add(pmin);
			_g.add(pmax);

			params_EditorGroups.add(_g);

			ofParameter<bool> b0{ _name, false };
			enablersForParams.push_back(b0);
			params_EditorEnablers.add(b0);

			continue;
		}
		*/

		//-

		ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << p->getName();
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

	// app
	if (key == 'g') bGui = !bGui;
	if (key == 'h') bHelp = !bHelp;

	// randomize
	if (!mod_CONTROL && key == ' ') { doRandomize(); }
	if (mod_CONTROL && key == ' ') { (surfingIndexRandomizer.doRandom()); }

	// resets
	if (mod_CONTROL && key == OF_KEY_BACKSPACE) { doResetParamsFull(RESET_PARAM_MIN); }
	if (!mod_CONTROL && key == OF_KEY_BACKSPACE) { doResetParamsFull(RESET_RANGE_MIN); }

	// play / tester
	if (!mod_CONTROL && key == OF_KEY_RETURN) bPLAY = !bPLAY;
	if (mod_CONTROL && key == OF_KEY_RETURN) surfingIndexRandomizer.bPLAY = !surfingIndexRandomizer.bPLAY;

	// index
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
	//surfingIndexRandomizer.keyPressed(key);

	//----

	// TODO: not working on windows.. We need to add int code
#ifdef INCLUDE__OFX_UNDO_ENGINE
	undoManger.keyPressed(eventArgs);
#endif

	//----
}
