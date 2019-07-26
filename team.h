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
	explicit Team(Colour colour);
	// returns a bitboard of the positions of all pieces in team
	Bitboard pos_pieces() const;
	// returns true if the king is in check by enemies_atk
	bool check(Bitboard enemies_atk) const;
	// returns a vector of pieces that are checking the enemey king
	std::vector<Piece> checking_pieces(const Bitboard enemy_king_pos);
	// returns true if two or more pieces are checking the enemy_king_pos
	bool is_double_check(const Bitboard enemy_king_pos) const;
	// returns a vector of bitboards which contain lines from a piece that is
	// checking the enemy_king_pos to the checking pieces pos. The bitboards
	// don't include the enemy_king_pos but do include the checking piece's pos
	std::vector<Bitboard> get_pinning_lines(const Bitboard enemy_king_pos);
	// calculates the sudo legal moves for each of the pieces in team. Sudo
	// legal moves are those that a piece can make according to it's type, they
	// ignore leaving/putting the king in check.
	void calc_sudo_legal_moves(const Bitboard friends, const Bitboard enemies,
			bool pawn_move);
	// the sudo legal moves the piece can make skipping the first instance of
	// an enemy. Used to track pins.
	void calc_one_deep_moves(const Bitboard friends, const Bitboard enemies);
	// calculates the legal moves the pieces in team can make. Legal moves are
	// those that the piece can make according to it's type while ensuring the
	// friendly king is not left/put into check. Must be called after 
	// calc_sudo_legal_moves has been run.
	void calc_legal_moves(const Bitboard friends, const Bitboard enemies,
			std::vector<Piece> checking, Bitboard checking_line,
			const std::vector<Bitboard> &pinning, Bitboard enemies_atk,
			bool double_check);

	// getter for sudo_legal_moves, must be called after calc_sudo_legal has
	// been run.
	Bitboard get_sudo_legal_moves() const;
	// getter for legal_moves, must be called after calc_legal_moves has been
	// run.
	Bitboard get_legal_moves() const;
	// returns the legal moves as a vector of Moves
	std::vector<Move> get_moves() const;
	// returns true if the king is in checkmate according to enemies_atk
	bool checkmate(const Bitboard enemies_atk) const;
	// moves a piece in team according to Move m, returns true if successful,
	// false otherwise. Must be called after ensuring Move is valid.
	bool move_piece(Move m);
	// returns true if the Move m is valid
	bool is_valid_move(Move m);
	// adds the Piece p to the team
	void add_piece(Piece p);
	// removes the piece at pos from the team, returns true if successful, false
	// otherwise.
	bool remove_piece(Bitboard pos);
	// returns a 64 character string with the pieces represented by (prnbkq),
	// capatalized for white, other squares are represented by ' '.
	std::string print_team() const;
	// returns a bitboard of the king's position
	Bitboard get_king_pos() const;
};

#endif
