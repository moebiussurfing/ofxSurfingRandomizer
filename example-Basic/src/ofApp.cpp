#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);

	setup_ImGui();

	// parameters to randomize
	params.setName("paramsGroup");// main container
	params2.setName("paramsGroup2");// nested
	params3.setName("paramsGroup3");// nested
	params.add(lineWidth.set("lineWidth", 0.5, 0, 1));
	params.add(separation.set("separation", 50, 1, 100));
	params.add(speed.set("speed", 0.5, 0, 1));
	params.add(shapeType.set("shapeType", 0, -50, 50));
	params.add(size.set("size", 100, 0, 100));
	params.add(amount.set("amount", 10, 0, 25));
	params2.add(shapeType2.set("shapeType2", 0, -50, 50));
	params2.add(size2.set("size2", 100, 0, 100));
	params2.add(amount2.set("amount2", 10, 0, 25));
	params3.add(lineWidth3.set("lineWidth3", 0.5, 0, 1));
	params3.add(separation3.set("separation3", 50, 1, 100));
	params3.add(speed3.set("speed3", 0.5, 0, 1));
	params2.add(params3);
	params.add(params2);

	data.setup(params);
}
//--------------------------------------------------------------
void ofApp::setup_ImGui()
{
	ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
	bool bAutoDraw = true;
	bool bRestore = true;
	bool bMouse = false;
	gui.setup(nullptr, bAutoDraw, flags, bRestore, bMouse);

	auto &io = ImGui::GetIO();
	auto normalCharRanges = io.Fonts->GetGlyphRangesDefault();

	//-

	// font
	std::string fontName;
	float fontSizeParam;
	fontName = "telegrama_render.otf"; //  WARNING: will crash if font not present!
	fontSizeParam = 11;

	//-

	std::string _path = "assets/fonts/"; // assets folder
	customFont = gui.addFont(_path + fontName, fontSizeParam, nullptr, normalCharRanges);
	io.FontDefault = customFont;

	// theme
	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
	//ofxSurfingHelpers::ImGui_ThemeModernDark();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	gui.begin();
	{
		bLockMouseByImGui = false;

		//panels sizes
		float xx = 10;
		float yy = 10;
		float ww = PANEL_WIDGETS_WIDTH;
		float hh = PANEL_WIDGETS_HEIGHT;

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

		static bool auto_resize = true;
		ImGuiWindowFlags flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

		ImGui::PushFont(customFont);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));
		{
			ImGui::SetNextWindowPos(ImVec2(10, 10), flagsw);
			ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsw);
			if (ofxImGui::BeginWindow("RANDOMIZER", mainSettings, flagsw))
			{
				ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				data.drawWidgets();
			}
			ofxImGui::EndWindow(mainSettings);

			//--

			ImGui::SetNextWindowPos(ImVec2(300, 10), flagsw);
			ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsw);
			if (ofxImGui::BeginWindow("PARAMETERS", mainSettings, flagsw))
			{
				ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
				ofxImGui::AddGroup(params, mainSettings);
			}
			ofxImGui::EndWindow(mainSettings);
		}
		ImGui::PopStyleVar();
		ImGui::PopFont();
	}
	//mouse lockers
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

	gui.end();
}


