#ifndef _LEVEL3_H_
#define _LEVEL3_H_

#include "strategy.h"

class Level3: public Strategy {
	public:
	~Level3() override;
	// Assigns an integer to each legal move by the following rules:
	// 	+1 for checks,
	// 	+1 for captures,
	// 	+1 for avoiding getting captured
	// Picks a move with the highest value
	Move choose_move(BoardState &bs) override;
};

#endif
