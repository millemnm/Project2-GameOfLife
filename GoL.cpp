#include "GoL.h"
#include "Neighborhood.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <stdio.h>
#include <limits>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

std::ofstream toFile("maxmiller.out");

//constructor
GoL::GoL(){
	xSize = 0;
	ySize = 0;
	mode = 0;
	living = 0;
	spaces = 0;
	genNum = 0;
	running = true;
	pause = false;
	outFile = false;
	now1 = 0;
}
//destructor
GoL::~GoL(){
	std::cout << "Game Over!" << '\n';
	toFile.close();
}

void GoL::gameMode(){//asks user which parameters to use to create the game

	//Choose game mode
	std::cout << "Which Game Mode do you want to play?\n[1] - Classic Mode\n[2] - Donut Mode\n[3] - Mirror Mode" << endl;
	std::cin >> mode;
	while(mode != 1 && mode != 2 && mode != 3){
		std::cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		std::cout << "INVALID: That is not a recognized choice, please enter either 1, 2, or 3." << endl;
		std::cin >> mode;
	}


	//Choose if game should pause between generations
	std::cout << "Do you want the game to pause between generations?\n[1] - Yes\n[2] - No" << endl;
	int c;
	std::cin >> c;
	while(c != 1 && c != 2){
		std::cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		std::cout << "INVALID: That is not a recognized choice, please enter either 1 or 2." << endl;
		std::cin >> c;
	}
	if(c == 1){
		pause = true;
	}


	//choose to output to a file
	std::cout << "Do you want to create a file with the output?\n[1] - Yes\n[2] - No" << endl;
	std::cin >> c;
	while(c != 1 && c != 2){
		std::cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		std::cout << "INVALID: That is not a recognized choice, please enter either 1 or 2." << endl;
		std::cin >> c;
	}
	if(c == 1){
		outFile = true;
	}

	//choose how to create world
	std::cout << "How would you like to create the game board?\n[1] - File\n[2] - Randomly" << endl;
	std::cin >> c;
	while(c != 1 && c != 2){
		cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		std::cout << "INVALID: That is not a recognized choice, please either enter 1 or 2." << endl;
		std::cin >> c;
	}
	if(c == 1){
		fileFill();
	}else{
		randFill();
	}
	
	std::cin.clear();
	std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//World Creation:
void GoL::fileFill(){//[1] world is created by a file
	std::ifstream file;
	string fileName;
	std::cout << "What name of the map file? " << flush;
	while(true){
		file.close();
    	file.clear();
		getline(cin, fileName); //get the file name from terminal
		file.open(fileName.c_str()); //open the file
		if(file){
			break;
		}
		std::cout << "Invalid file name, please enter a valid name: " << flush;
	}

	if (file.is_open()) {
	    std::string line;
		int row = 0;
	    while (getline(file, line)) {
			if(ySize == 0){
				ySize = stoi(line);
			}else if(xSize == 0){
				xSize = stoi(line);
				//Set up current game board
				spaces = xSize * ySize;
				curr = new Neighborhood(xSize,ySize);
			}else{
				for(int col = 0; col < xSize; ++col){
					if(char(line.at(col)) == 'X'){
						curr->set(true, col, row);
						++living;
					}
				}
				++row;
			}
	    }
	    file.close();
		std::cout << "Generation 0" << endl;
		curr->print();
		if(outFile){
			output();
		}
	}
}
void GoL::randFill(){//[2] world is created randomly
	//Get rows
	std::cout << "How many rows does the game board have?" << endl;
	std::cin >> ySize;
	while(cin.fail()){
		std::cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		std::cout << "INVALID: Please enter a valid number." << endl;
		std::cin >> ySize;
	}

	//Get collumns
	std::cout << "How many collumns does the game board have?" << endl;
	std::cin >> xSize;
	while(cin.fail()){
		std::cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		std::cout << "INVALID: Please enter a valid number." << endl;
		std::cin >> xSize;
	}

	//Get Population Density
	std::cout << "What is the population density of the neighborhood? (between 0 and 1)" << endl;
	double density;
	std::cin >> density;
	while(!isgreater(density, 0) && !islessequal(density, 1)){
		std::cin.clear();
		std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
		std::cout << density << "IS INVALID: Please enter a valid decimal from 0 to 1." << endl;
		std::cin >> density;
	}

	//Set up current game board
	spaces = xSize * ySize;
	living = spaces * density;
	//std::cout << "Living: " << living << '\n';
	curr = new Neighborhood(xSize,ySize);

	//fill current neighborhood randomly with the inputed Density
	int xRand = rand() % xSize;
	int yRand = rand() % ySize;
	curr->clear();
	for(int i = 0; i < living; ++i){//make each living node
		while(curr->get(xRand,yRand)){//check to see if that location was already taken (if it is look for a new one)
			xRand = rand() % xSize;
			yRand = rand() % ySize;
		}
		//std::cout << "set " << xRand << "|" << yRand << '\n';
		curr->set(true, xRand, yRand);
	}
	std::cout << "Generation " << genNum << endl;
	curr->print();
	if(outFile){
		output();
	}
}

void GoL::play(){//Makes sure the correct game mode is played each round
	if(pause){
		std::cout << "Press Enter to Continue..." << endl;
		std::string useless;
		getline(cin, useless);
		useless.clear();
	}else{
		std::chrono::milliseconds timespan(1500); // or whatever

		std::this_thread::sleep_for(timespan);
	}

	int neighbors;
	past = curr;
	curr = new Neighborhood(xSize,ySize);

	living = 0;
	++genNum;
	curr->clear();

	for(int y = 0; y < ySize; ++y){
		for(int x = 0; x < xSize; ++x){
			neighbors = 0;
			if(mode == 1){
				neighbors = classic(x,y);
			}else if(mode == 2){
				neighbors = donut(x,y);
			}else{
				neighbors = mirror(x,y);
			}

			if(neighbors == 2 && past->get(x,y)){
				curr->set(true, x, y);
				++living;
			}else if(neighbors == 3){
				curr->set(true, x, y);
				++living;
			}
		}
	}

	std::cout << "Generation " << genNum << endl;
	curr->print();
	if(outFile){
		output();
	}

	if(living == 0){
		running = false;
		std::cout << "There is no one left in the Neighborhood." << '\n';
	}else if(curr->equals(past)){
		running = false;
		std::cout << "The Neighborhood has stabilized." << '\n';
	}
}

//Game Modes:
int GoL::classic(int x,int y){//[1] walls are empty
	int n = 0;
	if(x != 0){
		if(past->get(x-1,y)){ //check to the left
			++n;
		}
		if(y != 0){
			if(past->get(x-1,y-1)){ //check up to the left
				++n;
			}
		}
		if(y != ySize-1){
			if(past->get(x-1,y+1)){ //check down to the left
				++n;
			}
		}
	}

	if(x != xSize-1){
		if(past->get(x+1,y)){ //check to the right
			++n;
		}
		if(y != 0){
			if(past->get(x+1,y-1)){ //check up to the right
				++n;
			}
		}
		if(y != ySize-1){
			if(past->get(x+1,y+1)){ //check down to the right
				++n;
			}
		}
	}

	if(y != 0){
		if(past->get(x,y-1)){ //check up
			++n;
		}
	}
	if(y != ySize-1){
		if(past->get(x,y+1)){ //check down
			++n;
		}
	}
	return n;
}
int GoL::donut(int x,int y){//[2] walls are connected in a loop
	int n = 0;
	if(x != 0){
		if(past->get(x-1,y)){ //check to the left
			++n;
		}
		if(y != 0){
			if(past->get(x-1,y-1)){ //check up to the left
				++n;
			}
		}
		if(y != ySize-1){
			if(past->get(x-1,y+1)){ //check down to the left
				++n;
			}
		}
	}else{//donut
		if(past->get(xSize-1,y)){
			++n;
		}
		if(y==0){//corners
			if(past->get(xSize-1,ySize-1)){
				++n;
			}
		}else if(y==ySize-1){
			if(past->get(xSize-1,0)){
				++n;
			}
		}
	}

	if(x != xSize-1){
		if(past->get(x+1,y)){ //check to the right
			++n;
		}
		if(y != 0){
			if(past->get(x+1,y-1)){ //check up to the right
				++n;
			}
		}
		if(y != ySize-1){
			if(past->get(x+1,y+1)){ //check down to the right
				++n;
			}
		}
	}else{//donut
		if(past->get(x,y)){//x=xSize
			++n;
			if(y==0 || y==ySize-1){//corners
				++n;
			}
		}
		if(past->get(0,y)){//x=0
			++n;
		}
		if(y==0){//corners
			if(past->get(0,ySize-1)){
				++n;
			}
		}else if(y==ySize-1){
			if(past->get(0,0)){
				++n;
			}
		}
	}

	if(y != 0){
		if(past->get(x,y-1)){ //check up
			++n;
		}
	}else{//donut
		if(past->get(x,ySize-1)){
			++n;
		}
	}
	if(y != ySize-1){
		if(past->get(x,y+1)){ //check down
			++n;
		}
	}else{//donut
		if(past->get(x,0)){
			++n;
		}
	}

	return n;
}
int GoL::mirror(int x,int y){//[3] walls are mirrors
	int n = 0;
	if(x != 0){
		if(past->get(x-1,y)){ //check to the left
			++n;
		}
		if(y != 0){
			if(past->get(x-1,y-1)){ //check up to the left
				++n;
			}
		}
		if(y != ySize-1){
			if(past->get(x-1,y+1)){ //check down to the left
				++n;
			}
		}
	}else{//mirror
		if(past->get(x,y)){//x=0
			++n;
			if(y==0 || y==ySize-1){//corners
				++n;
			}
		}
	}

	if(x != xSize-1){
		if(past->get(x+1,y)){ //check to the right
			++n;
		}
		if(y != 0){
			if(past->get(x+1,y-1)){ //check up to the right
				++n;
			}
		}
		if(y != ySize-1){
			if(past->get(x+1,y+1)){ //check down to the right
				++n;
			}
		}
	}else{//mirror
		if(past->get(x,y)){//x=xSize
			++n;
			if(y==0 || y==ySize-1){//corners
				++n;
			}
		}
	}

	if(y != 0){
		if(past->get(x,y-1)){ //check up
			++n;
		}
	}else{//mirror
		if(past->get(x,y)){//y=0
			++n;
		}
	}
	if(y != ySize-1){
		if(past->get(x,y+1)){ //check down
			++n;
		}
	}else{//mirror
		if(past->get(x,y)){//y=ySize
			++n;
		}
	}

	return n;
}

bool GoL::isRunning(){
	return running;
}

void GoL::output(){
	toFile << "Generation " << genNum << '\n';
	for(int y = 0; y < ySize; ++y){
		for(int x = 0; x < xSize; ++x){
			if(curr->get(x,y)){
				toFile << "X";
			}else{
				toFile << "-";
			}
		}
		toFile << endl;
	}
}
