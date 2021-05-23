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
	//TODO:
	setup_RandomizerPowered(group);

	// gui
	guiManager.setup(gui);
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
		drawWidgets();
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::drawWidgets() {
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	auto mainSettings = ofxImGui::Settings();

	float _spcx;
	float _spcy;
	float _w100;
	float _h100;
	float _w99;
	float _w50;
	float _w33;
	float _w25;
	float _h;
	ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	//toggles
	ofxImGui::AddGroup(params_RandomizersPowered_Toggles, mainSettings);

	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	//settings
	ofxImGui::AddGroup(params_RandomizersPowered, mainSettings);

	//ofxImGui::AddRange();

	ImGui::Dummy(ImVec2(0.0f, 5.0f));

    if (ImGui::Button("RANDOMIZE!", ImVec2(_w100, _h)))
    {
        doRamdomize();
    }
    if (ImGui::Button("RESET", ImVec2(_w100, _h)))
    {
        doReset();
    }
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doReset() {
    for (auto p : randomizersPowered_TogglesVector)
    {
        string name = p.getName();
        auto &g = params_RandomizersPowered.getGroup(name);
        auto &e = g.get(name);

         cout <<name << endl;

        if (e.type() == typeid(ofParameter<float>).name())
        {
            auto &p1 = g.getFloat("Min").get();
            auto &p2 = g.getFloat("Max").get();
            ofParameter<float> p0 = e.cast<float>();
            //auto p1 = min.cast<float>().get();
            //auto p2 = max.cast<float>().get();
            p0.set((float)(p1));
            cout <<p1 << endl;
            cout <<p2 << endl;
            cout <<p0 << endl;
        }

        else if (e.type() == typeid(ofParameter<int>).name())
        {
            auto &p1 = g.getInt("Min").get();
            auto &p2 = g.getInt("Max").get();

            ofParameter<int> p0 = e.cast<int>();
            //auto p1 = min.cast<int>().get();
            //auto p2 = max.cast<int>().get();
            p0.set((int)(p1));
        }
    }
}

//--------------------------------------------------------------
void ofxSurfingRandomizer::doRamdomize() {

	for (auto p : randomizersPowered_TogglesVector)
	{
		if (p.get())
		{
			string name = p.getName();
			cout << "toggle name:" << p << endl;
			cout << name << endl << endl;

			auto &g = params_RandomizersPowered.getGroup(name);

			cout << "g.toString(" << endl;
			cout << g.toString() << endl;
			cout << "getGroupHierarchyNames" << endl;

			for (auto pp : g.getGroupHierarchyNames()) {
				cout << pp << endl;
			}
				
			auto &e = g.get(name);
			
			if (e.type() == typeid(ofParameter<float>).name())
			{
				auto &p1 = g.getFloat("Min").get();
				auto &p2 = g.getFloat("Max").get();
                ofParameter<float> p0 = e.cast<float>();
				//auto p1 = min.cast<float>().get();
				//auto p2 = max.cast<float>().get();
				p0.set((float)ofRandom(p1, p2));
                cout <<p1 << endl;
                cout <<p2 << endl;
                cout <<p0 << endl;
            }

			else if (e.type() == typeid(ofParameter<int>).name())
			{
				auto &p1 = g.getInt("Min").get();
				auto &p2 = g.getInt("Max").get();

				ofParameter<int> p0 = e.cast<int>();
				//auto p1 = min.cast<int>().get();
				//auto p2 = max.cast<int>().get();
				p0.set((int)ofRandom(p1, p2));
			}

			//auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(p);
			//if (parameterFloat)
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
	params_RandomizersPowered.setName("Powered Randomizer");
	params_RandomizersPowered.clear();

	randomizersPowered_TogglesVector.clear();
	//randomizersPowered_Vector.clear();

	params_RandomizersPowered_Toggles.setName("Params");
	addGroupToRandomizerPowered(group);

}

//--------------------------------------------------------------
void ofxSurfingRandomizer::addGroupToRandomizerPowered(ofParameterGroup& group) {

	for (auto parameter : group)
	{
		// exclude params not marked as serializable
		if (!parameter->isSerializable()) return;

		// group
		auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(parameter);
		if (parameterGroup)
		{
			if (!parameterGroup->isSerializable()) return;

			addGroupToRandomizerPowered(*parameterGroup);
			continue;
		}


		{
			// parameter, try everything we know how to handle.

//			// x,y,z vectors
//#if OF_VERSION_MINOR >= 10
//			auto parameterVec2f = std::dynamic_pointer_cast<ofParameter<glm::vec2>>(parameter);
//			if (parameterVec2f)
//			{
//				ofParameter<bool> b{ parameterVec2f->getName(), false };
//				//params_RandomizersPowered.push_back(b);
//				continue;
//			}
//			auto parameterVec3f = std::dynamic_pointer_cast<ofParameter<glm::vec3>>(parameter);
//			if (parameterVec3f)
//			{
//				ofParameter<bool> b{ parameterVec3f->getName(), false };
//				//params_RandomizersPowered.push_back(b);
//				continue;
//			}
//			auto parameterVec4f = std::dynamic_pointer_cast<ofParameter<glm::vec4>>(parameter);
//			if (parameterVec4f)
//			{
//				ofParameter<bool> b{ parameterVec4f->getName(), false };
//				//params_RandomizersPowered.push_back(b);
//				continue;
//			}
//#endif
//			auto parameterOfVec2f = std::dynamic_pointer_cast<ofParameter<ofVec2f>>(parameter);
//			if (parameterOfVec2f)
//			{
//				//ofxImGui::AddParameter(*parameterOfVec2f);
//				ofParameter<bool> b{ parameterOfVec2f->getName(), false };
//				//params_RandomizersPowered.push_back(b);
//				continue;
//			}
//			auto parameterOfVec3f = std::dynamic_pointer_cast<ofParameter<ofVec3f>>(parameter);
//			if (parameterOfVec3f)
//			{
//				//ofxImGui::AddParameter(*parameterOfVec3f);
//				ofParameter<bool> b{ parameterOfVec3f->getName(), false };
//				//params_RandomizersPowered.push_back(b);
//				continue;
//			}
//			auto parameterOfVec4f = std::dynamic_pointer_cast<ofParameter<ofVec4f>>(parameter);
//			if (parameterOfVec4f)
//			{
//				ofParameter<bool> b{ parameterOfVec4f->getName(), false };
//				//params_RandomizersPowered.push_back(b);
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

			// normal types
			auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(parameter);
			if (parameterFloat)
			{
				std::string _name = parameterFloat->getName();

				float vtot = parameterFloat->getMax() - parameterFloat->getMin();
				float vmin = parameterFloat->getMin() + DEFALUT_MIN_PCT * vtot;
				float vmax = parameterFloat->getMin() + DEFALUT_MAX_PCT * vtot;

				ofParameterGroup _g{ _name };
				ofParameter<bool> b{ "Enable " + _name, false };
				ofParameter<float> p0 = group.getFloat(_name);
				ofParameter<float> p1{ "Min", vmin, parameterFloat->getMin() , parameterFloat->getMax() };
				ofParameter<float> p2{ "Max", vmax, parameterFloat->getMin() , parameterFloat->getMax() };

				_g.add(b);
				_g.add(p0);
				_g.add(p1);
				_g.add(p2);

				params_RandomizersPowered.add(_g);

				ofParameter<bool> b0{ _name, false };
				randomizersPowered_TogglesVector.push_back(b0);

				continue;
			}

			auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(parameter);
			if (parameterInt)
			{
				std::string _name = parameterInt->getName();

				int vtot = parameterInt->getMax() - parameterInt->getMin();
				int vmin = parameterInt->getMin() + DEFALUT_MIN_PCT * vtot;
				int vmax = parameterInt->getMin() + DEFALUT_MAX_PCT * vtot;

				ofParameterGroup _g{ _name };
				ofParameter<bool> b{ "Enable " + _name, false };
				ofParameter<int> p0 = group.getInt(_name);
				ofParameter<int> p1{ " Min", vmin, parameterInt->getMin() , parameterInt->getMax() };
				ofParameter<int> p2{ " Max", vmax, parameterInt->getMin() , parameterInt->getMax() };

				_g.add(b);
				_g.add(p0);
				_g.add(p1);
				_g.add(p2);

				params_RandomizersPowered.add(_g);

				ofParameter<bool> b0{ _name, false };
				randomizersPowered_TogglesVector.push_back(b0);

				continue;
			}

			auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(parameter);
			if (parameterBool)
			{
				std::string _name = parameterBool->getName();

				ofParameterGroup _g{ _name };
				ofParameter<bool> b{ "Enable " + _name, false };

				_g.add(b);

				params_RandomizersPowered.add(_g);

				ofParameter<bool> b0{ _name, false };
				randomizersPowered_TogglesVector.push_back(b0);

				continue;
			}

			ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << parameter->getName();
		}
	}

	for (auto b : randomizersPowered_TogglesVector)
	{
		params_RandomizersPowered_Toggles.add(b);
	}
}


//params_RandomizersPowered.add(parameter);
//f0.makeReferenceTo(parameterFloat.cast<float>());
//params_RandomizersPowered.add(parameter.cast<float>());
//params_RandomizersPowered.add(parameterFloat);
//ofParameter<float> prop = static_cast<ofParameter<float>&>(parameter);
//ofParameter<float> prop = parameter.cast<float>();
//params_RandomizersPowered.add(parameter);
