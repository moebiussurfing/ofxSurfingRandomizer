#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Randomizer

	// A. Index
	randomizer.setup(index);

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
	ofClear(colorByIndex);

	randomizer.drawGui();
}