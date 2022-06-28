#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	setupParams();

	//----

	// Randomizer

	// Target A. 
	// Params

	randomizer.setup(params);

	//--

	// Presets Manager

	presetsManager.setDiablePlayer(); // -> Force disable player bc now we have the randomizer player.

	presetsManager.addGroup(params);

	//--

	// Target B. 
	// Index

	// Link index with the Presets Manager selector!
	randomizer.setIndexPtr(presetsManager.index);

	//--
	 
	// Startup
	
	// Force not both keys callbacks enabled to avoid collide!
	presetsManager.bKeys = !randomizer.bKeys;

	// Create some presets
	presetsManager.doPopulatePresetsRandomized();

	//TODO: fix
	// Force visible on first start 
	randomizer.setGuiVisible(true);
	presetsManager.setGuiVisible(true);
}

//--------------------------------------------------------------
void ofApp::setupParams()
{
	// For Target A. 
	// Params to be randomized

	params.setName("myScene");
	params.add(alpha.set("Alpha", 0.5f, 0, 1.0f));
	params.add(size.set("Size", 400, 10, 800));
	params.add(round.set("Round", 0.5, 0, 1));
	params.add(rotation.set("Rotation", 180, 0, 360));
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	//--

	// Draw Gui

	randomizer.drawGui();

	presetsManager.drawGui();
}

//----

// Scene

//--------------------------------------------------------------
void ofApp::drawScene()
{
	static ofColor colorBg = 32;
	ofClear(colorBg);

	ofColor color = ofColor::yellow;

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(glm::vec2(ofGetWidth() / 2, ofGetHeight() / 2));

		// Alpha
		float _a = ofMap(alpha, alpha.getMin(), alpha.getMax(), 0.1, 1);
		ofSetColor(color.r, color.g, color.b, color.a * _a);

		// Size
		float _sz = 100 + size * 0.5f;

		// Rotation
		float _rot = rotation / 3.0;
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(45);
		ofRotateDeg(_rot);

		float _s = ofMap(rotation, rotation.getMin(), rotation.getMax(), 0, 0.1);
		float _scale = 1;

		//--

		// Draw 4 shapes

		drawShape(-_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(-_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(-_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(-_sz / 2, -_sz / 2, _sz);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawShape(int x, int y, int size)
{
	float r = ofMap(round, 0, 1, 0, 30);
	ofDrawRectRounded(x, y, size, size, r);
}