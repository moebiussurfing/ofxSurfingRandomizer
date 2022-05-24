#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Group
	params.setName("paramsGroup");
	params.add(size1.set("size1", 0.5, 0, 1));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));
	params.add(indexColor.set("indexColor", 0, 0, 2));

	setupRandomizer();
}

//--------------------------------------------------------------
void ofApp::setupRandomizer()
{
	// All the added parameters will auto-receive the randomizations

	// Randomizer

	randomizer.setIndexPtr(index); // -> Must be setted before call setup.

	randomizer.setup(params);

	//--
	// 
	// B. Lambda callback:
	// To receive the randomized index target
	//--------------------------------------------------------------
	listenerIndex = index.newListener([this](int &i) {
		ofLogNotice("ofApp") << "Index: " << i;

		indexColor = i;

		// Other Example:
		// -> Typical use for an int index...
		//presets.load(i); 
	});
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	//--

	randomizer.draw_ImGui();

	//// B. Easy callback:
	//// Log when index target changes by the randomizer:
	//
	//static int indexPre = 0;
	//if (index != indexPre) {
	//	indexPre = index;
	//	ofLogNotice(__FUNCTION__) << "Index: " << index << endl;
	//}
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	//-

	// Some preprocess
	//size1 += 0.2;
	float _scale2 = size1 / 5.f;
	int _rSz = size2 + (ofGetHeight() * _scale2);
	float _rot = rotation2 / 3.0;
	float _rat = ofMap(rotation2, 0, 360, 1, 0.5f + size1 * 0.4);
	float _rag = ofMap(size1, 0, 1, 0, -30);

	//-

	// Bg Color
	ofColor _colorBg = 255;//white

	// Shape Color
	ofColor _color;
	switch (indexColor)
	{
	case 0: _color = ofColor::black; break;
	case 1: _color = ofColor::orange; break;
	case 2: _color = ofColor::green; break;
	default: _color = ofColor::red; break;
	}

	int _alpha = ofMap(size2, 0, size2.getMax(), 200, 245);
	ofColor c(_color.r, _color.g, _color.b, _alpha);

	//--

	// Draw the Scene

	ofClear(_colorBg);

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth() * .5f, ofGetHeight() * .5f);
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(rotation1 * 45);
		ofScale(1.3f);
		ofRotateDeg(_rot);

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);
	}
	ofPopMatrix();
	ofPopStyle();
}