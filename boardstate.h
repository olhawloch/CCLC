#ifndef _BOARDSTATE_H_
#define _BOARDSTATE_H_

#include "piece.h"
#include "colour.h"
#include <string.h>
#include <vector>

class BoardState {
	Colour turn;
	std::String castling_rights;
	unsigned int half_turn;
	unsigned int full_turn;
	Bitboard enpassant_sqr;
	std::vector<Team> teams;

	public:
	std::string print_board() const;
	Colour get_turn() const;
	Bitboard get_castling_rights(Colour team) const;
	unsigned int get_half_turn() const;
	unsigned int get_full_turn() const;
	Bitboard get_enpassant_sqr() const;
};

#endif
