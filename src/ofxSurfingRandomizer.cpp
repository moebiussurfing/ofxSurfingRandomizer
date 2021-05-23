#include "ofxSurfingRandomizer.h"

//--------------------------------------------------------------
ofxSurfingRandomizer::ofxSurfingRandomizer() {
	//ofAddListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	//ofAddListener(ofEvents().keyPressed, this, &ofxSurfingRandomizer::keyPressed);
}

//--------------------------------------------------------------
ofxSurfingRandomizer::~ofxSurfingRandomizer() {
	//ofRemoveListener(ofEvents().update, this, &ofxSurfingRandomizer::update);
	//ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingRandomizer::keyPressed);
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::setup(ofParameterGroup& group) {
	params = group;

	//TODO:
	setup_RandomizerPowered(group);

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

		name = "Parameters";
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

		name = "Controls";
		if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw))
		{
			ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

			if (ImGui::Button("RANDOMIZE!", ImVec2(_w100, _h)))
			{
				doRandomize();
			}
			if (ImGui::Button("RESET", ImVec2(_w100, _h)))
			{
				doReset();
			}
			ImGui::Dummy(ImVec2(0.0f, 2.0f));

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
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			// enable toggles
			for (int i = 0; i < params_RandomizersPowered_Toggles.size(); i++)
			{
				auto &p = params_RandomizersPowered_Toggles[i];// ofAbstractParameter
				auto type = p.type();
				bool isBool = type == typeid(ofParameter<bool>).name();
				//bool isGroup = type == typeid(ofParameterGroup).name();
				//bool isFloat = type == typeid(ofParameter<float>).name();
				//bool isInt = type == typeid(ofParameter<int>).name();
				string name = p.getName();

				if (isBool) {
					ofParameter<bool> pb = p.cast<bool>();
					ofxSurfingHelpers::AddBigToggle(pb, _w100, _h / 2, false);

					//auto tmpRef = pb.get();
					//const auto& info = typeid(ParameterType); 
					//if (ImGui::Checkbox(GetUniqueName(parameter), (bool *)&tmpRef))
					//{
					//	parameter.set(tmpRef);
					//	return true;
					//}
				}
			}

			//TODO:
			//// simple ranges
			//for (int i = 0; i < params_RandomizersPowered_Toggles.size(); i++)
			//{
			//	auto &p = params_RandomizersPowered_Toggles[i];// ofAbstractParameter
			//	auto type = p.type();
			//	bool isBool = type == typeid(ofParameter<bool>).name();
			//	string name =
			//		ofxImGui::AddRange("range", parameterMin, parameterMax, speed);
			//}

			//-

			guiManager.drawAdvancedSubPanel();
		}
		ofxImGui::EndWindow(mainSettings);

		//-

		name = "Randomizers";
		if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagsw))
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
			flags |= ImGuiTreeNodeFlags_DefaultOpen;
			flags |= ImGuiTreeNodeFlags_Framed;
			ofxImGui::AddGroup(params_RandomizersPowered_Groups, flags);
			//ofxImGui::AddGroup(params_RandomizersPowered_Groups, mainSettings);

			//bool ofxImGui::AddRange(const std::string& name, ofParameter<float>& parameterMin, ofParameter<float>& parameterMax, float speed)
		}
		ofxImGui::EndWindow(mainSettings);
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

	ofxImGui::AddGroup(params, flags);
	//ofxImGui::AddGroup(params, mainSettings, flags);
	//ofxImGui::AddGroup(params, mainSettings);

	//ImGui::Dummy(ImVec2(0.0f, 5.0f));
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doSetAll(bool b) {
	ofLogNotice(__FUNCTION__) << b;

	for (int i = 0; i < params_RandomizersPowered_Toggles.size(); i++)
	{
		auto &p = params_RandomizersPowered_Toggles[i];//ofAbstractParameter
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
void ofxSurfingRandomizer::doReset() {
	ofLogNotice(__FUNCTION__);

	for (auto p : randomizersPowered_TogglesVector)
	{
		if (!p.get()) continue;//only reset this param if it's enabled

		//-

		string name = p.getName();//name
		auto &g = params_RandomizersPowered_Groups.getGroup(name);//ofParameterGroup
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
void ofxSurfingRandomizer::doRandomize() {
	ofLogNotice(__FUNCTION__);

	for (auto p : randomizersPowered_TogglesVector)
	{
		if (!p.get()) continue;//only reset this param if it's enabled

		//-

		string name = p.getName();//name
		auto &g = params_RandomizersPowered_Groups.getGroup(name);//ofParameterGroup
		auto &e = g.get(name);//ofAbstractParameter

		auto type = e.type();
		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();

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
	}
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::exit() {
}

//--

//TODO:
//randomizer powered
//better random engine with min- max for the params

//--------------------------------------------------------------
void ofxSurfingRandomizer::setup_RandomizerPowered(ofParameterGroup& group)
{
	params_RandomizersPowered_Groups.clear();
	params_RandomizersPowered_Groups.setName("Powered Randomizer");

	randomizersPowered_TogglesVector.clear();

	params_RandomizersPowered_Toggles.clear();
	params_RandomizersPowered_Toggles.setName("Params");

	addGroup_ToRandomizerPowered(group);
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::addGroup_ToRandomizerPowered(ofParameterGroup& group) {

	for (auto pa : group)//ofAbstractParameter
	{
		// exclude params not marked as serializable
		if (!pa->isSerializable()) continue;
		//if (!parameter->isSerializable()) return;

		//--

		// group
		auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(pa);
		if (parameterGroup)
		{
			if (!parameterGroup->isSerializable()) continue;
			//if (!parameterGroup->isSerializable()) return;

			addGroup_ToRandomizerPowered(*parameterGroup);
			continue;
		}

		{
			// parameter, try everything we know how to handle.

			//auto type = pa->type();
			//bool isFloat = type == typeid(ofParameter<float>).name();
			//bool isInt = type == typeid(ofParameter<int>).name();
			//bool isBool = type == typeid(ofParameter<bool>).name();

//			// x,y,z vectors
//#if OF_VERSION_MINOR >= 10
//			auto parameterVec2f = std::dynamic_pointer_cast<ofParameter<glm::vec2>>(parameter);
//			if (parameterVec2f)
//			{
//				ofParameter<bool> b{ parameterVec2f->getName(), false };
//				//params_RandomizersPowered_Groups.push_back(b);
//				continue;
//			}
//			auto parameterVec3f = std::dynamic_pointer_cast<ofParameter<glm::vec3>>(parameter);
//			if (parameterVec3f)
//			{
//				ofParameter<bool> b{ parameterVec3f->getName(), false };
//				//params_RandomizersPowered_Groups.push_back(b);
//				continue;
//			}
//			auto parameterVec4f = std::dynamic_pointer_cast<ofParameter<glm::vec4>>(parameter);
//			if (parameterVec4f)
//			{
//				ofParameter<bool> b{ parameterVec4f->getName(), false };
//				//params_RandomizersPowered_Groups.push_back(b);
//				continue;
//			}
//#endif
//			auto parameterOfVec2f = std::dynamic_pointer_cast<ofParameter<ofVec2f>>(parameter);
//			if (parameterOfVec2f)
//			{
//				//ofxImGui::AddParameter(*parameterOfVec2f);
//				ofParameter<bool> b{ parameterOfVec2f->getName(), false };
//				//params_RandomizersPowered_Groups.push_back(b);
//				continue;
//			}
//			auto parameterOfVec3f = std::dynamic_pointer_cast<ofParameter<ofVec3f>>(parameter);
//			if (parameterOfVec3f)
//			{
//				//ofxImGui::AddParameter(*parameterOfVec3f);
//				ofParameter<bool> b{ parameterOfVec3f->getName(), false };
//				//params_RandomizersPowered_Groups.push_back(b);
//				continue;
//			}
//			auto parameterOfVec4f = std::dynamic_pointer_cast<ofParameter<ofVec4f>>(parameter);
//			if (parameterOfVec4f)
//			{
//				ofParameter<bool> b{ parameterOfVec4f->getName(), false };
//				//params_RandomizersPowered_Groups.push_back(b);
//				//ofxImGui::AddParameter(*parameterOfVec4f);
//				continue;
//			}
			// colors
			//auto parameterColor = std::dynamic_pointer_cast<ofParameter<ofColor>>(parameter);
			//if (parameterColor)
			//{
			//	ofParameter<bool> b{ parameterColor->getName(), false };
			//	randomizersPowered_TogglesVector.push_back(b);
			//	continue;
			//}
			//auto parameterFloatColor = std::dynamic_pointer_cast<ofParameter<ofFloatColor>>(parameter);
			//if (parameterFloatColor)
			//{
			//	ofParameter<bool> b{ parameterFloatColor->getName(), false };
			//	randomizersPowered_TogglesVector.push_back(b);
			//	continue;
			//}

			//-

			// normal types

			// float
			auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(pa);
			if (parameterFloat)
			{
				std::string _name = parameterFloat->getName();

				float vtot = parameterFloat->getMax() - parameterFloat->getMin();
				float vmin = parameterFloat->getMin() + DEFAULT_MIN_PCT * vtot;
				float vmax = parameterFloat->getMin() + DEFAULT_MAX_PCT * vtot;

				ofParameterGroup _g{ _name };
				ofParameter<bool> b{ "Enable " + _name, false };
				ofParameter<float> p0 = group.getFloat(_name);
				ofParameter<float> pmin{ "Min", vmin, parameterFloat->getMin() , parameterFloat->getMax() };
				ofParameter<float> pmax{ "Max", vmax, parameterFloat->getMin() , parameterFloat->getMax() };


				//_g.add(b);
				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_RandomizersPowered_Groups.add(_g);

				//-

				ofParameter<bool> b0{ _name, false };
				randomizersPowered_TogglesVector.push_back(b0);
				//params_RandomizersPowered_Toggles.add(b0);

				continue;
			}

			// int
			auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(pa);
			if (parameterInt)
			{
				std::string _name = parameterInt->getName();

				int vtot = parameterInt->getMax() - parameterInt->getMin();
				int vmin = parameterInt->getMin() + DEFAULT_MIN_PCT * vtot;
				int vmax = parameterInt->getMin() + DEFAULT_MAX_PCT * vtot;

				ofParameterGroup _g{ _name };
				ofParameter<bool> b{ "Enable " + _name, false };
				ofParameter<int> p0 = group.getInt(_name);
				ofParameter<int> pmin{ "Min", vmin, parameterInt->getMin() , parameterInt->getMax() };
				ofParameter<int> pmax{ "Max", vmax, parameterInt->getMin() , parameterInt->getMax() };

				//_g.add(b);
				_g.add(p0);
				_g.add(pmin);
				_g.add(pmin);

				params_RandomizersPowered_Groups.add(_g);

				//-

				ofParameter<bool> b0{ _name, false };
				randomizersPowered_TogglesVector.push_back(b0);
				//params_RandomizersPowered_Toggles.add(b0);

				continue;
			}

			////TODO:
			//// bool
			auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(pa);
			if (parameterBool)
			{
				//	//std::string _name = parameterBool->getName();

				//	//ofParameterGroup _g{ _name };
				//	//ofParameter<bool> b{ "Enable " + _name, false };
				//	//ofParameter<int> p0 = group.getBool(_name);

				//	////_g.add(b);
				//	//_g.add(p0);

				//	//params_RandomizersPowered_Groups.add(_g);

				//	////-

				//	//ofParameter<bool> b0{ _name, false };
				//	//randomizersPowered_TogglesVector.push_back(b0);
				//	//params_RandomizersPowered_Toggles.add(b0);

				continue;
			}

			ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << pa->getName();
		}

		//if (parameterGroup) return;//skip add group below
	}

	//----

	// create an enabler bool/toggle for each parameter
	for (auto pb : randomizersPowered_TogglesVector)
	{
		params_RandomizersPowered_Toggles.add(pb);
	}
}

//params_RandomizersPowered_Groups.add(parameter);
//f0.makeReferenceTo(parameterFloat.cast<float>());
//params_RandomizersPowered_Groups.add(parameter.cast<float>());
//params_RandomizersPowered_Groups.add(parameterFloat);
//ofParameter<float> prop = static_cast<ofParameter<float>&>(parameter);
//ofParameter<float> prop = parameter.cast<float>();
//params_RandomizersPowered_Groups.add(parameter);