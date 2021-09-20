# ofxSurfingRandomizer

## Overview
An **openFrameworks** add-on to perform Two types of useful Randomizers for your scenes.  
Randomizer result targets over:  

A. The desired **ofParameters** (int/float/bool) of an **ofParameterGroup**.  

B. An **Index** (_int/preset/state..._) selector.  

## Why?

  - Useful to **explore not expected combinations** of the _parameters_ (A) that "modulate" your scene.  

  - Using the **Index Randomizer** (B) you can add some "_organic behavior_" to your scene, switching your scene between different _modes/presets/states_.  

  - A global **BPM timer** can be used to set the speed of the switching, with two different categories for stay duration (**long/short**).  

  - Each allowed index (ie: from 0 to 8) has a settable probability to make some states more or less frequently switched to. 

--------------------------

### 1. RANGED RANDOMIZATION 
* Just pass your ```ofParameterGroup``` parameters container.
* Enable the desired parameters from the group.
* Set the range limits for each parameter to fit the random inside them.
* Notice that limits will be also inside the Min/Max of the parameters but independent of them.
* Trig the randomization.
* Use the Tester player to repeat randomizations.

![image](/readme_images/Capture.PNG?raw=true "image")  

### 2. INDEX RANDOMIZER 
* Initialize the Min-Max of your index/int parameter. (ie: how many presets/states are available)
* Set the probability for each index.
* Set if the index should stay long or short staying duration when her random happens.
* Set the global BPM clock speed.
* Pass your *ofParameter<int>* target index to receive the index randoms (in ofApp).

![image](/readme_images/Capture2.PNG?raw=true "image")  

## Other Features
- Only ```float```, ```int``` and ```bool``` types yet.
- Reset all parameters to the parameter or the range minimum.
- Reset Min/Max ranges to parameter limits.
- Store/Recall a memory state to improve exploring / Undo / Redo.
- Auto Store/Recall all the settings.
- **ImGui** based GUI ready to integrate.
- Added **Undo Engine** to improve exploration.

<details>
  <summary>Usage</summary>
  <p>

**ofApp.h**
```.cpp
#include "ofxSurfingRandomizer.h"

ofxSurfingRandomizer randomizer;

// A. Params
ofParameterGroup params; // group container
ofParameter<float> lineWidth;
ofParameter<float> separation;
ofParameter<float> speed;
ofParameter<int> amount;
ofParameter<int> shapeType;

// B. Index
ofParameter<int> index{ "index", 0, 0, 8 };
ofEventListener listenerIndex;
```

**ofApp.cpp**
```.cpp
void ofApp::setup() 
{
  params.setName("paramsGroup");
  params.add(lineWidth.set("lineWidth", 0.5, 0.0, 1.0));
  params.add(separation.set("separation", 50.0, 1.0, 100.0));
  params.add(speed.set("speed", 0.5, 0.0, 1.0));
  params.add(amount.set("amount", 1, 1, 10));
  params.add(speed.set("shapeType", 0, 0, 3));

  // Setup
  randomizer.setup(params); // A
  randomizer.setTarget(index); // B

  // Lambda callback: 
  // To be notified when the randomized index changed.
  //--------------------------------------------------------------
  listenerIndex = index.newListener([this](int &i) {
    ofLogNotice("ofApp") << "Index: " << i;
  });
}
```
  </p>
</details>

<details>
  <summary>Dependencies</summary>
  <p>

Clone these add-ons and include into the **OF PROJECT GENERATOR**:
* [ofxImGui](https://github.com/Daandelange/ofxImGui/). Fork from @**Daandelange**.  
* [ofxImGuiSurfing](https://github.com/moebiussurfing/ofxImGuiSurfing/) 
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxSurfingUndoHelper](https://github.com/moebiussurfing/ofxSurfingUndoHelper/) [Optional. Can be disabled]  
* [ofxUndo](https://github.com/moebiussurfing/ofxUndo) [Optional. Can be disabled]  
* [ofxWindowApp](https://github.com/moebiussurfing/ofxWindowApp). Only for some examples.  
  </p>
</details>

<details>
  <summary>Tested Systems</summary>
  <p>

  - **Windows 10** / **VS 2017** / **OF ~0.11**
  </p>
</details>

<details>
  <summary>TODO</summary>
  <p>

* Add more types: 2D/3D vectors and colors. Using templates [?] ...  
  </p>
</details>

## Author
An add-on by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

[Twitter](https://twitter.com/moebiussurfing/)  
[YouTube](https://www.youtube.com/channel/UCzUw96_wjmNxyIoFXf84hQg)  
[Instagram](https://www.instagram.com/moebiussurfing/)  
