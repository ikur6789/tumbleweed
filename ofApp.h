#pragma once

#include "ofMain.h"
#include "ofNode.h"
#include "weed.h"
#include "weedRand.hpp"
#include <vector>
#include <iostream>

typedef struct {
    std::function<double(double *, unsigned int)> functionCall; // the actual evaluation function
    bool isMin;                                                // whether or not we're trying to minimize or maximize
} probFunc;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	private:
		ofEasyCam cam;
		ofVboMesh mesh;
        ofSpherePrimitive sphere;
		double wind[2] = {0.1, 0.1};

        void initializeMesh();

	//private:
		static double elvisNeedsBoats(double * coords, unsigned int dim);
		static double townsend(double * coords, unsigned int dim);
		std::vector<weed> weedPopulation;
		int populationSize = 10;

		const static unsigned int numFitnessFuncs = 2;
		probFunc fitnessFunctions[numFitnessFuncs];
		unsigned int selectedFunction = 0;
};
