#ifndef _LEVEL2_H_
#define _LEVEL2_H_

#include "strategy.h"

class Level2: public Strategy {
	public:
	~Level2() override;
	// Picks a move that captures or checks the enemy randomly, if no such
	// moves are available it randomly picks a legal move
	Move choose_move(BoardState &bs) override;
};

#endif
