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
