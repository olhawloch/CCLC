#ifndef _LEVEL1_H_
#define _LEVEL1_H_

#include "strategy.h"

class Level1: public Strategy {
	public:
	~Level1() override;
	Move choose_move(BoardState &bs) override;
};

#endif
