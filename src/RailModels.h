//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  RailModels.h
//  Header file for a collection of models for a railway scene
//  ========================================================================

void floor();

void tracks(const float width, const int nvert, const float* x, const float* z);

void sleepers(const int nvert, const float* x, const float* z);

void tunnel(const int start, const int length, const int nvert, const float* x, const float* z);

void createTrackMedianLine(const int nvert, const float* x, const float* z);

void engine();

void wagon();

void railwayStation();

void blockBase(float length,float width,float height);

void block1(float length,float width,float height);

void block2(float length,float width,float height);

void slope1(float length, float width, float height);

void slope2(float length, float width, float height);

void roof(float length, float width, float height);

void signalLights(int timer);

void airPlane(int timer);
