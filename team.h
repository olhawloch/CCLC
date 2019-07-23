#ifndef _TEAM_H_
#define _TEAM_H_

#include "piece.h"
#include "colour.h"
#include "move.h"
#include <vector>

class Team {
	std::vector<Piece> pieces;
	Colour colour;
	
	public:
	bool check(Bitboard enemies_atk) const;
	Piece *checking_piece(const Bitboard enemy_king_pos) const;
	bool is_double_check(const Bitboard enemy_king_pos) const;
	Bitboard pos_pieces() const;
	// assume we have up-to-date sudo moves
	std::vector<Bitboard> get_pinning_lines(const Bitboard enemy_king_pos) const;
	void calc_sudo_legal_moves(const Bitboard friends, const Bitboard enemies,
			bool pawn_move);
	void calc_one_deep_moves(const Bitboard friends, const Bitboard enemies);
	void calc_legal_moves(const Bitboard friends, const Bitboard enemies,
			Piece *checking, Bitboard checking_line,
			const std::vector<Bitboard> &pinning, Bitboard enemies_atk);
	Bitboard get_sudo_legal_moves() const;
	Bitboard get_legal_moves() const;
	bool checkmate(const Bitboard enemies_atk) const;
	bool move_piece(Move m);
	bool is_valid_move(Move m);
	void add_piece(Piece p);
	bool remove_piece(Bitboard pos);
	std::string print_team() const;
};

#endif
