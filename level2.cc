#include "level2.h"

Level2::~Level2()
{
}

Move Level2::choose_move(BoardState &bs)
{
	Colour playing = bs.get_turn();
	int att = (playing == Colour::WHITE) ? 0 : 1;
	int def = (att == 0) ? 1 : 0;

	BoardState tmp_bs = bs;

	std::vector<Move> all_moves = bs.teams[att].get_moves();

	Bitboard def_pos = bs.teams[def].pos_pieces();

	std::vector<Move> captures = bs.teams[att].get_captures(def_pos);
	
	for (auto m : all_moves) {
		tmp_bs.move(m);
		tmp_bs.toggle_turn();
		tmp_bs.calc_sudo_legal_moves();
		if (tmp_bs.check()) {
			captures.emplace_back(m);
		}
		tmp_bs = bs;
	}

	if (captures.size() == 0) {
		// random move
		std::vector<Move> moves = bs.teams[att].get_moves();
		int size = moves.size();
		int index = rand() % size;
		return moves[index];
	}
	
	int size = captures.size();
	int index = rand() % size;

	return captures[index];
}
