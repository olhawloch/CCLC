#include "player.h"

Player::Player(Colour c, std::string strat) : team{c}, name{""}
{
	if (s == "human") {
		strat = std::make_unique<Strategy>();
	} else if (s == "computer1") {
		strat = std::make_unique<Level1>();
	} else if (s == "computer2") {
		strat = std::make_unique<Level2>();
	} else if (s == "computer3") {
		strat = std::make_unique<Level3>();
	} else { // if (s == "computer4") {
		strat = std::make_unique<Level4>();
	}
}
