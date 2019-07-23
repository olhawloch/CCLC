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
	if (less_than(WHITE_DOUBLE_ROW, BLACK_DOUBLE_ROW)) {
		std::cout << "White Double Row less than" << std::endl;
	} else {
		std::cout << "White Double Row not less than" << std::endl;
	}
	Bitboard tmp1{0};
	Bitboard tmp2{0};
	if (less_than(tmp1, tmp2)) {
		std::cout << "tmp1 less than" << std::endl;
	} else {
		std::cout << "tmp1 not less than" << std::endl;
	}
	tmp1 = 1;
	if (less_than(tmp2, tmp1)) {
		std::cout << "tmp2 less than" << std::endl;
	} else {
		std::cout << "tmp2 not less than" << std::endl;
	}
}
