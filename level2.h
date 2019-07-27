#ifndef _LEVEL2_H_
#define _LEVEL2_H_

#include "strategy.h"

class Level2: public Strategy {
	public:
	~Level2() override;
	Move choose_move(BoardState &bs) override;
};

#endif
