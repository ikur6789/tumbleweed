#pragma once

#include "ofMain.h"
#include "ofNode.h"
#include "weed.h"
#include "weedRand.hpp"
#include <vector>
#include <iostream>

#include "probFunc.hpp"

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
		//OF variables
		ofEasyCam cam;
		ofVboMesh mesh;

        ofSpherePrimitive sphere;
		ofPolyline line;

		std::default_random_engine gen;
		std::uniform_real_distribution<double> domain;

		//wind variables
		double wind[2] = {0.001, 0.001};
		double WIND_MIN = -0.005;
		double WIND_MAX = 0.005;
		
		bool applyRandomSearch = true;
		double weed_step_min = -0.5;
		double weed_step_max = 0.5;
		
		bool globalApplyWind = true; // override the above apply wind fo rvariable control
		bool applyWind = true;

		unsigned int windLength = 10; // how many frames wind is applied for before break, jacob had as 20
		unsigned int windBreak = 200; // how may frames rest before wind starts again, jacob had as 100

		unsigned int fitnessCalls = 0;

        void initializeMesh();
		void initializeWindLine();

		//fitness functions
		static double elvisNeedsBoats(double * coords, unsigned int dim);
		static double townsend(double * coords, unsigned int dim);
		
		//tumbleweed variables
		std::vector<weed> weedPopulation;
		int populationSize = 30;
		unsigned int frameCount = 0;

		//stores the best
		double bestPos[2] = {0.0,0.0};
		double bestFitness = -9999.0;
		//Need now worst tumble weed
		int worstWeed = -1; //Identifier for worst tumbleweed
		double worstPos[2] = {0.0, 0.0};

		const static unsigned int numFitnessFuncs = 2;
		probFunc fitnessFunctions[numFitnessFuncs];
		unsigned int selectedFunction = 0;
};
