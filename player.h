#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "colour.h"
#include "strategy.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include <string>
#include <memory>

class Player {
	Colour team;
	std::string name; // will keep this empty as of right now

	public:
	float score = 0;
	Strategy *strat;

	explicit Player(Colour c);
	~Player();
	void set_strategy(std::string strat);
};

#endif
