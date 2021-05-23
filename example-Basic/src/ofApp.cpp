#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);

	//required call
	ImGuiConfigFlags flags;
	flags = ImGuiConfigFlags_DockingEnable;
	bool bMouse = false;//false to auto show on window workflow
	bool bAutoDraw = true;
	bool bRestore = true;
	gui.setup(nullptr, bAutoDraw, flags, bRestore, bMouse);

	params.setName("sceneParamsGroup");// main group 
	paramsNested1.setName("styleSubGroup");// another nested group
	paramsNested2.setName("itemSubGroup");// another nested group

	paramsNested1.add(fill.set("fill", true));
	paramsNested1.add(color.set("color", ofColor(0, 255), ofColor(0, 0), ofColor(255, 255)));
	paramsNested1.add(lineWidth.set("lineWidth", 1, 0.1, 10));

	paramsNested2.add(shapeType.set("shapeType", 1, 1, 2));
	paramsNested2.add(size.set("size", 100, 5, 200));
	paramsNested2.add(paramsNested1);

	params.add(amount.set("amount", 10, 1, 24));
	params.add(separation.set("separation", 10, 1, 100));
	params.add(paramsNested2);


	randomizerGroup.setup(params);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	auto mainSettings = ofxImGui::Settings();

	gui.begin();
	{
		ImGuiCond flagsw;
		flagsw = ImGuiCond_Once;

		float ww = PANEL_WIDGETS_WIDTH;
		float hh = 600;
		float pad = 10;

		ImGui::SetNextWindowPos(ImVec2(10, 10), flagsw);
		ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsw);
		if (ofxImGui::BeginWindow("RANDOMIZER", mainSettings, flagsw))
		{
			randomizerGroup.drawWidgets();
		}
		ofxImGui::EndWindow(mainSettings);


		ImGui::SetNextWindowPos(ImVec2(300, 10), flagsw);
		ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsw);
		if (ofxImGui::BeginWindow("PARAMETERS", mainSettings, flagsw))
		{
			ofxImGui::AddGroup(params, mainSettings);
		}
		ofxImGui::EndWindow(mainSettings);
	}
	gui.end();

}


