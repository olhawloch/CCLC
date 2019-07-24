#include "piece.h"
#include <iostream>
using namespace std;

int main()
{
	Piece p{34359738368, Type::QUEEN, Colour::WHITE};
	/*
	cout << p.get_type() << endl;
	cout << p.get_team() << endl;
	*/
	cout << print_bitboard(p.get_pos()) << endl;
	p.calc_sudo_legal_moves(0, 0, true);
	cout << print_bitboard(p.get_sudo_legal_moves()) << endl;
}
