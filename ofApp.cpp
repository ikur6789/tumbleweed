#include "ofApp.h"

#define MINIMUM -8
#define MAXIMUM 8

#ifndef DIMENSION
#define DIMENSION 2
#endif

//smarter wind Ian



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

double ofApp::birdFunction(double * coords, unsigned int dim)
{
	/* Assuming input is coords[0] = x, coords[1] = y */
	double x = coords[0];
	double y = coords[1];

	double sinVar = sin(y)*exp(pow(1-cos(x), 2));
	double cosVar = cos(x)*exp(pow(1-sin(y), 2));

	return ((sinVar + cosVar + pow(x-y, 2)) * 0.07 + 8.0f) * -1.0f; // scale that shiz down
}

void ofApp::setup()
{
	printf("Algorithm?: ");
	cin >> wind_algorithm;
	/*switch(wind_algorithm)
	{
		case 1:
		case 'A':
		case 'a':
		wind_algorithm='A';
		break;

		case 2:
		case 'B':
		case 'b':
		wind_algorithm='B';
		break;

		case 3:
		case 'C':
		case 'c':
		wind_algorithm='C';
		break;

		case 4:
		case 'D':
		case 'd':
		wind_algorithm='D';
		break;

		case 5:
		case 'E':
		case 'e':
		wind_algorithm='E';
		break;
		
		default:
		wind_algorithm=0;
		break;
	}*/
	if(wind_algorithm>5)
		wind_algorithm=5;
	
	printf("picked:%d\n",wind_algorithm);

	ofEnableDepthTest();

	// size is from -8 to 8
	const int size = 16;
	// how many vertices per 1 unit
	const int perUnit = 5;
	// square root of the number of vertices
	const int checks = perUnit * size;
	//size of spheres **bacteria
	const float width = 0.5f;

    // initialize the fitness funcs
    fitnessFunctions[0] = { elvisNeedsBoats, false };
    fitnessFunctions[1] = { townsend, true };
	fitnessFunctions[2] = { birdFunction, false };
    selectedFunction = 2;

	// initialize the mesh
	initializeMesh();

	// initialize our wind vain
	initializeWindLine();

	//create spheres
	sphere.setRadius(width);

	// Initialize the camera closer to our graph
	cam.setTarget(glm::vec3(0.0f,-5.0f,0.0f));
	cam.setDistance(15.0f);
	cam.removeAllInteractions();
	
	if(wind_algorithm==3)
		cam.setDistance(15.0f);
	else
		cam.setDistance(20.0f);
	//ofSetColor(255,255,0);
	//ofSetFrameRate(25);

	double initVel[] = {0.001, 0.001};
	for(int i = 0; i < populationSize; ++i)
	{
		weed w;
		w.initializeRandPosition();
		w.setVelocity(initVel); //Small initial velocity
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
			//std::cout << "Red: " << red << std::endl;
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

	std::sort(	weedPopulation.begin(), weedPopulation.end(), 
					[](weed a, weed b){return a.fitness > b.fitness;}
					);

	double newWind;
	if (globalApplyWind)
	{
		switch(wind_algorithm)
		{	
			case 'A': //BJ's Algorithm
			{
				if (applyWind == true && frameCount % windLength == 0) 
				{
					for (int i = 0; i < 2; i++) 
					{
						if(windCycle==0) //First time evaluation to randomized tumbleweed locations
							{newWind = WIND_MIN + (double)rand() / ((double)RAND_MAX / (WIND_MAX - WIND_MIN));}

							wind[i] = newWind;

							applyWind = false;
					}
					/* apply the wind again */
					
				}
				if(applyWind == false && frameCount % windBreak == 0) {
						//std::cout << "wind break end!\n";
						applyWind = true;
					}
			}
			break;

			case 'B': //Ian's Algorithm
			{
				if (applyWind == true && frameCount % windLength == 0) {
					for (int i = 0; i < 2; i++) {
						if(windCycle==0) //First time evaluation to randomized tumbleweed locations
							{newWind = WIND_MIN + (double)rand() / ((double)RAND_MAX / (WIND_MAX - WIND_MIN));}
						else
						{
							//Want to go reverse of worst tumble weed
							newWind = (weedPopulation[worstWeed].position[i]*(-1))/1000.0;
						}
						//std::cout << "new wind: " << newWind << std::endl;
						wind[i] = newWind;
					}

					applyWind = false;
				}
				/* apply the wind again */
				if(applyWind == false && frameCount % windBreak == 0) {
					//std::cout << "wind break end!\n";
					applyWind = true;
				}
			}
			break;
			case 'C': //Jacob's Algorithm
			{

			}
			break;
			case 'D': //Kevin's Algorithm
			{
				// KEVIN's ALGORITHM
					wind[0] = (weedPopulation[0].position[0] - weedPopulation[weedPopulation.size()-1].position[0])/1000;
					wind[1] = (weedPopulation[0].position[1] - weedPopulation[weedPopulation.size()-1].position[1])/1000;
			}
			break;
			case 'E': //Global Center Algorithm
			{
		
			}
			case 'F': //Mouse controlled algorithm/program
			{
				wind[0] = mouseX;
				wind[1] = mouseY;								
			}
			break;
			// by default, go to mix algorithm
			case 'G':
			default:
			{
				/* Set the wind to a new direction */
				if (applyWind == true && frameCount % windLength == 0) {
					for (int i = 0; i < 2; i++) {
						if(windCycle==0) //First time evaluation to randomized tumbleweed locations
							{newWind = WIND_MIN + (double)rand() / ((double)RAND_MAX / (WIND_MAX - WIND_MIN));}
						else
						{
							//Want to go reverse of worst tumble weed
							newWind = (weedPopulation[worstWeed].position[i]*(-1))/1000.0;
						}
						//std::cout << "new wind: " << newWind << std::endl;
						wind[i] = newWind;
					}

					// KEVIN's ALGORITHM
					//wind[0] = (weedPopulation[0].position[0] - weedPopulation[weedPopulation.size()-1].position[0])/1000;
					//wind[1] = (weedPopulation[0].position[1] - weedPopulation[weedPopulation.size()-1].position[1])/1000;

					//std::cout << "wind break begin!\n";
					applyWind = false;
				}
				/* apply the wind again */
				if(applyWind == false && frameCount % windBreak == 0) {
					//std::cout << "wind break end!\n";
					applyWind = true;
				}
			}
			break;
		}

		if (applyWind == false) {
			line.getVertices()[1].x = line.getVertices()[0].x + wind[0] * 0.0;
			line.getVertices()[1].z = line.getVertices()[0].z + wind[1] * 0.0;
		}
		else {
			line.getVertices()[1].x = line.getVertices()[0].x + wind[0] * 500.0;
			line.getVertices()[1].z = line.getVertices()[0].z + wind[1] * 500.0;
		}
	}


	double cycleWorstFitness = 9999.0;  // Ian's
	double worstFitness = 9999.0;       // jacob
	double a_bestFitness = -9999.0;     // jacob
	for(weed & w : weedPopulation)
	{
		// BJ!
		//if (globalApplyWind && applyWind) weedPopulation[i].updateVelocity(wind); // might need to delete!

		if(w.fitness < worstFitness)
		{
			worstFitness = w.fitness;	
		}

		if(w.fitness > a_bestFitness)
		{
			a_bestFitness = w.fitness;
		} 

        if(usingVelocity==false)
        {
	        // BJ added to immediately set velocity to 0 if no wind
	        double zeroVelocity[] = {0.0,0.0};
	        if (!applyWind || !globalApplyWind) {
	            w.setVelocity(zeroVelocity);
	        }
	    }
	}

	for(int i=0; i<weedPopulation.size(); ++i)
	{
		// BJ!
		if (applyRandomSearch) weedPopulation[i].doRandomSearch(weed_step_min, weed_step_max, &fitnessFunctions[selectedFunction]);

		/* Keep track of current best */
		if (weedPopulation[i].fitness > bestFitness) 
		{
			bestFitness = weedPopulation[i].fitness;
			bestPos[0] = weedPopulation[i].position[0];
			bestPos[1] = weedPopulation[i].position[1];

			std::cout << "Call Count : " << fitnessCalls << std::endl;
			std::cout << "Fitness : " << weedPopulation[i].fitness << std::endl;
			std::cout << "Location : "
				<< bestPos[0] << ", " << bestPos[1] << std::endl;
			std::cout << std::endl;
		}

		// Jacob!
		if (applyWind) 
		{
			double tmpwind[2] = {wind[0], wind[1]};
			double diff = worstFitness * -1.0;	
			double tmpWorstFitness = 0.0;
			double tmpBestFitness = a_bestFitness + diff;
			double tmpFitness = weedPopulation[i].fitness + diff;

			double percent;	
			if(tmpBestFitness != 0.0)
			{
				percent = tmpFitness / tmpBestFitness; 
			}
			else 
			{
				percent = 1.0;
			}

            // bj testing with changing vals
			percent = (-0.9999) * percent + 1.0; 
            //std::cout << "percent: " << percent << std::endl;

			tmpwind[0] *= percent; 
			tmpwind[1] *= percent;
			//if(usingVelocity) 
				//weedPopulation[i].updateVelocity(tmpwind);
			//else
				weedPopulation[i].setVelocity(tmpwind); // bj changed to directly set velocity (with no accel)
		}

		weedPopulation[i].applyDrag();

		weedPopulation[i].updatePosition();

		/*Ian: We will now also keep track of the worst tumbleweed's fitness*/
		//Evaluate the fitness of all of the tumbleweeds
		if(weedPopulation[i].fitness < cycleWorstFitness) {
			cycleWorstFitness = weedPopulation[i].fitness;
			worstWeed = i;
			worstPos[0] = weedPopulation[i].position[0];
			worstPos[1] = weedPopulation[i].position[1];
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
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
		fitnessCalls += 1;
		/* Update the population's fitness */
		weedPopulation[i].fitness = y;
		
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
			printf("Apply Random Search: ");
			printf(applyRandomSearch ? "true\n" : "false\n");
			break;
		case 'v':
			usingVelocity = !usingVelocity; //Yes, smart
			printf("Velocity: ");
			printf(usingVelocity ? "true\n" : "false\n");
			break;
		case '1':
			wind_algorithm = 'A';
			printf("Wind now A\n");
			weed::DRAG = 0.1;
			windBreak=100;
			 WIND_MIN = -0.050;
		 WIND_MAX = 0.050;
			break;
		case '2':
			wind_algorithm = 'B';
			printf("Wind now B\n");
			weed::DRAG = 0.1;
			windBreak=100;
			 WIND_MIN = -0.050;
		 WIND_MAX = 0.050;
			break;
		case '3':
			wind_algorithm = 'C';
			printf("Wind now C\n");
			weed::DRAG = 0.001;
			windBreak=100;
			 WIND_MIN = -0.200;
		 WIND_MAX = 0.200;
			break;
		case '4':
			wind_algorithm = 'D';
			printf("Wind now D\n");
			weed::DRAG = 0.01; //Changed for Kevin's algo
			windBreak=100;
			windLength=20;
			WIND_MIN= -0.01;
			WIND_MAX=  0.01;
			break;
		case '5':
			wind_algorithm = 'E';
			printf("Wind now E\n");
			weed::DRAG = 0.001;
			windBreak=100;
			 WIND_MIN = -0.200;
		 WIND_MAX = 0.200;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
	mouseX = x;
	mouseY = y;	
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
