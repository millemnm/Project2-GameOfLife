#ifndef GAME_H
#define GAME_H

#include "Neighborhood.h"

class GoL{
	public:
		GoL();//constructor
		~GoL();//destructor

		void gameMode();
		void play();
		bool isRunning();

	private:

		int classic(int x, int y);
		int doughnut(int x, int y);
		int mirror(int x, int y);

		void fileFill();
		void randFill();

		void output();

		Neighborhood *curr; //current board
		Neighborhood *past; //past board

		int xSize;
		int ySize;

		int mode; //current game mode
		int spaces; //total number of spaces
		int living; //how many spaces are filled
		int genNum; //generation number

		bool running;
		bool pause;
		int now1;

		bool outFile;
};

#endif
