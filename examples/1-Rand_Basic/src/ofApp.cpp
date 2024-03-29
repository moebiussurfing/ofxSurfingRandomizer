#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetCircleResolution(120);

	//--

	// A. Params to randomize
	params.setName("paramsGroup");
	params.add(alpha.set("alpha", 0.5f, 0, 1.0f));
	params.add(size.set("size", (int)ofGetWidth() / 2, 10, (int)ofGetWidth() / 4));
	params.add(round.set("round", 0.5, 0, 1));
	params.add(rotation.set("rotation", 180, 0, 360));

	//--

	// Randomizer

	// A. Setup with Params
	randomizer.addGroup(params);

	// B. Index
	randomizer.setIndexPtr(index);

	randomizer.setup();

	// Lambda callback to receive the randomized index target
	//--------------------------------------------------------------
	listenerIndex = index.newListener([this](int& i)
		{
			ofLogNotice("ofApp") << "Index: " << index.get();

			refreshColorByIndex(); //-> Will set the color getting the index.

			// Example for another common usage:
			// presets.load(index);
		});

	refreshColorByIndex();
}

//--------------------------------------------------------------
void ofApp::refreshColorByIndex()
{
	switch (index)
	{
	case 0: colorByIndex = ofColor::green; break;
	case 1: colorByIndex = ofColor::yellow; break;
	case 2: colorByIndex = ofColor::orange; break;
	case 3: colorByIndex = ofColor::red; break;
	case 4: colorByIndex = ofColor::blue; break;
	default: break;
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	randomizer.drawGui();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	static ofColor colorBg = 32;
	ofClear(colorBg);

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

		// alpha
		float _a = ofMap(alpha, alpha.getMin(), alpha.getMax(), 0.1, 1);
		ofSetColor(colorByIndex.r, colorByIndex.g, colorByIndex.b, colorByIndex.a * _a);

		// size
		float _sz = 100 + size /3;

		// rotation
		float _rot = rotation / 3.0;
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(45);
		ofRotateDeg(_rot);

		float _s = ofMap(rotation, rotation.getMin(), rotation.getMax(), 0, 0.1);
		float _scale = 1;

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
	// Only rectangles
	float r = ofMap(round, 0, 1, 0, 30);
	ofDrawRectRounded(x, y, size, size, r);
}