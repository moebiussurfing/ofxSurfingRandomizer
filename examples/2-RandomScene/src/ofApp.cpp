#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetCircleResolution(120);

	//--

	// A. Params to randomize
	params.setName("paramsGroup");
	params.add(alpha.set("alpha", 0.5f, 0, 1.0f));
	params.add(size.set("size", (int)ofGetHeight() / 2, 10, (int)ofGetHeight() / 4));
	params.add(round.set("round", 0.5, 0, 1));
	params.add(rotation.set("rotation", 180, 0, 360));
	params.add(position.set("position",
		glm::vec2(0.5*ofGetWidth(), 0.5*ofGetHeight()),
		glm::vec2(0.2*ofGetWidth(), 0.2*ofGetHeight()),
		glm::vec2(0.8*ofGetWidth(), 0.8*ofGetHeight())));

	//--

	// Randomizer

	// A. Setup with Params
	randomizer.setup(params);

	// B. Index
	randomizer.setIndexPtr(index);

	// Lambda callback to receive the randomized index target
	//--------------------------------------------------------------
	listenerIndex = index.newListener([this](int &i)
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
	default: break;
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	//// Easy callback to be notified when the randomized index changed:
	//// log when index changes by the randomizer
	//static int indexPre = 0;
	//if (index != indexPre) {
	//	indexPre = index;
	//	ofLogNotice(__FUNCTION__) << "Index: " << index << endl;
	//}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	randomizer.draw_ImGui();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	static ofColor colorBg = 32;
	ofClear(colorBg);

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(position.get());

		// alpha
		float _a = ofMap(alpha, alpha.getMin(), alpha.getMax(), 0.1, 1);
		// size
		float _sz = 100 + size * 1.1;
		// rotation
		float _rot = rotation / 3.0;
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(45);
		ofRotateDeg(_rot);

		ofSetColor(colorByIndex.r, colorByIndex.g, colorByIndex.b, colorByIndex.a * _a);
		drawShape(index, -_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		ofSetColor(colorByIndex.r, colorByIndex.g, colorByIndex.b, colorByIndex.a * _a);
		drawShape(index, -_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		ofSetColor(colorByIndex.r, colorByIndex.g, colorByIndex.b, colorByIndex.a * _a);
		drawShape(index, -_sz / 2, -_sz / 2, _sz);

		ofRotateDeg(_rot);
		ofSetColor(colorByIndex.r, colorByIndex.g, colorByIndex.b, colorByIndex.a * _a);
		drawShape(index, -_sz / 2, -_sz / 2, _sz);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawShape(int type, int x, int y, int size)
{
	// only rectangles
#ifdef USE_ONLY_RECTANGLES
	float r = ofMap(round, 0, 1, 0, 30);
	ofDrawRectRounded(x, y, size, size, r);
#endif

#ifndef USE_ONLY_RECTANGLES
	float offset = alpha * 15;

	switch (type)
	{

	case 0: // rectangle
	{
		float r = ofMap(round, 0, 2, 0, 50);
		ofDrawRectRounded(x, y, size, size, r);
	}
	break;

	case 1: // circle
	{
		ofPushMatrix();
		ofTranslate(+size / offset, -size / offset);
		ofDrawCircle(x, y, size / 2);
		ofPopMatrix();
	}
	break;

	case 2: // triangle
	{
		ofPushMatrix();
		ofTranslate(size / offset, size / offset);
		ofSetPolyMode(OF_POLY_WINDING_NONZERO);
		ofBeginShape();
		ofVertex(0, 0);
		ofVertex(0, size);
		ofVertex(size, 0);
		ofVertex(0, 0);
		ofEndShape();
		ofPopMatrix();
	}
	break;

	case 3: // star
	{
		ofPushMatrix();
		//ofTranslate(-size / 2, -size / 2);
		int nTips = 10;
		int nStarPts = nTips * 2;
		float angleChangePerPt = TWO_PI / (float)nStarPts;
		float innerRadius = size / 2;
		float outerRadius = innerRadius * 0.8;
		float origx = 0;
		float origy = 0;
		float angle = 0;
		ofBeginShape();
		for (int i = 0; i < nStarPts; i++) {
			if (i % 2 == 0) {
				// inside point:
				float x = origx + innerRadius * cos(angle);
				float y = origy + innerRadius * sin(angle);
				ofVertex(x, y);
			}
			else {
				// outside point
				float x = origx + outerRadius * cos(angle);
				float y = origy + outerRadius * sin(angle);
				ofVertex(x, y);
			}
			angle += angleChangePerPt;
		}
		ofEndShape();
		ofPopMatrix();
	}
	break;

	default:
		break;
	}
#endif
}