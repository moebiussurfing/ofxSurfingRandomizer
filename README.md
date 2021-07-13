# ofxSurfingRandomizer

## Overview
An **openFrameworks** add-on to perform two types of useful randomizers over the ofParameters of an ofParameterGroup and over an int/index/preset/state selector.  

Useful to explore not expected combinations of the parameters that modulate your scene.  

Using the index randomizer you can add some organic behavior to your scene, switching your scene between different index / presets / states.  

A global BPM timer can be used to set the speed of the switching, with two different categories (long/short).  

Each state/index / preset has a settable probability to make some states more or less frequently switched. 

#### RANGED RANDOMIZATION 
* Enable the desired parameters from the group.
* Set the range limits for each parameter to fit the random inside.
* Trig the randomization!

![image](/readme_images/Capture.PNG?raw=true "image")  

#### INDEX RANDOMIZER 
* Set the probability for each index.
* Set if the index should stay long or short staying duration.
* Set the global BPM clock speed.

![image](/readme_images/Capture2.PNG?raw=true "image")  

## Other Features
- Only ```float```, ```int``` and ```bool``` types yet.
- Reset all parameters to the parameter or the range minimum.
- Reset Min/Max ranges to parameter limits.
- Store/Recall a memory state to improve exploring / Undo / Redo.
- Auto Store/Recall all the settings.
- **ImGui** based GUI ready to integrate.

## Usage
- Just pass your ```ofParameterGroup``` parameters container.
- Enable all the parameters that you want to perform the randomization to.
- Set the minimum and maximum range sliders to fit the random inside them. 
- Notice that limits will be also inside the Min/Max of the parameters but independent of them.
- Pass your ofParameter<int> target index to receive the index randoms.

**ofApp.h**
```.cpp
#include "ofxSurfingRandomizer.h"

ofxSurfingRandomizer randomizer;

ofParameter<int> index{ "index", 0, 0, 8 };
ofEventListener listenerIndex;

ofParameterGroup params; // group container
ofParameter<float> lineWidth;
ofParameter<float> separation;
ofParameter<float> speed;
ofParameter<int> amount;
ofParameter<int> shapeType;
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

  randomizer.setTarget(index);
  randomizer.setup(params);

  // Lambda callback:to receive the randomized index
  //--------------------------------------------------------------
  listenerIndex = index.newListener([this](int &i) {
    ofLogNotice("ofApp") << "Index: " << i;
    /* presets.load(i); */
  });
}
```

<details>
  <summary>Dependencies</summary>
  <p>

Clone these add-ons and include into the **OF PROJECT GENERATOR**:
* [ofxImGui](https://github.com/Daandelange/ofxImGui/). Fork from @**Daandelange**.  
* [ofxImGuiSurfing](https://github.com/moebiussurfing/ofxImGuiSurfing/) 
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxWindowApp](https://github.com/moebiussurfing/ofxWindowApp). Only for some examples.  

*Thanks a lot to all these ofxAddons coders.*  
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
[ ANY HELP/PULL ON THIS IS APPRECIATED! ]  
* Add Undo Engine to improve exploration.
  </p>
</details>

## Author
An add-on by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

[Twitter](https://twitter.com/moebiussurfing/)  
[YouTube](https://www.youtube.com/channel/UCzUw96_wjmNxyIoFXf84hQg)  
[Instagram](https://www.instagram.com/moebiussurfing/)  
