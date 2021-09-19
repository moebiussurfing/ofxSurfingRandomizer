#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Params to randomize
	params.setName("paramsGroup");
	params.add(size1.set("size1", 1, 0, 2));
	params.add(size2.set("size2", (int)ofGetHeight() / 2, 5, (int)ofGetHeight() / 4));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));

	//--

	// Randomizer

	//randomizer.setAutodraw(true); // -> required when only one ImGui instantiated
	randomizer.setIndexPtr(index);

	// Setup
	randomizer.setup(params);

	// Lambda callback:
	// to receive the randomized index
	//--------------------------------------------------------------
	listenerIndex = index.newListener([this](int &i)
	{
		ofLogNotice("ofApp") << "Index: " << i;

		//example:
		//presets.load(i);

		switch (i)
		{
		case 0: color = ofColor::yellow; break;
		case 1: color = ofColor::orange; break;
		case 2: color = ofColor::red; break;
		case 3: color = ofColor::green; break;
		default: break;
		}
	});
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
	// some code from @Daandelange 
	// -> https://github.com/Daandelange/ofxImGui/tree/master/example-sharedcontext

	static ofColor colorBg = 32;

	ofClear(colorBg);

	ofPushStyle();
	ofPushMatrix();

	ofParameter<ofFloatColor> color1{ "Color1", color };
	ofParameter<ofFloatColor> color2{ "Color2", color };

	int a = ofMap(size1, 0, 2, 255, 170);

	float _scale2 = 0.2f;
	float staticAnimationPos = 1;
	int rectSize = size2 + abs((((ofGetHeight() * _scale2 - size2)*size1))*(staticAnimationPos));

	ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);

	float rot = rotation2 / 3.0;
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
	ofRotateZDeg(rotation1 * 45);
	ofScale(1.3);
	ofRotateDeg(rot);

	int a2 = ofMap(size2, 0, size2.getMax(), 255, 225);

	ofSetColor(color1->r * 255, color1->g * 255, color1->b * 255, color1->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	ofRotateDeg(rot);
	ofSetColor(color1->r * 255, color1->g * 255, color1->b * 255, color1->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	ofRotateDeg(rot);
	ofSetColor(color2->r * 255, color2->g * 255, color2->b * 255, color2->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	ofRotateDeg(rot);
	ofSetColor(color2->r * 255, color2->g * 255, color2->b * 255, color2->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawShape(int type, int x, int y, int size)
{
	float offset = size1 * 15;

	switch (type)
	{

	case 0: // rectangle
	{
		float r = ofMap(rotation1, 0, 2, 0, 20);
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
}