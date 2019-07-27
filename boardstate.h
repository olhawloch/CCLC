#ifndef _BOARDSTATE_H_
#define _BOARDSTATE_H_

#include "team.h"
#include "colour.h"
#include <string.h>
#include <vector>

class BoardState {
	Colour turn;
	std::string castling_rights;
	unsigned int half_turn;
	unsigned int full_turn;
	Bitboard enpassant_sqr;

public:
	std::vector<Team> teams;

	BoardState();

	std::string print_board() const;
	Colour get_turn() const;
	void set_turn(Colour c);
	void toggle_turn();
	Bitboard get_castling_rights(Colour team) const;
	unsigned int get_half_turn() const;
	unsigned int get_full_turn() const;
	Bitboard get_enpassant_sqr() const;
	void calc_sudo_legal_moves();
	void calc_legal_moves();
	bool move(Move m);
	bool check() const;
	bool checkmate() const;
	// call only after ensuring it's not checkmate
	bool stalemate() const;
};

#endif
