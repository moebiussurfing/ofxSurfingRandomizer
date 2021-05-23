#include "ofxSurfingRandomizer.h"

//--------------------------------------------------------------
ofxSurfingRandomizer::ofxSurfingRandomizer() {
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
	//ofRemoveListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	//ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingRandomizer::keyPressed);

	exit();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::setup(ofParameterGroup& group) {
	ofxSurfingHelpers::CheckFolder(path_Global);

	params = group;

	setupEditor(group);

	// gui
	guiManager.setup();
	//guiManager.setup(gui);
	//guiManager.setUseAdvancedSubPanel(true);
}

////--------------------------------------------------------------
//void ofxSurfingRandomizer::update(){
//}

//--------------------------------------------------------------
//void ofxSurfingRandomizer::update(ofEventArgs & args)
//{
//}

//--------------------------------------------------------------
void ofxSurfingRandomizer::draw() {

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

		if (bParams) {
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

		if (bGui) {
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
							ofParameter<bool> pb = p.cast<bool>();
							ofxSurfingHelpers::AddBigToggle(pb, _w100, _h / 2, false);

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

		if (bEditor) {
			name = "EDITOR";
			if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw))
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
				flags |= ImGuiTreeNodeFlags_DefaultOpen;
				flags |= ImGuiTreeNodeFlags_Framed;

				ofxImGui::AddGroup(params_EditorGroups, flags);

				//TODO:   
				//bool ofxImGui::AddRange(const std::string& name, ofParameter<float>& parameterMin, ofParameter<float>& parameterMax, float speed)
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
	params_EditorGroups.clear();
	params_EditorGroups.setName(params.getName());

	params_EditorEnablers.clear();
	params_EditorEnablers.setName("Params");

	enablersForParams.clear();

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

		// group
		auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(pa);

		// different types
		auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(pa);
		auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(pa);
		auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(pa);

		// parameter, try everything we know how to handle.

		//auto type = pa->type();
		//bool isFloat = type == typeid(ofParameter<float>).name();
		//bool isInt = type == typeid(ofParameter<int>).name();
		//bool isBool = type == typeid(ofParameter<bool>).name();

		//---
/*
//			// x,y,z vectors
//#if OF_VERSION_MINOR >= 10
//			auto parameterVec2f = std::dynamic_pointer_cast<ofParameter<glm::vec2>>(parameter);
//			if (parameterVec2f)
//			{
//				ofParameter<bool> b{ parameterVec2f->getName(), false };
//				//params_EditorGroups.push_back(b);
//				continue;
//			}
//			auto parameterVec3f = std::dynamic_pointer_cast<ofParameter<glm::vec3>>(parameter);
//			if (parameterVec3f)
//			{
//				ofParameter<bool> b{ parameterVec3f->getName(), false };
//				//params_EditorGroups.push_back(b);
//				continue;
//			}
//			auto parameterVec4f = std::dynamic_pointer_cast<ofParameter<glm::vec4>>(parameter);
//			if (parameterVec4f)
//			{
//				ofParameter<bool> b{ parameterVec4f->getName(), false };
//				//params_EditorGroups.push_back(b);
//				continue;
//			}
//#endif
//			auto parameterOfVec2f = std::dynamic_pointer_cast<ofParameter<ofVec2f>>(parameter);
//			if (parameterOfVec2f)
//			{
//				//ofxImGui::AddParameter(*parameterOfVec2f);
//				ofParameter<bool> b{ parameterOfVec2f->getName(), false };
//				//params_EditorGroups.push_back(b);
//				continue;
//			}
//			auto parameterOfVec3f = std::dynamic_pointer_cast<ofParameter<ofVec3f>>(parameter);
//			if (parameterOfVec3f)
//			{
//				//ofxImGui::AddParameter(*parameterOfVec3f);
//				ofParameter<bool> b{ parameterOfVec3f->getName(), false };
//				//params_EditorGroups.push_back(b);
//				continue;
//			}
//			auto parameterOfVec4f = std::dynamic_pointer_cast<ofParameter<ofVec4f>>(parameter);
//			if (parameterOfVec4f)
//			{
//				ofParameter<bool> b{ parameterOfVec4f->getName(), false };
//				//params_EditorGroups.push_back(b);
//				//ofxImGui::AddParameter(*parameterOfVec4f);
//				continue;
//			}
			// colors
			//auto parameterColor = std::dynamic_pointer_cast<ofParameter<ofColor>>(parameter);
			//if (parameterColor)
			//{
			//	ofParameter<bool> b{ parameterColor->getName(), false };
			//	enablersForParams.push_back(b);
			//	continue;
			//}
			//auto parameterFloatColor = std::dynamic_pointer_cast<ofParameter<ofFloatColor>>(parameter);
			//if (parameterFloatColor)
			//{
			//	ofParameter<bool> b{ parameterFloatColor->getName(), false };
			//	enablersForParams.push_back(b);
			//	continue;
			//}
			//-
*/
//---

// group
		if (parameterGroup)
		{
			//if (!parameterGroup->isSerializable()) continue;
			////if (!parameterGroup->isSerializable()) return;

			addGroup(*parameterGroup);

			continue;
		}

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
			//params_EditorEnablers.add(b0);

			continue;
		}

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
			//params_EditorEnablers.add(b0);

			continue;
		}

		//TODO:
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
			//params_EditorEnablers.add(b0);

			continue;
		}

		ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << pa->getName();
	}

	//----

	// create an enabler bool/toggle for each parameter
	for (auto pb : enablersForParams)
	{
		params_EditorEnablers.add(pb);
	}
}

//params_EditorGroups.add(parameter);
//f0.makeReferenceTo(parameterFloat.cast<float>());
//params_EditorGroups.add(parameter.cast<float>());
//params_EditorGroups.add(parameterFloat);
//ofParameter<float> prop = static_cast<ofParameter<float>&>(parameter);
//ofParameter<float> prop = parameter.cast<float>();
//params_EditorGroups.add(parameter);