#ifndef BOARD_H
#define BOARD_H

class Neighborhood{
	public:
		Neighborhood();//constructor
		Neighborhood(int x, int y);//overloaded constructor
		~Neighborhood();//destructor

		void set(bool filled, int x, int y);
		bool get(int x, int y);

		bool equals(Neighborhood * n2);

		void clear();
		void print();

	private:
		bool** arr;
		int xSize;
		int ySize;

};
#endif
