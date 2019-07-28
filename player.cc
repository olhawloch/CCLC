#include "player.h"

Player::Player(Colour c): team{c}, name{""}
{
}

Player::~Player() {
}

void Player::set_strategy(std::string strategy)
{
	if (strategy == "human") {
		strat.reset(new Strategy{});
	} else if (strategy == "computer1") {
		strat.reset(new Level1{});
	} else if (strategy == "computer2") {
		strat.reset(new Level2{});
	} else if (strategy == "computer3") {
		strat.reset(new Level3{});
	} else { // if (s == "computer4") {
		strat.reset(new Level4{});
	}
}
