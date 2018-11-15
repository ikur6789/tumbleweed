#include "ofApp.h"

#define MINIMUM -8
#define MAXIMUM 8

#ifndef DIMENSION
#define DIMENSION 2
#endif

unsigned frame_cnt = 0;

// Fitness function
double ofApp::elvisNeedsBoats(double * coords, unsigned int dim)
{
	double sum1 = 0;

	double sum2 = 0;

	for(unsigned int index = 1; index <= dim; ++index)
	{
		sum1 += pow((coords[index - 1] + pow(-1.0, index) * (index % 4)), 2);
		
		sum2 += pow(coords[index - 1], index);	
	}

	double result = (-1.0) * sqrt(sum1) + sin(sum2);

	return result;
}

// other test fitness function
// this function expects dim to be 2 anyways...
// townsend function:
// https://en.wikipedia.org/wiki/Test_functions_for_optimization
double ofApp::townsend(double * coords, unsigned int dim)
{
	double x = coords[0];
	double y = coords[1];
	double cosVal = -1.0 * pow(cos((x - 0.1)*y), 2.0);
	
	double sinVal = x*sin(3*x+y);
	
	return cosVal - sinVal;
}

void ofApp::setup()
{
	
	ofEnableDepthTest();

	// size is from -8 to 8
	const int size = 16;
	// how many vertices per 1 unit
	const int perUnit = 5;
	// square root of the number of vertices
	const int checks = perUnit * size;
	//size of spheres **bacteria
	const float width = 0.5f;
	//random number generator
	domain = std::uniform_real_distribution<double>(MINIMUM, MAXIMUM);

    // initialize the fitness funcs
    fitnessFunctions[0] = { elvisNeedsBoats, false };
    fitnessFunctions[1] = { townsend, true };
    selectedFunction = 0;

    // initialize the mesh
    initializeMesh();

	// initialize our wind vain
	initializeWindLine();

	//create spheres
	sphere.setRadius(width);

	// Initialize the camera closer to our graph
	cam.setTarget(glm::vec3(0.0f,-5.0f,0.0f));
	cam.setDistance(15.0f);
	//ofSetColor(255,255,0);
	//ofSetFrameRate(25);

	for(int i = 0; i < populationSize; ++i)
	{
		weed w;
		w.initializeRandPosition();
		w.r = 100 + rand() % 155;
		w.g = 100 + rand() % 155;
		w.b = 100 + rand() % 155;
		weedPopulation.push_back(w);
	}
}

void ofApp::initializeMesh()
{
	// size is from -8 to 8
	const int size = 16;
	// how many vertices per 1 unit
	const int perUnit = 5;
	// square root of the number of vertices
	const int checks = perUnit * size;
	//size of spheres **bacteria
	const float width = 0.5f;
	//random number generator
	domain = std::uniform_real_distribution<double>(MINIMUM, MAXIMUM);

    // clear dah mesh
    mesh.clear();

	// Create Verticies
	for(int z = 0; z < checks; ++z)
	{
		// the z position of the current vertex
		double currentZ = ((double)z / (double)perUnit) - ((double)size / 2.0);

		for(int x = 0; x < checks; ++x)
		{
			// the x position of the current vertex
			double currentX = ((double)x / (double)perUnit) - ((double)size / 2.0);

			// pass in these coordinates to the fitness function to get the y position
			double coord [] = {currentX, currentZ};

			// the y position of the current vertex
			//double currentY = function(coord, 2);
			double currentY = fitnessFunctions[selectedFunction].functionCall(coord, 2);
			
			ofVec3f point(currentX, currentY, currentZ);
			mesh.addVertex(point);

			double red = 3.0f * currentY / 14.0f + 1.0f; // try and get values between 0 and 1
			std::cout << "Red: " << red << std::endl;
			mesh.addColor(ofFloatColor(red, 0.0f, 0.0f));
		}
	}

	// Create indices

	for(unsigned int y = 0; y < checks - 1; ++y)
	{
		for(unsigned int x = 0; x < checks; ++x)
		{
			unsigned int current = x + checks * y;
			unsigned int below = x + checks * (y + 1);
			unsigned int left = (x - 1) + checks * y;
			unsigned int belowRight = (x + 1) + checks * (y + 1);

			if(x == 0)
			{
				mesh.addIndex(current);
				mesh.addIndex(below);
				mesh.addIndex(belowRight);	
			}
			else if(x == checks - 1)
			{
				mesh.addIndex(current);
				mesh.addIndex(left);
				mesh.addIndex(below);
			}
			else
			{
				mesh.addIndex(current);
				mesh.addIndex(below);
				mesh.addIndex(belowRight);
				
				mesh.addIndex(current);
				mesh.addIndex(left);
				mesh.addIndex(below);
			}
		}
	}

}

void ofApp::initializeWindLine()
{
	line.addVertex(ofVec3f(4.0f, 1.0f, 4.0f));
	line.addVertex(ofVec3f(4.0f, 1.0f, 4.0f));
}

//--------------------------------------------------------------
void ofApp::update()
{
	frameCount++;

	if (globalApplyWind)
	{
		/* Set the wind to a new direction */
		if (applyWind == true && frameCount % windLength == 0) {
			for (int i = 0; i < 2; i++) {
				double newWind = WIND_MIN + (double)rand() / ((double)RAND_MAX / (WIND_MAX - WIND_MIN));
				//std::cout << "new wind: " << newWind << std::endl;
				wind[i] = newWind;
			}

			//std::cout << "wind break begin!\n";
			applyWind = false;
		}
		/* apply the wind again */
		if(applyWind == false && frameCount % windBreak == 0) {
			//std::cout << "wind break end!\n";
			applyWind = true;
		}
	}

	line.getVertices()[1].x = line.getVertices()[0].x + wind[0] * 300.0;
	line.getVertices()[1].z = line.getVertices()[0].z + wind[1] * 300.0;

	for(int i=0; i<weedPopulation.size(); ++i)
	{
		if (globalApplyWind && applyWind) weedPopulation[i].updateVelocity(wind);

		weedPopulation[i].applyDrag();

		weedPopulation[i].updatePosition();

		if (applyRandomSearch) weedPopulation[i].doRandomSearch(weed_step_min, weed_step_max, &fitnessFunctions[selectedFunction]);

		/* Keep track of current best */
		if (weedPopulation[i].fitness > bestFitness) {
			bestFitness = weedPopulation[i].fitness;
			bestPos[0] = weedPopulation[i].position[0];
			bestPos[1] = weedPopulation[i].position[1];
			std::cout << "NEW BEST Fitness: " << weedPopulation[i].fitness <<
				"    " << bestPos[0] << ", " << bestPos[1] << std::endl;
		}

	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackgroundGradient(ofColor(65,62,50), ofColor(25,22,10));	

	cam.begin();

	mesh.enableColors();
	//ofSetColor(100,100,100);
	mesh.drawWireframe();
	//mesh.disableColors();

	double x = 0;
	double y = 0;
	double z = 0;
	
	for(int i = 0; i < weedPopulation.size(); ++i)
	{
		x = weedPopulation[i].position[0];
		z = weedPopulation[i].position[1];
		double postArray[] = {x,z};
		y = fitnessFunctions[selectedFunction].functionCall(postArray,2);

		/* Update the population's fitness */
		weedPopulation[i].fitness = y;
		//std::cout << "Fitness: " << weedPopulation[i].fitness << std::endl;
		
		ofSetColor(weedPopulation[i].r, weedPopulation[i].g, weedPopulation[i].b);
		ofDrawSphere(glm::vec3(x, y, z), 0.25);
	}

	
	auto linePoint = line.getVertices()[0];
	ofSetColor(0,255,255);
	ofDrawSphere(glm::vec3(linePoint.x, linePoint.y, linePoint.z), 0.05f);
	ofSetColor(0,255,0);
	line.draw();

	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case ' ':
			globalApplyWind = !globalApplyWind;
			std::cout << "Global Apply wind: " << globalApplyWind << std::endl;
			break;
		case 'a':
			WIND_MIN += 0.0005;
			WIND_MAX -= 0.0005;
			std::cout << "Wind Min,Max: " << WIND_MIN << " " << WIND_MAX << std::endl;
			break;
		case 's':
			WIND_MIN -= 0.0005;
			WIND_MAX += 0.0005;
			std::cout << "Wind Min,Max: " << WIND_MIN << " " << WIND_MAX << std::endl;
			break;
		case 'r':
			applyRandomSearch = !applyRandomSearch;
			std::cout << "Apply Random Search: " << applyRandomSearch << std::endl;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
