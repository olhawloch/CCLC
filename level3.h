#ifndef _LEVEL3_H_
#define _LEVEL3_H_

#include "strategy.h"

class Level3: public Strategy {
	public:
	~Level3() override;
	Move choose_move(BoardState &bs) override;
};

#endif
