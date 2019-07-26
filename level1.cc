#include "level1.h"

Move level1::choose_move(BoardState bs)
{
	// figure out whose turn it is, make an array of their pieces moves, choose from that
	// randomly
	Colour playing = bs.get_turn();
	int index = (playing == Colour::WHITE) : 0 ? 1;

	for (auto &piece : teams[index].pieces) {
		for 
	}
}
