#include "ofMain.h"
#include "ofApp.h"

int main(){

#if defined(TARGET_OPENGLES)
    ofGLESWindowSettings settings;
    settings.setSize(1280, 720);
    settings.setGLESVersion(4,3);
    ofCreateWindow(settings);
#else
    ofSetupOpenGL(1920, 1060, OF_WINDOW);
#endif

    ofRunApp( new ofApp());

}
