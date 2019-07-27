#include "player.h"

Player::Player(Colour c): team{c}, name{""}, strat{nullptr}
{
}

Player::~Player() {
	delete strat;
}

void Player::set_strategy(std::string strategy)
{
	if (strategy == "human") {
		strat = new Strategy{};
	} else if (strategy == "computer1") {
		strat = new Level1{};
	} else if (strategy == "computer2") {
		strat = new Level2{};
	} else if (strategy == "computer3") {
		//strat = std::make_unique<Strategy>(new Level3());
	} else { // if (s == "computer4") {
		//strat = std::make_unique<Strategy>(new Level4());
	}
}
