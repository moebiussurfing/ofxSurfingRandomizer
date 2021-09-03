#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// params to randomize
	params.setName("paramsGroup");// main container
	params2.setName("paramsGroup2");// nested
	params3.setName("paramsGroup3");// nested
	params.add(lineWidth.set("lineWidth", 0.5, 0, 1));
	params.add(separation.set("separation", 50, 1, 100));
	params.add(speed.set("speed", 0.5, 0, 1));
	params.add(shapeType.set("shapeType", 0, -50, 50));
	params.add(size.set("size", 100, 0, 100));
	params.add(amount.set("amount", 10, 0, 25));
	params.add(bPrevious.set("prev", false));//bools
	params.add(bNext.set("next", false));
	params2.add(shapeType2.set("shapeType2", 0, -50, 50));
	params2.add(size2.set("size2", 100, 0, 100));
	params2.add(amount2.set("amount2", 10, 0, 25));
	params.add(params2);

	setupRandomizer();
}

//--------------------------------------------------------------
void ofApp::setupRandomizer()
{
	// All the added parameters will auto-receive the randomizations

	// Randomizer
	//randomizer.setAutodraw(true); // -> required when only one ImGui instantiated
	randomizer.setIndexPtr(index);
	randomizer.setup(params);

	// A. Lambda callback:
	// To receive the randomized index target
	//--------------------------------------------------------------
	listenerIndex = index.newListener([this](int &i) {
		ofLogNotice("ofApp") << "Index: " << i;

		//presets.load(i); // -> Typical use for an int index...
	});
}

//--------------------------------------------------------------
void ofApp::draw()
{
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