#include "ofxSurfingRandomizer.h"

//--------------------------------------------------------------
ofxSurfingRandomizer::ofxSurfingRandomizer() {
	ofAddListener(ofEvents().draw, this, &ofxSurfingRandomizer::draw);
	//ofAddListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	//ofAddListener(ofEvents().keyPressed, this, &ofxSurfingRandomizer::keyPressed);

	path_Editor = path_Global + "ofxSurfingRandomizer_Editor.json";
	path_AppState = path_Global + "ofxSurfingRandomizer_State.json";
	path_MemoryState = path_Global + "ofxSurfingRandomizer_MemoryState.json";

	bGui.set("SURFING RANDOMIZER", true);
	bParams.set("PARAMETERS", true);
	bEditor.set("EDITOR", true);
	bControls.set("CONTROLS", true);

	params_AppState.setName("ofxSurfingRandomizer");
	params_AppState.add(bGui);
	params_AppState.add(bParams);
	params_AppState.add(bEditor);
	params_AppState.add(bControls);
	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);
}

//--------------------------------------------------------------
ofxSurfingRandomizer::~ofxSurfingRandomizer() {
	ofRemoveListener(ofEvents().draw, this, &ofxSurfingRandomizer::draw);
	//ofRemoveListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	//ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingRandomizer::keyPressed);

	exit();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::setup(ofParameterGroup& group) {
	ofAddListener(ofEvents().draw, this, &ofxSurfingRandomizer::draw, OF_EVENT_ORDER_AFTER_APP);

	ofxSurfingHelpers::CheckFolder(path_Global);

	params = group;//store the external target params

	setupEditor(group);//build the editor for all the params

	// gui
	guiManager.setup();//initiate ImGui
	//guiManager.setUseAdvancedSubPanel(true);
}

//--------------------------------------------------------------
//void ofxSurfingRandomizer::update(ofEventArgs & args)
//{
//}

//--------------------------------------------------------------
void ofxSurfingRandomizer::draw(ofEventArgs & args) {
	if (!bGui) return;

	guiManager.begin();
	{
		auto mainSettings = ofxImGui::Settings();
		ImGuiColorEditFlags _flagsw = ImGuiWindowFlags_None;
		string name;

		bool bOpen;
		ImGuiColorEditFlags _flagc;

		//widgets sizes
		float _spcx;
		float _spcy;
		float _w100;
		float _h100;
		float _w99;
		float _w50;
		float _w33;
		float _w25;
		float _h;

		if (guiManager.auto_resize) _flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		// 1. window parameters

		if (bParams)
		{
			name = "PARAMETERS";
			if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw))
			{
				drawParams();

				////TODO:
				////range sliders widgets
				//static float vmin = 0;
				//static float vmax = 100;
				//static float v_min = 0;
				//static float v_max = 100;
				//static float power = 0.001f;
				////TODO:
				//static ofParameter<float> parameterMin{ "Min", 0.5, 0, 1 };
				//static ofParameter<float> parameterMax{ "Max", 0.5, 0, 1 };
				//static float speed = 0.001;
				////bool ofxImGui::AddRange(const std::string& name, ofParameter<float>& parameterMin, ofParameter<float>& parameterMax, float speed)
				////ImGui::RangeSliderFloat("range", &vmin, &vmax, v_min, v_max, "(%.3f, %.3f)", power);
				//ofxImGui::AddRange("range", parameterMin, parameterMax, speed);
			}
			ofxImGui::EndWindow(mainSettings);
		}

		//----

		// 2. window panels

		if (bGui)
		{
			name = "SURFING RANDOMIZER";
			if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw))
			{
				ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

				//-

				bOpen = true;
				_flagc = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
				if (ImGui::CollapsingHeader("PANELS", _flagc))
				{
					//ImGui::Text("PANELS");
					ofxSurfingHelpers::AddBigToggle(bParams, _w50, _h);
					ImGui::SameLine();
					ofxSurfingHelpers::AddBigToggle(bEditor, _w50, _h);
					//ofxSurfingHelpers::AddBigToggle(bControls, _w50, _h);
				}
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				//-

				bOpen = true;
				_flagc = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
				if (ImGui::CollapsingHeader("COMMANDS", _flagc))
				{
					//ImGui::Text("COMMANDS");

					if (ImGui::Button("RANDOMiZE!", ImVec2(_w100, 2 * _h)))
					{
						doRandomize();
					}

					if (ImGui::Button("RESET PARAMS", ImVec2(_w50, _h / 2)))
					{
						doResetParams();
					}
					ImGui::SameLine();
					if (ImGui::Button("RESET RANGES", ImVec2(_w50, _h / 2)))
					{
						doResetRanges();
					}

					//state memory
					if (ImGui::Button("RECALL", ImVec2(_w50, _h / 2)))
						//if (ImGui::Button("LOAD STATE", ImVec2(_w50, _h/2)))
					{
						doLoadState();
					}
					ImGui::SameLine();
					if (ImGui::Button("MEMORiZE", ImVec2(_w50, _h / 2)))
						//if (ImGui::Button("SAVE STATE", ImVec2(_w50, _h/2)))
					{
						doSaveState();
					}
				}
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				//-

				// enable toggles

				bOpen = true;
				_flagc = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
				if (ImGui::CollapsingHeader("ENABLE PARAMETERS", _flagc))
				{
					//ImGui::Text("ENABLE PARAMETERS");

					static bool bNone, bAll;
					if (ImGui::Button("NONE", ImVec2(_w50, _h / 2)))
					{
						doDisableAll();
					}
					ImGui::SameLine();
					if (ImGui::Button("ALL", ImVec2(_w50, _h / 2)))
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
							ofxSurfingHelpers::AddBigToggle(pb, _w75, _h / 2, false);
							ImGui::SameLine();
							// 2. button random
							ImGui::PushID(i);//required to allow same name in all buttons
							if (ImGui::Button("RANDOM", ImVec2(_w25, _h / 2)))
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
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

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

				guiManager.drawAdvancedSubPanel();
			}
			ofxImGui::EndWindow(mainSettings);
		}

		//-

		// 3. window editor

		if (bEditor)
		{
			name = "EDITOR";
			if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw))
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
				flags |= ImGuiTreeNodeFlags_DefaultOpen;
				flags |= ImGuiTreeNodeFlags_Framed;

				//ofxImGui::AddGroup(params_EditorGroups, flags);

				bool bOpen = true;
				ImGuiColorEditFlags _flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagw |= ImGuiTreeNodeFlags_Framed;

				//sliders props
				const int _ww1 = 80;
				const int _ww2 = 200;
				ImGuiSliderFlags sflag = ImGuiSliderFlags_None;
				//sflag |= ImGuiSliderFlags_ReadOnly;
				string spcl = "    ";//space between min-max range slider label

				if (ImGui::TreeNodeEx("RANGES", _flagw))
				{
					ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

					//for (int i = 0; i < params_EditorGroups.size(); i++) 
					for (int i = 0; i < enablersForParams.size(); i++)
					{
						//numerize
						string tag;//to push ids
						string n = "#" + ofToString(i < 10?"0":"") + ofToString(i);
						//ImGui::Dummy(ImVec2(0,10));
						ImGui::Text(n.c_str());
						ImGui::SameLine();
						//ImGui::Dummy(ImVec2(0,-20));

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
							if (ImGui::Button("RANDOM"))
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

							// 0. MIN-MAX
							tag = n + "min";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MIN"))
							{
								pmin = _p0;
								//_p0 = pmin;
							}
							ImGui::PopID();
							ImGui::SameLine();

							tag = n + "max";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MAX"))
							{
								pmax = _p0;
								//_p0 = pmax;
							}
							ImGui::PopID();
							ImGui::SameLine();

							// 2. range
							//vanilla
							tag = n + "dragMin";
							ImGui::PushID(tag.c_str());
							ImGui::DragFloatRange2(""/*_name.c_str()*/, (float*)&pmin.get(), (float*)&pmax.get(), speed, pmin.getMin(), pmin.getMax(), "%.3f", "%.3f");
							ImGui::PopID();
							ImGui::SameLine();
							//widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(_ww2);
							string ss = "%.3f" + spcl + "%.3f";
							ImGui::AddRangeParam(_name, pmin, pmax, ss.c_str(), power);
							//ImGui::AddRangeParam("%.3f   %.3f", pmin, pmax, ss.c_str(), power);
							ImGui::PopItemWidth();
							ImGui::PopID();
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
							if (ImGui::Button("RANDOM"))
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

							// 0. MIN-MAX
							tag = n + "min";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MIN"))
							{
								pmin = _p0;
								//_p0 = pmin;
							}
							ImGui::PopID();
							ImGui::SameLine();

							tag = n + "max";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("MAX"))
							{
								pmax = _p0;
								//_p0 = pmax;
							}
							ImGui::PopID();
							ImGui::SameLine();

							// 2. range
							// vanilla
							tag = n + "dragMin";
							ImGui::PushID(tag.c_str());
							ImGui::DragIntRange2(""/*_name.c_str()*/, (int*)&pmin.get(), (int*)&pmax.get(), speed, pmin.getMin(), pmin.getMax(), "%.0f", "%.0f");
							ImGui::PopID();
							ImGui::SameLine();
							//widget
							float power = 1;
							tag = n + "dragMax";
							ImGui::PushID(tag.c_str());
							ImGui::PushItemWidth(_ww2);
							string ss = "%.0f" + spcl + "%.0f";
							ImGui::AddRangeParam(_name, pmin, pmax, ss.c_str(), power);
							//ImGui::AddRangeParam("%.0f   %.0f", pmin, pmax, ss.c_str(), power);
							ImGui::PopItemWidth();
							ImGui::PopID();
						}

						//-

						else if (isBool)
						{
							// 0. button random
							tag = n + "random";
							ImGui::PushID(tag.c_str());
							if (ImGui::Button("RANDOM"))
							{
								doRandomize(i, true);
							}
							ImGui::PopID();
							ImGui::SameLine();

							auto &_p0 = g.getBool(_name);
							ofxImGui::AddParameter(_p0);
						}
					}

					ImGui::TreePop();
				}
			}
			ofxImGui::EndWindow(mainSettings);
		}
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawParams() {

	auto mainSettings = ofxImGui::Settings();

	//float _spcx;
	//float _spcy;
	//float _w100;
	//float _h100;
	//float _w99;
	//float _w50;
	//float _w33;
	//float _w25;
	//float _h;
	//ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
	flags |= ImGuiTreeNodeFlags_DefaultOpen;
	flags |= ImGuiTreeNodeFlags_Framed;

	ofxImGui::AddGroup(params, flags);

	//ImGui::Dummy(ImVec2(0.0f, 5.0f));
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
void ofxSurfingRandomizer::doResetParams() {
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
			p0.set(pmin);//reset to min
		}

		else if (isInt)
		{
			auto pmin = g.getInt("Min").get();
			auto pmax = g.getInt("Max").get();
			ofParameter<int> p0 = e.cast<int>();
			p0.set(pmin);//reset to min
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

	//for (auto p : enablersForParams)
	//{
	//	if (!p.get()) continue;//only reset this param if it's enabled

	//	//-

	//	string name = p.getName();//name
	//	auto &g = params_EditorGroups.getGroup(name);//ofParameterGroup
	//	auto &e = g.get(name);//ofAbstractParameter

	//	auto type = e.type();
	//	bool isFloat = type == typeid(ofParameter<float>).name();
	//	bool isInt = type == typeid(ofParameter<int>).name();
	//	bool isBool = type == typeid(ofParameter<bool>).name();

	//	if (isFloat)
	//	{
	//		auto pmin = g.getFloat("Min").get();
	//		auto pmax = g.getFloat("Max").get();
	//		ofParameter<float> p0 = e.cast<float>();
	//		p0.set((float)ofRandom(pmin, pmax));//random
	//	}

	//	else if (isInt)
	//	{
	//		auto pmin = g.getInt("Min").get();
	//		auto pmax = g.getInt("Max").get();
	//		ofParameter<int> p0 = e.cast<int>();
	//		p0.set((int)ofRandom(pmin, pmax + 1));//random
	//	}

	//	else if (isBool)
	//	{
	//		bool b = (ofRandom(0, 2) >= 1);
	//		ofParameter<bool> p0 = e.cast<bool>();
	//		p0.set(b);
	//	}
	//}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doRandomize(int index, bool bForce) {
	ofLogNotice(__FUNCTION__) << index;

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
	params_Editor.setName("Editor");
	params_Editor.add(params_EditorEnablers);
	params_Editor.add(params_EditorGroups);
	ofxSurfingHelpers::loadGroup(params_Editor, path_Editor);

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
