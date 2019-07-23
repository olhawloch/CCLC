#include "boardstate.h"

std::string BoardState::print_board() const
{
	std::string board;
	int size = 64;
	// empty board
	bool white_sqr = true;
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
		for (auto piece : pieces)
	}
	
}
