#include "piece.h"
#include <cassert>

Bitboard Piece::get_pos() const {
	return pos;
}

Type Piece::get_type() const {
	return type;
}

Colour Piece::get_team() const {
	return team;
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

void Piece::set_type(Type t)
{
	type = t;
}

static Bitboard sudo_legal_pawn(const Piece &p, const Bitboard &friends,
		const Bitboard &enemies, const bool pawn_atk, const bool pawn_move)
{
	Bitboard slm{0};
	Bitboard tmp{0};
	const Bitboard empty{0};
	const Bitboard filled = friends | enemies;

	if (team == Colour::WHITE) {
		if (pawn_atk) {
			// diagonal attacks, left and right respectively
			slm |= p.pos << (COL_SHIFT - 1);
			slm |= p.pos << (COL_SHIFT + 1);
		}

		if (pawn_move) {
			// single move
			tmp |= (p.pos << COL_SHIFT) & ~filled;
			// double move
			tmp |= (tmp && p.pos & WHITE_DOUBLE_ROW) ? 
				(p.pos << 2 * COL_SHIFT) & ~filled : empty;
			slm |= tmp;
		}	
	} else {
		if (pawn_atk) {
			slm |= p.pos >> (COL_SHIFT - 1);
			slm |= p.pos >> (COL_SHIFT + 1);
		}
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
		tmp = p.pos << (COL_SHIFT * i);
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
		tmp = p.pos >> (COL_SHIFT * i);
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
		tmp = p.pos << i;
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
		tmp = p.pos >> i;
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
		tmp = p.pos << (COL_SHIFT * i) + i;
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
		tmp = p.pos << (COL_SHIFT * i) - i;
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
		tmp = p.pos >> (COL_SHIFT * i) + i;
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
		tmp = p.pos >> (COL_SHIFT * i) - i;
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
	Bitboard slm{0};

	slm |= p.pos >> COL_SHIFT;
	slm |= p.pos << COL_SHIFT;
	slm |= p.pos >> COL_SHIFT + 1;
	slm |= p.pos << COL_SHIFT + 1;
	slm |= p.pos >> COL_SHIFT - 1;
	slm |= p.pos << COL_SHIFT - 1;
	slm |= p.pos >> 1;
	slm |= p.pos << 1;

	return slm & VALID_BOARD;
}

void Piece::calc_sudo_legal_moves(const Bitboard friends, const Bitboard enemies,
		const bool pawn_move)
{
	switch (type) {
	case Type::PAWN:
		sudo_legal_moves = sudo_legal_pawn(*this, friends, enemies, true,
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
		assert(0);
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
		assert(0);
	}
}

static Bitboard not_check_legal_pawn(Piece &p, Bitboard friends,
		Bitboard enemies, const std::vector<Bitboard> &pinning)
{
	Bitboard lm = sudo_legal_pawn(p, friends, enemies, true, false) & enemies;
	lm |= sudo_legal_pawn(p, friends, enemies, false, true);	

	for (auto &line : pinning) {
		if (line & p.pos)
			return lm & line;
	}

	return lm;
}

static Bitboard check_legal_pawn(Piece &p, Bitboard friends, Bitboard enemies,
		Piece &checking, Bitboard checking_line)
{
	Bitboard lm = sudo_legal_pawn(p, friends, enemies, true, false)
		& checking.pos;
	lm |= sudo_legal_pawn(p, friends, enemies, false, true)
		& (checking_line & ~checking.pos);
	
	return lm;
}

static Bitboard not_check_legal_back_rank(Piece &p, Bitboard friends,
		const std::vector<Bitboard> &pinning)
{
	Bitboard lm = p.sudo_legal_moves & ~friends;

	for (auto &line : pinning) {
		if (line & p.pos)
			return lm & line;
	}

	return lm;
}

static Bitboard check_legal_back_rank(Piece &p, Bitboard friends,
		Bitboard checking_line)
{
	return (p.sudo_legal_moves & ~friends) & checking_line;
}

static Bitboard not_check_legal_king(Piece &p, Bitboard friends,
		Bitboard enemies_atk)
{
	return (p.sudo_legal_moves & ~friends) & ~enemies_atk;
}

static Bitboard check_legal_king(Piece &p, Piece &checking, Bitboard friends,
		Bitboard enemies_atk)
{
	return not_check_legal_king(p, friends, enemies_atk)
		& ~checking.one_deep_moves;
}

/*
 * enemies are those that 
 */
void Piece::calc_legal_moves(Bitboard friends, Bitboard enemies, Piece *checking,
		Bitboard checking_line, const std::vector<Bitboard> &pinning,
		Bitboard enemies_atk, bool double_check)
{
	// only king can move if in double check
	if (type != Type::KING && double_check) {
		legal_moves = 0;
		return;
	}

	switch (type) {
	case Type::PAWN:
		if (checking)
			legal_moves = check_legal_pawn(*this, friends, enemies,
					*checking, checking_line);
		else
			legal_moves = not_check_legal_pawn(*this, friends, enemies,
					pinning);
		break;
	case Type::ROOK:
		// use fall through, since rook, knight, bishop, and queen 
		// behave the same
	case Type::KNIGHT:
	case Type::BISHOP:
	case Type::QUEEN:
		if (checking)
			legal_moves = check_legal_back_rank(*this, checking_line);
		else
			legal_moves = not_check_legal_back_rank(*this, pinning);
		break;
	case Type::KING:
		if (checking)
			legal_moves = check_legal_king(*this, checking, friends,
					enemies_atk);
		else
			legal_moves = not_check_legal_king(*this, friends,
					enemies_atk);
		break;
	default:
		// type is empty, should never be the case 
		assert(0);
	}
}

static Bitboard line_to_king_rook(Bitboard p_pos, Bitboard king_pos)
{
	Bitboard ltk = p_pos;
	Bitboard tmp{0};

	int i = 1;

	if (p_pos < king_pos) {
		// check right
		while(1) {
			tmp = p_pos << i;
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				ltk = p_pos;
				break;
			ltk |= tmp;
			++i;
		}
		// check up
		i = 1;
		while(1) {
			tmp = p_pos << (COL_SHIFT * i);
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				break;
			ltk |= tmp;
			++i;
		}
	} else {
		// check left 
		i = 1;
		while(1) {
			tmp = p_pos >> i;
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				ltk = p_pos;
				break;
			ltk |= tmp;
			++i;
		}
		// check down 
		i = 1;
		while(1) {
			tmp = p_pos >> (COL_SHIFT * i);
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				break;
			ltk |= tmp;
			++i;
		}
	}

	return 0;
}

static Bitboard line_to_king_bishop(Bitboard p_pos, Bitboard king_pos)
{
	Bitboard ltk = p_pos;
	Bitboard tmp{0};

	int i = 1;

	if (p_pos < king_pos) {
		// check up left 
		while(1) {
			tmp = p_pos << (COL_SHIFT * i) - i;
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				ltk = p_pos;
				break;
			ltk |= tmp;
			++i;
		}
		// check up right
		i = 1;
		while(1) {
			tmp = p_pos << (COL_SHIFT * i) + i;
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				break;
			ltk |= tmp;
			++i;
		}
	} else {
		// check down left 
		i = 1;
		while(1) {
			tmp = p_pos >> (COL_SHIFT * i) + i;
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				ltk = p_pos;
				break;
			ltk |= tmp;
			++i;
		}
		// check down right
		i = 1;
		while(1) {
			tmp = p_pos >> (COL_SHIFT * i) - i;
			if (tmp & king_pos)
				return ltk;
			if (tmp & ~VALID_BOARD)
				assert(0);
			ltk |= tmp;
			++i;
		}
	}

	return 0;
}

// returns a line from the pos of piece to the king pos, 
// according to the piece's type, otherwise 0
Bitboard Piece::line_to_king(const Bitboard king_pos)
{
	Bitboatd ltk{0};

	switch (type) {
	case Type::PAWN:
		break;
	case Type::ROOK:
		ltk = line_to_king_rook(pos, king_pos);
		assert(ltk != 0);
		break;
	case Type::KNIGHT:
		break;
	case Type::BISHOP:
		ltk = line_to_king_bishop(pos, king_pos);
		assert(ltk != 0);
		break;
	case Type::QUEEN:
		ltk = line_to_king_rook(pos, king_pos)
			| line_to_king_bishop(pos, king_pos);
		assert(ltk != 0);
		break;
	case Type::KING:
		break;
	default:
		// type is empty, should never be the case 
		assert(0);
	}

	return ltk;
}
