#include "level1.h"

Level1::~Level1()
{
}

Move Level1::choose_move(BoardState &bs)
{
	// figure out whose turn it is, make an array of their pieces moves, choose from that
	// randomly
	Colour playing = bs.get_turn();
	int index = (playing == Colour::WHITE) ? 0 : 1;

	std::vector<Move> moves = bs.teams[index].get_moves();

	int size = moves.size();
	index = rand() % size;

	return moves[index];
}
