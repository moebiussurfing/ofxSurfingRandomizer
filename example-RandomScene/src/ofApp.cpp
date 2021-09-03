#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// params to randomize
	params.setName("paramsGroup"); // main container
	params.add(size1.set("size1", 1, 0, 2));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation.set("rotation", 1, 0, 2));
	params.add(rotationOffset.set("rotationOffset", 180, 0, 360));

	//--

	// Randomizer

	//randomizer.setAutodraw(true); // -> required when only one ImGui instantiated
	randomizer.setIndexPtr(index);

	// setup
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
	//// Easy callback:
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

	// some code from @Daandelange 
	// -> https://github.com/Daandelange/ofxImGui/tree/master/example-sharedcontext

	//ofParameter<ofFloatColor> background{ "Background", ofFloatColor::black };
	//ofParameter<ofFloatColor> foreground{ "Foreground", ofFloatColor::black };

	ofParameter<ofFloatColor> background{ "Background", color };
	ofParameter<ofFloatColor> foreground{ "Foreground", color};

	//ImVec4 color = { 0,0,0,1.0 };

	int a = ofMap(size1, 0, 2, 255, 170);

	//ofSetColor(color.x * 255, color.y * 255, color.z * 255, color.w * a);
	////ofSetColor(color.x * 255, color.y * 255, color.z * 255, color.w * 255);
	//float _scale = 0.13f;
	//float r = ofGetHeight()*_scale*size1*(1);
	////ofDrawCircle(ofGetWidth()*0.5f, ofGetHeight()*0.5f, r);
	//drawShape(index, ofGetWidth()*0.5f, ofGetHeight()*0.5f, r);

	float _scale2 = 0.2f;
	float staticAnimationPos = 1;
	int rectSize = size2 + abs((((ofGetHeight() * _scale2 - size2)*size1))*(staticAnimationPos));

	ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);

	float rot = rotationOffset / 3.0;
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
	ofRotateZDeg(rotation * 45);
	ofScale(1.3);
	ofRotateDeg(rot);

	int a2 = ofMap(size2, 0, size2.getMax(), 255, 225);

	ofSetColor(background->r * 255, background->g * 255, background->b * 255, background->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	ofRotateDeg(rot);
	ofSetColor(background->r * 255, background->g * 255, background->b * 255, background->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	ofRotateDeg(rot);
	ofSetColor(foreground->r * 255, foreground->g * 255, foreground->b * 255, foreground->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	ofRotateDeg(rot);
	ofSetColor(foreground->r * 255, foreground->g * 255, foreground->b * 255, foreground->a * a2);
	drawShape(index, -rectSize * .5f, -rectSize * .5f, rectSize);

	//// inner circle
	//ofFill();
	//ofSetColor(colorBg);
	//int r2 = ofMap(_size1, 0, size1.getMax(), 3, 20, true);
	//ofDrawCircle(0,0, r2);

	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawShape(int type, int x, int y, int size)
{
	float offset = size1 * 15;
	//float offset = 10;

	switch (type)
	{

	case 0://rectangle
		ofDrawRectangle(x, y, size, size);
		break;

	case 1://circle
		ofPushMatrix();
		ofTranslate(+size / offset, -size / offset);
		ofDrawCircle(x, y, size / 2);
		ofPopMatrix();
		break;

	case 2://triangle
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
		break;

	case 3://star
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
	//ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);
}