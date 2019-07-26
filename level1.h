#include "strategy.h"

class level1 : public strategy {
	public:
		Move choose_move(BoardState bs) override;
};
