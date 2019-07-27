#include "bitboard.h"
#include <iostream>

using namespace std;

int main() {
	Bitboard tmp{0};
	const Bitboard pos{33554432};

	cout << print_bitboard(pos);
	tmp = pos << COL_SHIFT;
	cout << print_bitboard(tmp);
	tmp = pos << COL_SHIFT * 2 + 1;
	cout << print_bitboard(tmp);
}
