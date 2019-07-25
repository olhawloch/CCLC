#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "colour.h"
#include <string>

struct Player {
	Colour team;
	Strategy * strat;
	string name;
};

#endif
