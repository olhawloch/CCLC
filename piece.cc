#include "piece.h"
#include <cassert>

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

static Bitboard sudo_legal_pawn(const Piece &p, const Bitboard &friends,
		const Bitboard &enemies, const bool pawn_move)
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

static Bitboard sudo_legal_rook(const Piece &p, const Bitboard &friends,
		const Bitboard &enemies, const unsigned int depth)
{
	Bitboard slm{0};
	Bitboard tmp{0};
	// vertical up
	unsigned int count = 0;
	int i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos << (COL_SHIFT * i);
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}
	// vertical down
	count = 0;
	i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos >> (COL_SHIFT * i);
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}
	// horizontal right 
	count = 0;
	i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos << i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}
	// horizontal left 
	count = 0;
	i = 1;
	while(1) {
		tmp = 0;
		tmp |= p.pos >> i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}
	// out of bounds check is handled in the loops
	return slm;
}

static Bitboard sudo_legal_knight(const Piece &p)
{
	Bitboard slm{0};

	slm |= p.pos << COL_SHIFT + 2;
	slm |= p.pos << (2 * COL_SHIFT) + 1;
	slm |= p.pos << COL_SHIFT - 2;
	slm |= p.pos << (2 * COL_SHIFT) - 1;
	slm |= p.pos >> COL_SHIFT + 2;
	slm |= p.pos >> (2 * COL_SHIFT) + 1;
	slm |= p.pos >> COLSHIFT - 2;
	slm |= p.pos >> (2 * COL_SHIFT) - 1;

	return slm & VALID_BOARD;
}

static Bitboard sudo_legal_bishop(const Piece &p, const Bitboard &friends,
		const Bitboard &enemies, const unsigned int depth)
{
	Bitboard slm{0};
	Bitboard tmp{0};
	// up right
	unsigned int count = 0;
	int i = 1;
	while (1) {
		tmp = 0;
		tmp |= p.pos << (COL_SHIFT * i) + i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}
	// up left 
	count = 0;
	i = 1;
	while (1) {
		tmp = 0;
		tmp |= p.pos << (COL_SHIFT * i) - i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}
	// down left 
	count = 0;
	i = 1;
	while (1) {
		tmp = 0;
		tmp |= p.pos >> (COL_SHIFT * i) + i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}
	// down right 
	count = 0;
	i = 1;
	while (1) {
		tmp = 0;
		tmp |= p.pos >> (COL_SHIFT * i) - i;
		if (tmp & ~VALID_BOARD)
			break;
		slm |= tmp;
		if (tmp & friends)
			break;
		if (tmp & enemies)
			++count;
		if (count == depth)
			break;
		++i;
	}

	return slm;	
}

static Bitboard sudo_legal_queen(const Piece &p, const Bitboard &friends,
		const Bitboard &enemies, const unsigned int depth)
{
	return sudo_legal_rook(p, friends, enemies, depth)
		| sudo_legal_bishop(p, friends, enemies, depth);
}

static Bitboard sudo_legal_king(const Piece &p)
{
}

void Piece::calc_sudo_legal_moves(const Bitboard friends, const Bitboard enemies,
		const bool pawn_move)
{
	switch (type) {
	case Type::PAWN:
		sudo_legal_moves = sudo_legal_pawn(*this, friends, enemies,
							pawn_move);
		break;
	case Type::ROOK:
		sudo_legal_moves = sudo_legal_rook(*this, friends, enemies, 1);
		break;
	case Type::KNIGHT:
		sudo_legal_moves = sudo_legal_knight(*this);
		break;
	case Type::BISHOP:
		sudo_legal_moves = sudo_legal_bishop(*this, friends, enemies, 1);
		break;
	case Type::QUEEN:
		sudo_legal_moves = sudo_legal_queen(*this, friends, enemies, 1);
		break;
	case Type::KING:
		sudo_legal_moves = sudo_legal_king(*this);
		break;
	default:
		// type is empty, should never be the case 
		cassert(0);
	}
}

void Piece::calc_one_deep_moves(const Bitboard friends, const Bitboard enemies)
{
	switch (type) {
	case Type::PAWN:
		one_deep_moves = sudo_legal_moves;
		break;
	case Type::ROOK:
		one_deep_moves = sudo_legal_rook(*this, friends, enemies, 2);
		break;
	case Type::KNIGHT:
		one_deep_moves = sudo_legal_moves;
		break;
	case Type::BISHOP:
		one_deep_moves = sudo_legal_bishop(*this, friends, enemies, 2);
		break;
	case Type::QUEEN:
		one_deep_moves = sudo_legal_queen(*this, friends, enemies, 2);
		break;
	case Type::KING:
		one_deep_moves = sudo_legal_moves;
		break;
	default:
		// type is empty, should never be the case 
		cassert(0);
	}
}
