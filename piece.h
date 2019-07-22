#ifndef _PIECE_H_
#define _PIECE_H_

#include "bitboard.h"
#include "type.h"

class Piece {
	// a single 1 at the position where the piece is
	Bitboard pos;
	// the moves the piece can make according to it's type,
	// includes moving onto a friendly piece
	Bitboard sudo_legal_moves;
	// the moves the piece can make according to it's type,
	// skipping over the first enemy it encounters
	Bitboard one_deep_moves;
	// the moves the piece can make according to it's type,
	// and not putting the king in check
	Bitboard legal_moves;
	Type type;
	Colour team;
	
	public:
	Bitboard get_pos() const;
	Type get_type() const;
	Colour get_team() const;
	Bitboard get_sudo_legal_moves() const;
	Bitboard get_one_deep_moves() const;
	Bitboard get_legal_moves() const;
	void set_pos(Bitboard new_pos);
	void set_type(Type t);
	void calc_sudo_legal_moves(const Bitboard friends,
			const Bitboard enemies, const bool pawn_move);
	void calc_one_deep_moves(const Bitboard friends,
			const Bitboard enemies);
	void calc_legal_moves(Bitboard friends, Bitboard enemies, Piece *checking,
			Bitboard checking_line,
			const std::vector<Bitboard> &pinning,
			Bitboard enemies_atk, bool double_check);
	Bitboard line_to_king(const Bitboard king_pos);
};

#endif
