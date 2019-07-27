#include "level3.h"
#include <map>

const int CHECK_VAL = 1;
const int CAPTURE_VAL = 1;
const int AVOID_VAL = 1;

Level3::~Level3()
{
}

Move Level3::choose_move(BoardState &bs)
{
	Colour playing = bs.get_turn();
	int my_team = (playing == Colour::WHITE) ? 0 : 1;
	int enemy_team = (my_team == 0) ? 1 : 0;

	BoardState tmp_bs = bs;
	
	const int start_num_enemies = bs.teams[enemy_team].get_num_pieces();
	int end_num_enemies;

	std::vector<Move> all_moves = bs.teams[my_team].get_moves();
	std::multimap<int, Move> ordered_moves;

	int val;
	
	for (auto m : all_moves) {
		val = 0;
		tmp_bs.move(m);
		tmp_bs.toggle_turn();
		tmp_bs.calc_sudo_legal_moves();
		if (tmp_bs.check())
			val += CHECK_VAL;
		end_num_enemies = tmp_bs.teams[enemy_team].get_num_pieces();
		if (end_num_enemies < start_num_enemies)
			val += CAPTURE_VAL;
		tmp_bs.calc_legal_moves();
		if ((tmp_bs.teams[enemy_team].get_legal_moves()
				& m.to.to_bitboard()).none())
			val += AVOID_VAL;
		ordered_moves.emplace(val, m);
		tmp_bs = bs;
	}

	return ordered_moves.rbegin()->second;
}
