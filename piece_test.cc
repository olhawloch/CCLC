#include "piece.h"
#include "posn.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

Type convert_piece(char letter);

int main()
{
	Type t;
	Colour c;
	string line;

	while(1) {
		cout << "Input: x y type king_x king_y" << endl;

		getline(cin, line); 
		if (cin.fail())
			break;
		stringstream ss(line);
		int x;
		int y;

		ss >> x;
		ss >> y;

		Posn tmp{x, y};
		Bitboard start = tmp.to_bitboard();
		char type;
		ss >> type;
		t = convert_piece(type);

		Piece p{start, t, Colour::BLACK};

		ss >> x;
		ss >> y;

		Posn king_tmp{x, y};
		Bitboard king_pos = king_tmp.to_bitboard();
		Piece enemy_king{king_pos, Type::KING, Colour::WHITE};

		Bitboard king_line = p.line_to_king(king_pos);

		cout << print_bitboard(king_line) << endl;
		/*
		Piece boarder{start, Type::KING, Colour::WHITE};
		boarder.calc_sudo_legal_moves(0, 0, false);

		cout << "Type is: " << p.get_type() << endl;
		cout << print_bitboard(p.get_pos()) << endl;

		p.calc_one_deep_moves(0,
				WHITE_DOUBLE_ROW | boarder.get_sudo_legal_moves());

		cout << print_bitboard(p.get_one_deep_moves()) << endl;
		*/
	}

	return 0;
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
