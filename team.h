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
	// caclulate once per broadstate
	bool check(Bitboard enemies_atk) const;
	// caclulate once per broadstate
	Piece *checking_piece(const Bitboard enemy_king_pos) const;
	// caclulate once per boardstate
	bool is_double_check(const Bitboard enemy_king_pos) const;
	// caclulate once per boardstate
	Bitboard pos_pieces() const;
	// assume we have up-to-date sudo moves
	std::vector<Bitboard> get_pinning_lines(const Bitboard enemy_king_pos) const;
	// caclulate once per boardstate
	void calc_sudo_legal_moves(const Bitboard friends, const Bitboard enemies,
			bool pawn_move);
	// caclulate once per boardstate
	void calc_one_deep_moves(const Bitboard friends, const Bitboard enemies);
	// caclulate once per boardstate
	void calc_legal_moves(const Bitboard friends, const Bitboard enemies,
			Piece *checking, Bitboard checking_line,
			const std::vector<Bitboard> &pinning, Bitboard enemies_atk,
			bool double_check);
	Bitboard get_sudo_legal_moves() const;
	Bitboard get_legal_moves() const;
	// caclulate once per boardstate
	bool checkmate(const Bitboard enemies_atk) const;

	bool move_piece(Move m);
	bool is_valid_move(Move m);

	void add_piece(Piece p);
	bool remove_piece(Bitboard pos);

	std::string print_team() const;

	Bitboard get_king_pos() const;
};

#endif
