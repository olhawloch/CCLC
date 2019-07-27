#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include <iostream>
#include <vector>
#include "move.h"
#include "boardstate.h"
#include "commandinterpreter.h"

class Strategy {
	public:
	virtual ~Strategy();
	// the default implemenation is for a human player
	virtual Move choose_move(BoardState &bs);
};

#endif
