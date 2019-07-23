#include "bitboard.h"

const int COL_SHIFT = 10;
const int COL_HEIGHT = 12;

const Bitboard VALID_BOARD(
	std::string("000000000000000000000111111110011111111001111111100111111110011111111001111111100111111110011111111000000000000000000000"));

const Bitboard WHITE_DOUBLE_ROW(
	std::string("000000000000000000000000000000000000000000000000000000000000000000000000000000000111111110000000000000000000000000000000"));

const Bitboard BLACK_DOUBLE_ROW(
	std::string("0000000000000000000000000000000111111110000000000000000000000000000000000000000000000000000000000000000000000000000000000"));

const Bitboard WHITE_PAWN_PROMOTION(
	std::string("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111100000000000000000000000"));

const Bitboard BLACK_PAWN_PROMOTION(
	std::string("0000000000000000000000011111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"));

bool less_than(const Bitboard &a, const Bitboard &b)
{
	for (int i = (COL_SHIFT * COL_HEIGHT) - 1; i >= 0; --i) {
		if (a[i] < b[i])
			return true;
		if (a[i] > b[i])
			return false;
	}
	return false;
}

/*
 * print_bitboard returns a string that displays a Bitboard that 
 * is COL_HEIGHTxCOL_SHIFT, with numbers along the left hand side 
 * of the rows, 1s in filled squares and 0s in empty squares.
 */
std::string print_bitboard(const Bitboard &b)
{
	std::string board_string = b.to_string();
	std::string s = "";
	for (int i = 0; i < 12; ++i) {
		s += std::to_string(12 - i);
		s += " ";
		s += (12 - i < 10) ? " " : "";
		for (int j = 9; j >= 0; --j) {
			s+= board_string[i * COL_SHIFT + j];
		}
		s += "\n";	
	}
	s += "    ABCDEFGH \n";
	return s;
}
