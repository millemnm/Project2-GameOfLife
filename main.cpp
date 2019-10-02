#include "GoL.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
	GoL game;

	//Set up the Game:
	game.gameMode();

	//Play the Game
	while(game.isRunning()){
		game.play();
	}

	return 0;
};
