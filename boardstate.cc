#include "boardstate.h"

std::string BoardState::print_board() const
{
	std::string board;
	int size = 64;
	// empty board
	bool white_sqr = false;
	for (int i = 0; i < size; ++i) {
		if (white_sqr) {
			board[i] = " ";
			white_sqr = false;
		} else {
			board[i] = "-";
			white_sqr = true;
		}
	}

	for (auto &team : teams) {
		std::string pieces = team.print_team();
		for (int i = 0; i < size; ++i) {
			if (pieces[i] != " ") {
				//since only one piece/sqr, no team overlaps
				board[i] = pieces[i];
			}
		}
	}

	const int width = COL_SHIFT - 2;

	char tmp;
	// flip horizontally, so black is on top
	for (int i = 0; i < width / 2; ++i) {
		for (int j = 0; i < width; ++i) {
			board[i * width + j] = tmp;
			board[i * width + j] = board[(1 - i) * width - 1 + j];
			board[(1 - i) * width - 1 + j] = tmp;
		}
	}
	// adds row numbers and newlines
	for (int i = 0; i < width; ++i) {
		board.insert(i * width, to_string(width - i - 1) + " ");
		board.insert((i * width) + width, "\n") 
	}
	return board;
}

Colour Boardstate::get_turn() const
{
	return turn;
}

Bitboard Boardstate::get_castling_rights(Colour team) const
{
//how are we holing the info in the string, Black or White, long or short or both
}

unsigned int Boardstate::get_half_turn() const
{
	return half_turn;
}

unsigned int Boardstate::get_full_turn() const
{
	return full_turn;
}

Bitboard Boardstate::get_enpassane_sqr() const
{
	return enpassant_sqr;
}
