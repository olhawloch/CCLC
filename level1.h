#ifndef _LEVEL1_H_
#define _LEVEL1_H_

#include "strategy.h"

class Level1: public Strategy {
	public:
	~Level1() override;
	// Picks move randomly from all available legal moves, pawn promotion is
	// always set to queen
	Move choose_move(BoardState &bs) override;
};

#endif
