# ofxSurfingRandomizer

## Overview
An **openFrameworks** add-on to perform ranged randomization between the settable minimum and maximum values of each selected ```ofParameters``` from a group.

## Screenshot 
<!-- <img src="docs/readme_images/ofxSurfingRandomizer.gif" width="80%" height="80%"> -->
<img src="readme_images/Capture.PNG">

## Features
- Only ```float```, ```int``` and ```bool``` types yet.
- Workflow Helpers:
  * Select NONE/ALL parameters.
  * Reset all parameters to minimum.
  * Reset Min/Max ranges to parameter limits.
  * Store/Recall a memory state to improve exploring.
- Auto Store/Recall all the settings.
- **ImGui** based GUI ready to integrate.

## Usage
- Just pass your ```ofParameterGroup``` parameters container.
- Enable all the parameters that you want to perform the randomization to.
- Set the minimum and maximum range sliders to fit the random inside them. 
- Notice that limits will be also inside the Min/Max of the parameters.

**ofApp.h**
```.cpp
#include "ofxSurfingRandomizer.h"

ofxSurfingRandomizer data;

ofParameterGroup params; // main container
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

  data.setup(params);
}
```

<details>
  <summary>Dependencies</summary>
  <p>

Clone these add-ons and include into the **OF PROJECT GENERATOR**:
* [ofxImGui](https://github.com/Daandelange/ofxImGui/tree/ofParameters-Helpers-Test)  [ FORK | BRANCH ]  
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxWindowApp](https://github.com/moebiussurfing/ofxWindowApp)  [ Only for **example-Basic** ]  

*Thanks a lot to all these ofxAddons coders.*  
  </p>
</details>

<details>
  <summary>Tested Systems</summary>
  <p>

  - **Windows 10** / **VS 2017** / **OF ~0.11**
  </p>
</details>

## TODO
* Add more types: 2D/3D vectors and colors. Using templates [?] ...  
[ ANY HELP/PULL ON THIS IS APPRECIATED! ]  
* Add Undo engine to improve exploration.

## Author
An add-on by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

[Twitter](https://twitter.com/moebiussurfing/)  
[YouTube](https://www.youtube.com/channel/UCzUw96_wjmNxyIoFXf84hQg)  
[Instagram](https://www.instagram.com/moebiussurfing/)  
