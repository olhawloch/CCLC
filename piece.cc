#include "piece.h"

Bitboard Piece::get_pos() const {
	return pos;
}

Bitboard Piece::get_sudo_legal_moves() const
{
	return sudo_legal_moves;
}

Bitboard Piece::get_one_deep_moves() const
{
	return one_deep_moves;
}

Bitboard Piece::get_legal_moves() const
{
	return legal_moves;
}

void Piece::set_pos(Bitboard new_pos) 
{
	pos = new_pos;
}

static Bitboard sudo_legal_pawn(Piece &p, Bitboard friends, Bitboard enemies,
		bool pawn_move)
{
	Bitboard slm{0};
	Bitboard tmp{0};
	const Bitboard empty{0};
	const Bitboard filled = friends | enemies;

	if (team == Colour::WHITE) {
		// diagonal attacks, left and right respectively
		slm |= p.pos << (COL_SHIFT - 1);
		slm |= p.pos << (COL_SHIFT + 1);

		if (pawn_move) {
			// single move
			tmp |= (p.pos << COL_SHIFT) & ~filled;
			// double move
			tmp |= (tmp && p.pos & WHITE_DOUBLE_ROW) ? 
				(p.pos << 2 * COL_SHIFT) & ~filled : empty;
			slm |= tmp;
		}	
	} else {
		slm |= p.pos >> (COL_SHIFT - 1);
		slm |= p.pos >> (COL_SHIFT + 1);
		if (pawn_move) {
			tmp |= (p.pos >> COL_SHIFT) & ~filled;
			tmp |= (tmp && p.pos & BLACK_DOUBLE_ROW) ? 
				(p.pos >> 2 * COL_SHIFT) & ~filled : empty;
			slm |= tmp;
		}
	}

	return slm & VALID_BOARD;
}

static Bitboard sudo_legal_rook(Piece &p, Bitboard friends, Bitboard enemies)
{
	Bitboard slm{0};
	Bitboard tmp{0};
	const Bitboard filled = friends | enemies;
	// vertical up
	int i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos << (COL_SHIFT * i);
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & filled)
			break;
		++i;
	}
	// vertical down
	i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos >> (COL_SHIFT * i);
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & filled)
			break;
		++i;
	}
	// horizontal right 
	i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos << i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & filled)
			break;
		++i;
	}
	// horizontal left 
	i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos >> i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & filled)
			break;
		++i;
	}

	return slm;
}

static Bitboard sudo_legal_knight(Piece &p)
{
	Bitboard slm{0};

	slm |= p.pos << COL_SHIFT + 2;
	slm |= p.pos << (2 * COL_SHIFT)  + 1;
	slm |= p.pos << COL_SHIFT - 2;
	slm |= p.pos << (2 * COL_SHIFT) - 1;
	slm |= p.pos >> COL_SHIFT + 2;
	slm |= p.pos >> (2 * COL_SHIFT) + 1;
	slm |= p.pos >> COLSHIFT - 2;
	slm |= p.pos >> (2 * COL_SHIFT) - 1;

	return slm & VALID_BOARD;
}

void Piece::calc_sudo_legal_moves(Bitboard friends, Bitboard enemies,
		bool pawn_move)
{
	switch (type) {
	case Type::PAWN:
		sudo_legal_moves = sudo_legal_pawn(*this, friends, enemies, pawn_move);
		break;
	case Type::ROOK:
		break;
	case Type::KNIGHT:
		sudo_legal_moves = sudo_legal_knight(*this);
		break;
	case Type::BISHOP:
		break;
	case Type::QUEEN:
		break;
	case Type::KING:
		break;
	default:
		// exception
	}
}
