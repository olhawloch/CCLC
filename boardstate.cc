#include "boardstate.h"

std::string BoardState::print_board() const
{
	std::string board;
	int size = 64;
	// empty board
	bool white_sqr = false;
	for (int i = 0; i < size; ++i) {
		if (white_sqr) {
			board[i] = " ";
			white_sqr = false;
		} else {
			board[i] = "-";
			white_sqr = true;
		}
	}

	for (auto &team : teams) {
		std::string pieces = team.print_team();
		for (int i = 0; i < size; ++i) {
			if (pieces[i] != " ") {
				//since only one piece/sqr, no team overlaps
				board[i] = pieces[i];
			}
		}
	}

	const int width = COL_SHIFT - 2;

	char tmp;
	// flip horizontally, so black is on top
	for (int i = 0; i < width / 2; ++i) {
		for (int j = 0; i < width; ++i) {
			board[i * width + j] = tmp;
			board[i * width + j] = board[(1 - i) * width - 1 + j];
			board[(1 - i) * width - 1 + j] = tmp;
		}
	}
	// adds row numbers and newlines
	for (int i = 0; i < width; ++i) {
		board.insert(i * width, to_string(width - i - 1) + " ");
		board.insert((i * width) + width, "\n") 
	}
	return board;
}

Colour BoardState::get_turn() const
{
	return turn;
}

void BoardState::set_turn(Colour c)
{
	turn = c;
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

	for (int i = 0; i < castling_rights.size(); ++i) {
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

void BoardState::calc_legal_moves()
{
	Team &attack = teams[0];
	Team &defend = teams[1];

	if (turn == Colour::BLACK) {
		attack = teams[1];
		defend = teams[0];
	}

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
		checking_line |= checking.line_to_king(attack_king_pos);
	}

	auto pinning = defend.get_pinning_lines(attack_king_pos);

	attack.calc_legal_moves(attack_pos, defend_pos, checking, checking_line,
			pinning, defend.get_sudo_legal_moves(), double_check);
}
