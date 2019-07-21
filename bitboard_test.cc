#include "bitboard.h"
#include <iostream>

int main() {
	Bitboard tmp{0};
	std::cout << print_bitboard(tmp) << std::endl;
	tmp = 1;
	std::cout << print_bitboard(tmp) << std::endl;
	tmp = 512;
	std::cout << print_bitboard(tmp) << std::endl; 
	std::cout << print_bitboard(WHITE_DOUBLE_ROW) << std::endl;
	std::cout << print_bitboard(BLACK_DOUBLE_ROW) << std::endl;
	std::cout << print_bitboard(VALID_BOARD) << std::endl;
}
