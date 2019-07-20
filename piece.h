#ifndef _PIECE_H_
#define _PIECE_H_

#include "bitboard.h"
#include "type.h"

class Piece {
	Bitboard pos; // a single 1 at the position where the piece is
	Bitboard sudo_legal_moves; // the moves the piece can make according to it's type, includes moving onto a friendly piece
	Bitboard one_deep_moves; // the moves the piece can make according to it's type, skipping over the first enemy it encounters
	Bitboard legal_moves; // the moves the piece can make according to it's type, and not putting the king in check
	Type type;
	Colour team;
	
	public:
	Bitboard get_pos() const;
	Bitboard get_sudo_legal_moves(Bitboard enemies, bool pawn_move) const;
	Bitboard get_one_deep_moves() const;
	Bitboard get_legal_moves() const;
	void set_pos(Bitboard new_pos);
	void calc_sudo_legal_moves();
	void calc_one_deep_moves();
	void calc_legal_moves();
};

#endif
