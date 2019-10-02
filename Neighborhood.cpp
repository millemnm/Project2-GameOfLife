#include "Neighborhood.h"
#include <iostream>

using namespace std;
//constructor
Neighborhood::Neighborhood(){
	xSize = 0;
	ySize = 0;

}
//Overloaded constructor
Neighborhood::Neighborhood(int x,int y){
	xSize = x;
	ySize = y;
	//Dynamically alocate storage for 2D Array
	arr = new bool*[y];
	for(int i = 0; i < y; ++i){
		arr[i] = new bool[x];
	}
	// initialize the array
	for(int k = 0; k < y; ++k){
		for(int j = 0; j < x; ++j){
	  		arr[k][j] = false;
		}
	}
}
//destructor
Neighborhood::~Neighborhood(){
	for(int i = 0; i < ySize; ++i){
		delete[] arr[i];
	}
	delete[] arr;
}

bool Neighborhood::get(int x,int y){
	return arr[y][x];
}

void Neighborhood::set(bool filled,int x,int y){
	arr[y][x] = filled;
}

void Neighborhood::print(){
	for(int y = 0; y < ySize; ++y){
		for(int x = 0; x < xSize; ++x){
			if(arr[y][x]){
				//std::cout << x << "|" << y << "|" << "true" << '\n';
				std::cout << "X";
			}else{
				//std::cout << x << "|" << y << "|" << "false" << '\n';
				std::cout << "-";
			}
		}
		cout << endl;
	}
}

void Neighborhood::clear(){
	for(int k = 0; k < ySize; ++k){
		for(int j = 0; j < xSize; ++j){
	  		arr[k][j] = false;
		}
	}
}

bool Neighborhood::equals(Neighborhood * n2){
	for(int y = 0; y < ySize; ++y){
		for(int x = 0; x < xSize; ++x){
			if(arr[y][x] != n2->get(x,y)){
				return false;
			}
		}
	}
	return true;
}
