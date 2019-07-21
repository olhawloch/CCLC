#include "bitboard.h"
#include <string>

/*
print_bitboard returns a string that displays a Bitboard that is COL_HEIGHTxCOL_SHIFT, 
with numbers along the left hand side of the rows, 
1s in filled squares and 0s in empty squares.
*/

string print_bitboard(Bitboard b)
{
	std::string board_string = b.to_string();
	std::string s = "";
	for (int i = COL_HEIGHT - 1; i >= 0; --i) {
		s += std::to_string(i + 1);
		s += " ";
		for (int j = 0; j < COL_SHIFT; ++j) {
			s+= board_string(i * COL_SHIFT + j);
		}
		s += "\n";
	}
	return s;
}
