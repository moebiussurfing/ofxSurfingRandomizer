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
	bGui_Editor.set("RND RANGE EDITOR", true);
	bGui_Params.set("RND PARAMS", true);
	bGui_Index.set("RND INDEX", true);

	params_AppState.setName("ofxSurfingRandomizer");
	params_AppState.add(bGui);
	params_AppState.add(bKeys.set("Keys", true));
	params_AppState.add(bGui_Params);
	params_AppState.add(bGui_Editor);
	params_AppState.add(bGui_Index);
	params_AppState.add(bMinimal);
	params_AppState.add(bPLAY.set("PLAY", false));
	params_AppState.add(playSpeed.set("Speed", 0.5, 0, 1));
	params_AppState.add(surfingIndexGroupRandomizer.params_Clicker);

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

#ifdef INCLUDE_ofxUndoSimple
	undoManger.setPathGlobal(path_Global);
	undoManger.setup(params);
	//params_AppState.add(undoManger.getParamsAppState());
#endif

	//--

	// gui

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	guiManager.setImGuiAutodraw(bAutoDraw);
	guiManager.setup();
#endif

	//--

	// index randomizer

	if (!bCustomIndex) {
		const int NUM_INDEX_ITEMS = 9;
		indexTarget.set("Index", 0, 0, NUM_INDEX_ITEMS - 1);
	}

	//surfingIndexGroupRandomizer.indexSelected.makeReferenceTo(indexTarget);// TODO: link
	//surfingIndexGroupRandomizer.setup(indexTarget.getMax());
	//surfingIndexGroupRandomizer.setBoolGui(bGui_Index);
	////surfingIndexGroupRandomizer.setBoolGuiPtr(bGui_Index);

	surfingIndexGroupRandomizer.setPath(path_Global);
	surfingIndexGroupRandomizer.setup(indexTarget, bGui_Index);

	//-

	setupEditor(group); // build the editor for all the params
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Editor() {

	// window editor
	ImGuiWindowFlags _flagsw;
	string n;

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
	if (guiManager.bGui) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
#endif

	//#ifdef USE_RANDOMIZE_IMGUI_LOCAL
	//	if (auto_resize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
	//#endif

		//--

	if (bGui_Editor)
	{
		//if (guiManager.beginWindow())
		//if (guiManager.beginWindow(bGui_Editor, _flagsw))
		//guiManager.beginWindow();
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

			string spcl = "    ";//space between min-max range slider label

			//--

			if (!bMinimal) {
				bOpen = false;
				_flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagw |= ImGuiTreeNodeFlags_Framed;
				if (ImGui::TreeNodeEx("TOOLS", _flagw))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);
					_h *= 2;

					if (ImGui::Button("RANDOMiZE!", ImVec2(_w50, _h)))
					{
						doRandomize();
					}
					ImGui::SameLine();

					// blink by timer progress
					bool b = bPLAY;
					float a;
					if (b) a = 1 - tn;
					else a = 1.0f;
					if (b) ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.5f, 0.0f, 1.0f, a));
					ofxImGuiSurfing::AddBigToggle(bPLAY, _w50, _h, false);
					if (b) ImGui::PopStyleColor();

					ImGui::Dummy(ImVec2(0, 2));
					ImGui::Text("RESET:");
					if (ImGui::Button("TO PARAM MIN", ImVec2(_w33, _h)))
					{
						doResetParams(true);
					}
					ImGui::SameLine();
					if (ImGui::Button("TO RANGES MIN", ImVec2(_w33, _h)))
					{
						doResetParams(false);
					}
					ImGui::SameLine();
					if (ImGui::Button("RANGES", ImVec2(_w33, _h)))
					{
						doResetRanges();
					}


					ImGui::Dummy(ImVec2(0.0f, 2.0f));

					ImGui::TreePop();
				}
			}

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
					string tag;//to push ids
					string n = "#" + ofToString(i < 10 ? "0" : "") + ofToString(i);
					//ImGui::Dummy(ImVec2(0,10));

					ImGui::AlignTextToFramePadding();
					ImGui::Text(n.c_str());
					ImGui::SameLine();

					string _name = enablersForParams[i].getName();
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
							ImGui::Text("GET");
							ImGui::SameLine(0, 0);
							// 0. MIN-MAX
							tag = n + "getMIN";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MIN"))
							{
								_p0 = pmin;
							}
							ImGui::PopID();
							ImGui::SameLine(0, 0);
							tag = n + "getMAX";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MAX"))
							{
								_p0 = pmax;
							}
							ImGui::PopID();
						}

						ImGui::SameLine();

						//-

						// set to min max

						//ImGui::Dummy(ImVec2(10, 0));

						ImGui::AlignTextToFramePadding();
						ImGui::Text("SET");
						ImGui::SameLine(0, 0);
						// 0. MIN-MAX
						tag = n + "setMIN";
						ImGui::PushID(tag.c_str());
						if (ImGui::Button("MIN"))
						{
							pmin = _p0;
						}
						ImGui::PopID();
						ImGui::SameLine(0, 0);
						tag = n + "setMAX";
						ImGui::PushID(tag.c_str());
						if (ImGui::Button("MAX"))
						{
							pmax = _p0;
						}
						ImGui::PopID();
						ImGui::SameLine();

						// 2. range
						//vanilla
						tag = n + "dragMin";
						ImGui::PushID(tag.c_str());
						ImGui::PushItemWidth(_ww1 * 2);
						ImGui::DragFloatRange2(""/*_name.c_str()*/, (float*)&pmin.get(), (float*)&pmax.get(), speed, pmin.getMin(), pmin.getMax(), "%.3f", "%.3f");
						ImGui::PopItemWidth();
						ImGui::PopID();

						if (!bMinimal) {
							ImGui::SameLine();
							//widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(_ww2);
							string ss = "%.3f" + spcl + "%.3f";
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
							ImGui::Text("GET");
							ImGui::SameLine(0, 0);
							// 0. MIN-MAX
							tag = n + "getMIN";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MIN"))
							{
								_p0 = pmin;
							}
							ImGui::PopID();
							ImGui::SameLine(0, 0);
							tag = n + "getMAX";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MAX"))
							{
								_p0 = pmax;
							}
							ImGui::PopID();
						}

						ImGui::SameLine();

						//-

						// set to min max

						ImGui::AlignTextToFramePadding();
						ImGui::Text("SET");
						ImGui::SameLine(0, 0);

						// 0. MIN-MAX
						tag = n + "setMIN";
						ImGui::PushID(tag.c_str());
						if (ImGui::Button("MIN"))
						{
							pmin = _p0;
						}
						ImGui::PopID();
						ImGui::SameLine(0, 0);

						tag = n + "setMAX";
						ImGui::PushID(tag.c_str());
						if (ImGui::Button("MAX"))
						{
							pmax = _p0;
						}
						ImGui::PopID();

						// 2. range
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
							//widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(_ww2);
							string ss = "%.0f" + spcl + "%.0f";
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
				if (guiManager.bGui) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
#endif

				//#ifdef USE_RANDOMIZE_IMGUI_LOCAL
				//				if (auto_resize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
				//#endif

				ImGui::TreePop();
			}

			//guiManager.endWindow();
		}
		guiManager.endWindow();
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Widgets() {

	if (!bGui) return;

	{
		drawImGui_Main();
		drawImGui_Editor();
		drawImGui_Params();
		drawImGui_Index();

#ifdef INCLUDE_ofxUndoSimple
		undoManger.drawImGui();
#endif
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Main() {

	ImGuiWindowFlags _flagsw = ImGuiWindowFlags_None;
	string n;

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

	//#ifdef USE_RANDOMIZE_IMGUI_LOCAL
	//	if (auto_resize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
	//#endif

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

					//ImGui::Text("COMMANDS");

					if (ImGui::Button("RANDOMiZE!", ImVec2(_w100, 3 * _h)))
					{
						doRandomize();
					}

					ImGui::Spacing();
					//ImGui::Dummy(ImVec2(0.0f, 2.0f));

					//-

					// tester

					bOpen = false;
					ImGuiColorEditFlags _flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
					_flagw |= ImGuiTreeNodeFlags_Framed;

					if (ImGui::TreeNodeEx("TESTER", _flagw))
					{
						ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

						ofxImGuiSurfing::AddBigToggleNamed(bPLAY, _w100, 2 * _h, "STOP RND", "PLAY RND", true, 1 - tn);

						//if (ImGui::Button("RANDOMIZE", ImVec2(_w100, _h))) {
						//	doRandomize();
						//}

						if (bPLAY) {
							ofxImGuiSurfing::ProgressBar2(tn);
							ImGui::PushItemWidth(_w50);
							ofxImGuiSurfing::AddParameter(playSpeed);
							//ImGui::SliderFloat("Speed", &playSpeed, 0, 1);
							ImGui::PopItemWidth();
						}

						//ImGui::Dummy(ImVec2(0.0f, 2.0f));

						ImGui::TreePop();
					}
				}
			}

			//-

			// panels
			{
				ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

				//ImGui::Dummy(ImVec2(0.0f, 2.0f));

				bOpen = true;
				_flagc = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
				if (ImGui::CollapsingHeader("PANELS", _flagc))
				{
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Params);
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Index);

#ifdef INCLUDE_ofxUndoSimple
					ofxImGuiSurfing::AddToggleRoundedButton(undoManger.bGui_UndoEngine);
#endif

					if (bGui_Index) ofxImGuiSurfing::AddParameter(indexTarget);
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

					//ImGui::Text("ENABLE PARAMETERS");

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
						string name = p.getName();

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

					//ImGui::Dummy(ImVec2(0.0f, 5.0f));
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

				if (ImGui::TreeNodeEx("TOOLS", _flagw))
				{
					ofxImGuiSurfing::refreshImGui_WidgetsSizes(_w100, _w50, _w33, _w25, _h);

					ImGui::Dummy(ImVec2(0, 2));
					ImGui::Text("RESET:");
					if (ImGui::Button("TO PARAM MIN", ImVec2(_w100, _h)))
					{
						doResetParams(true);
					}
					//ImGui::SameLine();
					if (ImGui::Button("TO RANGES MIN", ImVec2(_w100, _h)))
					{
						doResetParams(false);
					}
					//ImGui::SameLine();
					if (ImGui::Button("RANGES", ImVec2(_w100, _h)))
					{
						doResetRanges();
					}

					//state memory
					ImGui::Dummy(ImVec2(0, 2));
					ImGui::Text("MEMORY:");
					if (ImGui::Button("STORE", ImVec2(_w50, _h)))
					{
						doSaveState();
					}
					ImGui::SameLine();
					if (ImGui::Button("RECALL", ImVec2(_w50, _h)))
					{
						doLoadState();
					}

					ImGui::Dummy(ImVec2(0.0f, 2.0f));
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
			//	string name =
			//		ofxImGui::AddRange("range", parameterMin, parameterMax, speed);
			//}

			//-

			ofxImGuiSurfing::AddToggleRoundedButton(bKeys);

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

	// tester
	// play timed randoms
	static const int _secs = 2;
	if (bPLAY) {
		//int max = 60 * _secs;
		int max = ofMap(playSpeed, 0, 1, 60, 5) * _secs;
		tf = ofGetFrameNum() % max;
		tn = ofMap(tf, 0, max, 0, 1);
		if (tf == 0)
		{
			doRandomize();
		}
	}

	//-

	// randomizers group
	surfingIndexGroupRandomizer.update();

	//-
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::draw(ofEventArgs & args) {

	//#ifdef USE_RANDOMIZE_IMGUI_EXTERNAL
	//	return;
	//#endif

	if (!bGui) return;

	//----

	//// instance - Plugin code that was made for sharing context
	//ImGui::PushID("##ofxSurfingRandomizerHASH"); // <-- if you want to ensure a sandbox between both (fails with sharedMode off)

//#ifdef USE_RANDOMIZE_IMGUI_LOCAL
//	gui.begin();
//	drawImGui_Widgets();
//	gui.end();
//#endif

#ifdef USE_RANDOMIZE_IMGUI_LAYOUT_MANAGER
	guiManager.begin();
	{
		drawImGui_Widgets();
	}
	guiManager.end();
#endif

	//ImGui::PopID(); // <-- If you want to ensure a sandbox between both
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawImGui_Index() {
	ImGuiWindowFlags _flagsw;
	_flagsw = ImGuiWindowFlags_None;

	if (bGui_Index)
	{
		surfingIndexGroupRandomizer.drawImGui();

		//guiManager.beginWindow(bGui_Index, _flagsw);
		//{
		//	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		//	flags |= ImGuiTreeNodeFlags_DefaultOpen;
		//	flags |= ImGuiTreeNodeFlags_Framed;
		//	//ofxImGuiSurfing::AddGroup(params, flags);
		//	{
		//		surfingIndexGroupRandomizer.drawImGui();
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
		string name = p.getName();

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
void ofxSurfingRandomizer::doResetParams(bool bFull) {
	ofLogNotice(__FUNCTION__);

	for (auto p : enablersForParams)
	{
		if (!p.get()) continue;//only reset this param if it's enabled

		//-

		string name = p.getName();//name
		auto &g = params_EditorGroups.getGroup(name);//ofParameterGroup
		auto &e = g.get(name);//ofAbstractParameter

		auto type = e.type();
		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();

		if (isFloat)
		{
			auto pmin = g.getFloat("Min").get();
			auto pmax = g.getFloat("Max").get();
			ofParameter<float> p0 = e.cast<float>();

			if (bFull) p0.set(p0.getMin());//reset to param min
			else p0.set(pmin);//reset to range min
		}

		else if (isInt)
		{
			auto pmin = g.getInt("Min").get();
			auto pmax = g.getInt("Max").get();
			ofParameter<int> p0 = e.cast<int>();

			if (bFull) p0.set(p0.getMin());//reset to param min
			else p0.set(pmin);//reset to range min
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doResetRanges() {
	ofLogNotice(__FUNCTION__);

	for (auto p : enablersForParams)
	{
		if (!p.get()) continue;//only reset this param if it's enabled

		//-

		string name = p.getName();//name
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
void ofxSurfingRandomizer::doRandomize() {
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < enablersForParams.size(); i++)
	{
		doRandomize(i, false);
	}

	//--

#ifdef INCLUDE_ofxUndoSimple
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

		string name = p.getName();//name
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

//#ifdef INCLUDE_ofxUndoSimple
//	undoManger.exit();
//#endif
}

//--

//--------------------------------------------------------------
void ofxSurfingRandomizer::setupEditor(ofParameterGroup& group)
{
	// clear

	params_EditorGroups.clear();// a group for each param
	params_EditorGroups.setName(params.getName());

	enablersForParams.clear();// an enabler toggler for each param
	params_EditorEnablers.clear();// an enabler toggler for each param
	params_EditorEnablers.setName("Params");

	// initiate
	addGroup(group);

	//-

	// settings
	params_Editor.setName("Ranges Editor");
	params_Editor.add(params_EditorEnablers);
	params_Editor.add(params_EditorGroups);
	ofxSurfingHelpers::loadGroup(params_Editor, path_Editor);

	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);
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
	if (key == ' ') { doRandomize(); }
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

	// index randomizer
	surfingIndexGroupRandomizer.keyPressed(key);

	//----

	// TODO: not working on windows.. We need to add int code
#ifdef INCLUDE_ofxUndoSimple
	undoManger.keyPressed(eventArgs);
#endif

	//----
}
