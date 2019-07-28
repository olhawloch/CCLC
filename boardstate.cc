#include "boardstate.h"
#include <iostream>
#include <algorithm>

BoardState::BoardState() : turn{Colour::WHITE}, castling_rights{""}, half_turn{0},
			full_turn{0}, enpassant_sqr{0}
			{
				teams.emplace_back(Team{Colour::WHITE});
				teams.emplace_back(Team{Colour::BLACK});
			}

std::string BoardState::print_board() const
{
	int size = 64;
	int width = COL_SHIFT - 2;
	std::string board;
	board.reserve(size);
	// empty board
	bool white_sqr = false;
	for (int i = 0; i < size; ++i) {
		if (white_sqr) {
			board[i] = ' ';
			if (!(i % width == 7)) {
				white_sqr = false;	
			}
		} else {
			board[i] = '-';
			if (!(i % width == 7)) {
				white_sqr = true;
			}
		}
	}
	for (auto &team : teams) {
		std::string pieces = team.print_team();
		for (int i = 0; i < size; ++i) {
			if (pieces[i] != ' ') {
				//since only one piece/sqr, no team overlaps
				board[i] = pieces[i];
			}
		}
	}
	
	// flip horizontally, so black is on top
	for (int i = 0; i < width / 2; ++i) {
		for (int j = 0; j < width; ++j) {
			std::swap(board[(i * width) + j], board[width * (width - 1 - i) + j]);
		}
	}

	std::string final_board;
	for (int i = 0; i < size; ++i) {
		if (i % width == 0) {
			if (i != 0)
				final_board += '\n';
			final_board += std::to_string(width - (i / width));
			final_board += " ";
		}
		final_board += board[i];
	}

	final_board += "\n  abcdefgh\n";

	return final_board;
}

Colour BoardState::get_turn() const
{
	return turn;
}

void BoardState::set_turn(Colour c)
{
	turn = c;
}

void BoardState::toggle_turn() {
	turn = (turn == Colour::WHITE) ? Colour::BLACK : Colour::WHITE;
}

Bitboard BoardState::get_castling_rights(Colour team) const
{
	char king_side = 'k';
	char queen_side = 'q';
	bool white = false;
	if (team == Colour::WHITE) {
		white = true;
		king_side += 'A' - 'a';
		queen_side += 'A' - 'a';
	}

	Bitboard tmp{0};

	for (size_t i = 0; i < castling_rights.size(); ++i) {
		if (castling_rights[i] == king_side) {
			if (white) {
				tmp |= WHITE_KS_CASTLE;
			} else {
				tmp |= BLACK_KS_CASTLE;
			}
		} else if (castling_rights[i] == queen_side) {
			if (white) {
				tmp |= WHITE_QS_CASTLE;
			} else {
				tmp |= BLACK_QS_CASTLE;
			}
		}
	}

	return tmp;
}

unsigned int BoardState::get_half_turn() const
{
	return half_turn;
}

unsigned int BoardState::get_full_turn() const
{
	return full_turn;
}

Bitboard BoardState::get_enpassant_sqr() const
{
	return enpassant_sqr;
}

void BoardState::calc_sudo_legal_moves()
{
	Bitboard w_pos = teams[0].pos_pieces();
	Bitboard b_pos = teams[1].pos_pieces();
	teams[0].calc_sudo_legal_moves(w_pos, b_pos, false);
	teams[1].calc_sudo_legal_moves(b_pos, w_pos, false);
}

void BoardState::calc_legal_moves()
{
	Team &attack = (turn == Colour::WHITE) ? teams[0] : teams[1];
	Team &defend = (turn == Colour::WHITE) ? teams[1] : teams[0];

	Bitboard attack_pos = attack.pos_pieces();
	Bitboard defend_pos = defend.pos_pieces();

	defend.calc_sudo_legal_moves(defend_pos, attack_pos, false);
	defend.calc_one_deep_moves(defend_pos, attack_pos);

	attack.calc_sudo_legal_moves(attack_pos, defend_pos, true);
	
	Bitboard attack_king_pos = attack.get_king_pos();

	std::vector<Piece> checking = defend.checking_pieces(attack_king_pos);
	bool double_check = defend.is_double_check(attack_king_pos);
	Bitboard checking_line{0}; 

	for (auto &piece : checking) {
		checking_line |= piece.line_to_king(attack_king_pos);
	}

	auto pinning = defend.get_pinning_lines(attack_king_pos);

	Bitboard castling = get_castling_rights(turn);

	attack.calc_legal_moves(attack_pos, defend_pos, checking, checking_line,
			pinning, defend.get_sudo_legal_moves(), double_check,
			castling, enpassant_sqr);
}

bool BoardState::move(Move m)
{
	Team &attack = (turn == Colour::WHITE) ? teams[0] : teams[1];
	Team &defend = (turn == Colour::WHITE) ? teams[1] : teams[0];

	Piece *moved = attack.get_piece_at(m.from.to_bitboard());
	if (moved->get_type() == Type::KING) {
		if (m.from.x - m.to.x > 1) {
			Piece *rook = attack.get_piece_at(
					Posn{m.from.x - 4, m.from.y}.to_bitboard());
			Posn rook_to{m.from.x - 1, m.from.y};
			rook->set_pos(rook_to.to_bitboard());
		} else if (m.to.x - m.from.x > 1) {
			Piece *rook = attack.get_piece_at(
					Posn{m.from.x + 3, m.from.y}.to_bitboard());
			Posn rook_to{m.from.x + 1, m.from.y};
			rook->set_pos(rook_to.to_bitboard());
		}
	}

	bool captured;
	if (enpassant_sqr.any() && moved->get_type() == Type::PAWN
			&& (enpassant_sqr & m.to.to_bitboard()).any()) {
		int offset = (turn == Colour::WHITE) ? -1 : 1;
		captured = defend.remove_piece(
				Posn{m.to.x, m.to.y + offset}.to_bitboard());
	} else {
		captured = defend.remove_piece((m.to).to_bitboard());
	}
	attack.move_piece(m);

	return captured;
}

bool BoardState::checkmate() const
{
	const Team &attack = (turn == Colour::WHITE) ? teams[0] : teams[1];
	const Team &defend = (turn == Colour::WHITE) ? teams[1] : teams[0];
	return attack.checkmate(defend.get_sudo_legal_moves());
}

bool BoardState::stalemate() const
{
	const Team &attack = (turn == Colour::WHITE) ? teams[0] : teams[1];
	// it's stalemate if the attacking side has no moves
	return !(attack.get_legal_moves().any());
}

bool BoardState::check() const
{
	const Team &attack = (turn == Colour::WHITE) ? teams[0] : teams[1];
	const Team &defend = (turn == Colour::WHITE) ? teams[1] : teams[0];
	return attack.check(defend.get_sudo_legal_moves());
}

void BoardState::set_castling_rights(std::string s)
{
	castling_rights = s;
}

void BoardState::set_castling_rights(Move m)
{
	Team &attack = (turn == Colour::WHITE) ? teams[0] : teams[1];
	const Bitboard castling = get_castling_rights(get_turn());

	if (castling.none())
		return;

	int ascii_shift = 0;

	if (get_turn() == Colour::WHITE)
		ascii_shift = 'A' - 'a';

	Piece *moved = attack.get_piece_at(m.from.to_bitboard());
	Type type = moved->get_type();

	if (type == Type::KING) {
		castling_rights.erase(std::remove(castling_rights.begin(), 
			castling_rights.end(), 'q' + ascii_shift),
			castling_rights.end());

		castling_rights.erase(std::remove(castling_rights.begin(), 
			castling_rights.end(), 'k' + ascii_shift),
			castling_rights.end());
	}
	else if (type == Type::ROOK) {
		if (m.from.x == 0) {
			// left rook, remove queen side castling rights
			castling_rights.erase(std::remove(castling_rights.begin(), 
				castling_rights.end(), 'q' + ascii_shift),
				castling_rights.end());
		} else if (m.from.x == 7) {
			// right rook, remove king side castling rights	
			castling_rights.erase(std::remove(castling_rights.begin(), 
				castling_rights.end(), 'k' + ascii_shift),
				castling_rights.end());
		}
	}
}

// must be called after piece has already been moved
void BoardState::set_enpassant_sqr(Move m)
{
	Team &attack = (turn == Colour::WHITE) ? teams[0] : teams[1];
	Piece *moving = attack.get_piece_at(m.to.to_bitboard());
	Type type = moving->get_type();

	enpassant_sqr = 0;
	if (type != Type::PAWN) {
		return;
	}

	// Pawn move
	if (m.to.y - m.from.y > 1) { // white pawn moving 2 up
		enpassant_sqr = Posn{m.from.x, m.from.y + 1}.to_bitboard();
	}
	if (m.from.y - m.to.y > 1) { // black pawn moving 2 down
		enpassant_sqr = Posn{m.from.x, m.from.y - 1}.to_bitboard();
	}
	return;
}
