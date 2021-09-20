# ofxSurfingRandomizer

## Overview
An **openFrameworks** add-on to perform Two types of useful Randomizers for your scenes.  
Randomizer result targets over:  

A. The desired **ofParameters** (int/float/bool) of an **ofParameterGroup**.  

B. An **Index** (_int/preset/state..._) selector.  

## Why?

  - Useful to **explore not expected combinations** of the **Randomizer ofParameters** (A) that "modulate" your scene.  

  - Using the **Index Randomizer** (B) you can add some "_organic behavior_" to your scene, switching your scene between different _modes/presets/states_.  

  - A global **BPM timer** can be used to set the speed of the switching, with two different categories for stay duration (**long/short**).  

  - Each allowed index (ie: from 0 to 8) has a settable probability to make some states more or less frequently switched to. 

--------------------------

### 1. RANGED RANDOMIZATION 
#### Usage:  
* 1. Just pass your ```ofParameterGroup``` parameters container.
* 2. Enable the desired parameters from the group.
* 3. Set the range limits for each parameter to fit the random inside them.
* 4. Notice that limits will be also inside the Min/Max of the parameters but independent of them.
* 5. Trig the randomization.
* 6. Use the Tester player to repeat randomizations during tweaking.

![image](/readme_images/Capture.PNG?raw=true "image")  

### 2. INDEX RANDOMIZER 
#### Usage:  
* 1. Initialize the Min-Max of your index/int parameter. (ie: how many presets/states are available)
* 2. Set the probability for each index.
* 3. Set if the index should stay long or short staying duration when her random happens.
* 4. Set the global BPM clock speed.
* 5. Pass your *ofParameter<int>* target index to receive the index randoms (in ofApp).
* 6. Use the Tester player to repeat randomizations during tweaking.

![image](/readme_images/Capture2.PNG?raw=true "image")  

<details>
  <summary>Other Features</summary>
  <p>

- Only ```float```, ```int``` and ```bool``` types yet.
- Reset all parameters to the parameter or the range minimum.
- Reset Min/Max ranges to parameter limits.
- Store/Recall a memory state to improve exploring / Undo / Redo.
- Auto Store/Recall all the settings.
- **ImGui** based GUI ready to integrate.
- Added **Undo Engine** to improve exploration.
  </p>
</details>

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
* [ofxImGui](https://github.com/Daandelange/ofxImGui/). [_FORK_ from @**Daandelange**]  
* [ofxImGuiSurfing](https://github.com/moebiussurfing/ofxImGuiSurfing/)  
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect) [_FORK_]  
* [ofxMSAInteractiveObject](https://github.com/moebiussurfing/ofxMSAInteractiveObject) [_FORK_]  
* [ofxSurfingUndoHelper](https://github.com/moebiussurfing/ofxSurfingUndoHelper/) [_Optional. Can be disabled_]  
* [ofxUndo](https://github.com/moebiussurfing/ofxUndo) [_Optional. Can be disabled_]  
* [ofxWindowApp](https://github.com/moebiussurfing/ofxWindowApp). [_Only for some examples_]  
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
