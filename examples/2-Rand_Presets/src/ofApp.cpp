#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	setupParams();

	//----

	// Randomizer

	//--

	// Target A. 
	// Setup with Params

	randomizer.setup(params);
	//randomizer.setGuiVisible(true);

	//--

	// Target B. 
	// Index

	randomizer.setIndexPtr(index);

	//// Lambda callback to receive the randomized index target
	////--------------------------------------------------------------
	//listenerIndex = index.newListener([this](int& i)
	//	{
	//		ofLogNotice("ofApp") << "Index: " << index.get();

	//		////-> Will set the color getting the index.
	//		//refreshColorByIndex();

	//		// Use the index as selector on the Presets Manager
	//		presetsManager.load(index);
	//	});

	//refreshColorByIndex();

	//----

	// Presets Manager

	presetsManager.addGroup(params);
	//presetsManager.setGuiVisible(true);

	// Force not both keys callbacks enabled to avoid collide!
	presetsManager.bKeys = !randomizer.bKeys;

	// Link index with the Presets Manager selector!
	index.makeReferenceTo(presetsManager.index);
}

//--------------------------------------------------------------
void ofApp::setupParams()
{
	// For Target A. 
	// Params to be randomized

	params.setName("paramsGroup");
	params.add(alpha.set("alpha", 0.5f, 0, 1.0f));
	params.add(size.set("size", 400, 10, 800));
	params.add(round.set("round", 0.5, 0, 1));
	params.add(rotation.set("rotation", 180, 0, 360));
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
		//ofTranslate(position.get());

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

		// Draw 4 shapes

		drawShape(index, -_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(index, -_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(index, -_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		_scale -= _s;
		ofScale(_scale);
		drawShape(index, -_sz / 2, -_sz / 2, _sz);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawShape(int type, int x, int y, int size)
{
	float r = ofMap(round, 0, 1, 0, 30);
	ofDrawRectRounded(x, y, size, size, r);
}
