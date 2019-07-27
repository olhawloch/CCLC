#ifndef _LEVEL4_H_
#define _LEVEL4_H_

#include "strategy.h"

class Level4: public Strategy {
	public:
	~Level4() override;
	Move choose_move(BoardState &bs) override;
};

#endif
