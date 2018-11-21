#include "coord.h"
#include <stdlib.h>

#define TABLESIZE 256

enum actions{
	MATCH_X,
	MATCH_Y,
	RUN_TO_BIF,
	RUN_FROM_M,
	ACTION_SIZE
};

enum direction{
	NORTH = 0x3,
	SOUTH = 0xc,
	EAST = 0x30,
	WEST = 0xc0,
};

enum visible{ 
	//to be used as (MINOTAUR & NORTH) | (BIFURCATION & SOUTH)
	MINOTAUR = 0x55,
	BIFURCATION = 0xaa,
};

class table{
private:
	char reaction[TABLESIZE];
public:
	table(){}
	table(const table&);
	table(table&&);

	void gen_random();
	table cross_over(table&);
	void debug();
	void set_entry(int entry,int new_val){reaction[entry] = new_val;}

	table operator = (const table& other);
	table operator = (table&& other);
	char operator[] (int val) const {return reaction[val];}
};