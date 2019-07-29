#include "level4.h"
#include <map>

const int SEARCH_DEPTH = 1;

Level4::~Level4()
{
}

static float average_num_moves(Move move, BoardState bs, int depth)
{
	Colour playing = bs.get_turn();
	const int my_team = (playing == Colour::WHITE) ? 0 : 1;
	const int enemy_team = (my_team == 0) ? 1 : 0;

	bs.move(move);
	bs.toggle_turn();
	bs.calc_legal_moves();

	if (depth <= 0)
		return bs.teams[enemy_team].get_moves().size();

	--depth;
	auto enemy_moves = bs.teams[enemy_team].get_moves(); 
	float total_average = 0;

	BoardState tmp_bs;

	for (auto enemy_m : enemy_moves) {
		tmp_bs = bs;
		tmp_bs.move(enemy_m);
		tmp_bs.toggle_turn();
		tmp_bs.calc_legal_moves();
		auto my_moves = tmp_bs.teams[my_team].get_moves();
		float average = 0;
		for (auto my_m : my_moves) {
			average += average_num_moves(my_m, tmp_bs, depth);
		}
		average /= my_moves.size();
		total_average += average;
	}
	return total_average / enemy_moves.size();
}

Move Level4::choose_move(BoardState &bs)
{
	Colour playing = bs.get_turn();
	const int my_team = (playing == Colour::WHITE) ? 0 : 1;

	BoardState tmp_bs = bs;

	std::vector<Move> all_moves = bs.teams[my_team].get_moves();
	std::multimap<float, Move> ordered_moves;

	for (auto m : all_moves) {
		float val = average_num_moves(m, bs, SEARCH_DEPTH);
		ordered_moves.emplace(val, m);
	}
	return ordered_moves.begin()->second;
}

