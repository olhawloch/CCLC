#include <iostream>
#include <string>
#include <sstream>
#include "piece.h"
#include "posn.h"
#include "team.h"
#include <vector>

using namespace std;

Type convert_piece(char letter);

int main() {
	
	Team w{Colour::WHITE};
	Team b{Colour::BLACK};

	string command;
	string line;

	while(1) {
		cout << "White: add x y type or done if done adding" << endl;
		getline(cin, line);
		if (cin.fail())
			break;

		stringstream ss(line);

		ss >> command;

		if (command == "add") {
			int x;
			int y;

			ss >> x;
			ss >> y;

			Posn tmp{x, y};
			Bitboard start = tmp.to_bitboard();
			char type;
			ss >> type;
			Type t = convert_piece(type);

			Piece p{start, t, Colour::WHITE};
			
			w.add_piece(p);
			
		} else if (command == "done") {
			break;
		} else {
			cout << "Incorrect command, add or done" << endl;
		}
	}

	while(1) {
		cout << "Black: add x y type or done if done adding" << endl;
		getline(cin, line);
		if (cin.fail())
			break;

		stringstream ss(line);

		ss >> command;

		if (command == "add") {
			int x;
			int y;

			ss >> x;
			ss >> y;

			Posn tmp{x, y};
			Bitboard start = tmp.to_bitboard();
			char type;
			ss >> type;
			Type t = convert_piece(type);

			Piece p{start, t, Colour::BLACK};

			b.add_piece(p);

		} else if (command == "done") {
			break;
		} else {
			cout << "Incorrect command, add or done" << endl;
		}
	}

	Bitboard w_posns = w.pos_pieces();
	Bitboard b_posns = b.pos_pieces();

	cout << "WHITE\n" << print_bitboard(w_posns) << endl;
	cout << "BLACK\n" << print_bitboard(b_posns) << endl;

	w.calc_sudo_legal_moves(w_posns, b_posns, true);
	b.calc_sudo_legal_moves(b_posns, w_posns, false);
	b.calc_one_deep_moves(b_posns, w_posns);

	while (1) {
		/*
		cout << "king_x king_y" << endl;
		getline(cin, line);
		if (cin.fail())
			break;
		stringstream ss(line);

		int x;
		int y;

		ss >> x;
		ss >> y;

		Posn king{x, y};
		*/
		Bitboard w_king = w.get_king_pos();
		auto lines = b.get_pinning_lines(w_king);
		for (auto line : lines) {
			cout << print_bitboard(line) << endl;
		}
		bool dc = b.is_double_check(w_king);
		auto checking = b.checking_pieces(w_king);

		Bitboard cl{0};
		for (auto &piece : checking) {
			cl |= piece.line_to_king(w_king);
		}	
		cout << print_bitboard(cl) << endl;

		w.calc_legal_moves(w_posns, b_posns, checking, cl, lines,
				b.get_sudo_legal_moves(), dc);
		cout << print_bitboard(w.get_legal_moves()) << endl;

		string ret = w.checkmate(b.get_sudo_legal_moves()) ? "Yes" : "No";
		cout << ret << " checkmate" << endl;

		break;
		/*
		cout << "from_x from_y to_x to_y" << endl;
		getline(cin, line);
		if (cin.fail())
			break;
		stringstream ss(line);

		int x;
		int y;

		ss >> x;
		ss >> y;

		Posn from{x, y};
		
		ss >> x;
		ss >> y;

		Posn to{x, y};

		Move m{to, from, Type::EMPTY};

		bool success = t.move_piece(m);
		if (success) {
			cout << "Move successful" << endl;
		} else {
			cout << "Move unsucessful" << endl;
		}

		team_posns = t.pos_pieces();

		cout << print_bitboard(team_posns) << endl;
		*/
	}

}


Type convert_piece(char letter)
{
	Type t = Type::EMPTY;

	switch (letter) {
	case 'p':
	case 'P':
		t = Type::PAWN;
		break;
	case 'r':
	case 'R':
		t = Type::ROOK;
		break;
	case 'n':
	case 'N':
		t = Type::KNIGHT;
		break;
	case 'b':
	case 'B':
		t = Type::BISHOP;
		break;
	case 'q':
	case 'Q':
		t = Type::QUEEN;
		break;
	case 'k':
	case 'K':
		t = Type::KING;
		break;
	}
	return t;
}
