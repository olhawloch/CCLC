#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "colour.h"
#include "strategy.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include <string>

class Player {
	public:
		Colour team;
		std::unique_ptr<Strategy> strat;
		string name; // will keep this empty as of right now

		Player(Colour c, std::string strat);
};

#endif
