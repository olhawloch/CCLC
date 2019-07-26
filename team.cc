#include "team.h"
#include <cassert>
#include <iostream>

Team::Team(Colour colour) : colour{colour}
{
}

bool Team::check(Bitboard enemies_atk) const
{
	const Piece *king = nullptr;
	for (auto &piece : pieces) {
		if (piece.get_type() == Type::KING) {
			king = &piece;
		}
	}
	return (king->get_pos() & enemies_atk).any();
}

std::vector<Piece> Team::checking_pieces(const Bitboard enemy_king_pos)
{
	std::vector<Piece> ret;

	for (auto &e : pieces) {
		if ((e.get_sudo_legal_moves() & enemy_king_pos).any())
			ret.emplace_back(e);
	}
	
	return ret;
}

bool Team::is_double_check(const Bitboard enemy_king_pos) const
{
	if (pieces.size() < 3)
		// only a king or a king and another piece
		return false;
	Bitboard tmp{0};
	tmp = ~tmp;

	for (auto &e : pieces) {
		tmp &= e.get_sudo_legal_moves();
	}

	return (tmp & enemy_king_pos).any();
}

Bitboard Team::pos_pieces() const
{
	Bitboard tmp{0};
	for (auto &piece : pieces) {
		tmp |= piece.get_pos();
	}
	return tmp;
}

std::vector<Bitboard> Team::get_pinning_lines(const Bitboard enemy_king_pos)
{
	std::vector<Bitboard> lines;
	for (auto &piece : pieces) {
		if ((piece.get_sudo_legal_moves() & enemy_king_pos).none()
				&& (piece.get_one_deep_moves() & enemy_king_pos).any()) {
			lines.emplace_back(piece.line_to_king(enemy_king_pos));
		}
	}
	return lines;
}	

void Team::calc_sudo_legal_moves(const Bitboard friends, const Bitboard enemies,
		bool pawn_move)
{
	for (auto &piece : pieces) {
		piece.calc_sudo_legal_moves(friends, enemies, pawn_move);
	
	}
}

void Team::calc_one_deep_moves(const Bitboard friends, const Bitboard enemies)
{
	for (auto &piece : pieces) {
		piece.calc_one_deep_moves(friends, enemies);
	}
}

void Team::calc_legal_moves(Bitboard friends, Bitboard enemies,
		std::vector<Piece> checking, Bitboard checking_line,
		const std::vector<Bitboard> &pinning, Bitboard enemies_atk,
		bool double_check)
{
	for (auto &piece : pieces) {
		piece.calc_legal_moves(friends, enemies, checking, checking_line,
				pinning, enemies_atk, double_check);
	}
}

Bitboard Team::get_sudo_legal_moves() const
{
	Bitboard tmp{0};
	for (auto &piece : pieces) {
		tmp |= piece.get_sudo_legal_moves();
	}
	return tmp;
}

Bitboard Team::get_legal_moves() const
{
	Bitboard tmp{0};	
	for (auto &piece : pieces) {
		tmp |= piece.get_legal_moves();
	}
	return tmp;
}

bool Team::checkmate(const Bitboard enemies_atk) const
{
	return check(enemies_atk) && get_legal_moves().none();
}

bool Team::move_piece(Move m)
{
	Bitboard from = m.from.to_bitboard();
	Bitboard to = m.to.to_bitboard();
	Piece *p = nullptr;

	for (auto &piece : pieces) {
		if (piece.get_pos() == from) {
			p = &piece;
			break;
		}
	}

	if (!p)
		return false;	
	if (p->get_type() == Type::PAWN && m.promotion != Type::EMPTY)
		p->set_type(m.promotion);
	p->set_pos(to);
	return true;
}

bool Team::is_valid_move(Move m)
{

	std::cout << "WHITE_PROMOTION" << std::endl;
	std::cout << print_bitboard(WHITE_PROMOTION) << std::endl;

	std::cout << "BLACK_PROMOTION" << std::endl;
	std::cout << print_bitboard(BLACK_PROMOTION) << std::endl;

	Bitboard from = m.from.to_bitboard();
	Bitboard to = m.to.to_bitboard();
	Piece *p = nullptr;

	for (auto &piece : pieces) {
		if (piece.get_pos() == from) {
			p = &piece;
			break;
		}
	}

	if (!p)
		return false;

	if (p->get_type() != Type::PAWN && m.promotion != Type::EMPTY)
		return false;

	if (p->get_type() == Type::PAWN) {
		if (m.promotion != Type::EMPTY) {
			if (p->get_team() == Colour::WHITE) {
				std::cout << "Type is pawn colour is white and non-empty promotion" << std::endl;
				to &= WHITE_PROMOTION;
			} else {
				std::cout << "Type is pawn colour is black and non-empty promotion" << std::endl;
				to &= BLACK_PROMOTION;
			}
		} else {
			if (p->get_team() == Colour::WHITE) {
				std::cout << "Type is pawn colour is white and empty promotion" << std::endl;
				to &= ~WHITE_PROMOTION;
			} else {
				std::cout << "Type is pawn colour is black and empty promotion" << std::endl;
				to &= ~BLACK_PROMOTION;
			}
		}
	}

	return (p->get_legal_moves() & to).any(); 
}

void Team::add_piece(Piece p)
{
	pieces.emplace_back(p);
}

bool Team::remove_piece(Bitboard pos)
{
	const int size = pieces.size();
	for (int i = 0; i < size; ++i) {
		if (pieces[i].get_pos() == pos) {
			pieces.erase(pieces.begin() + i);
			return true;
		}
	}
	return false;
}

static Posn to_posn(const Bitboard &b)
{
	int index = b.to_string().find("1");
	const int size = COL_SHIFT * COL_HEIGHT;
	int actual_index = size - 1 - index;
	int y = (actual_index % COL_SHIFT) - 1;
	int x = (actual_index / COL_SHIFT) - 2;
	return Posn{x, y};
}

std::string Team::print_team() const
{
	std::string board(64, ' ');
	int ascii_shift = 0;
	char p;
	if (colour == Colour::WHITE)
		ascii_shift = 'A' - 'a';

	for (auto piece : pieces) {
		switch (piece.get_type()) {
		case Type::PAWN:
			p = 'p'; 
			break;
		case Type::ROOK:
			p = 'r';
			break;
		case Type::KNIGHT:
			p = 'n';
			break;
		case Type::BISHOP:
			p = 'b';
			break;
		case Type::QUEEN:
			p = 'q';
			break;
		case Type::KING:
			p = 'k';
			break;
		default:
			// type is empty, should never be the case 
			assert(0);
		}
		p += ascii_shift;
		Posn pos = to_posn(piece.get_pos());
		board[pos.y * (COL_SHIFT - 2) + pos.x] = p;
	}
	return board;
}

Bitboard Team::get_king_pos() const
{
	const Piece *king = nullptr;
	for (auto &piece : pieces) {
		if (piece.get_type() == Type::KING) {
			king = &piece;
		}
	}
	if (king)
		return king->get_pos();

	return 0;
}
