#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include <vector>
#include "move.h"

class Strategy {
	public:
		virtual Move choose_move(BoardState bs);
};

#endif
