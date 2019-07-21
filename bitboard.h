#ifndef _BITBOARD_H_
#define _BITBOARD_H_

#include <bitset>
#include <string>


typedef std::bitset<120> Bitboard;

extern const Bitboard VALID_BOARD;

extern const Bitboard WHITE_DOUBLE_ROW;
extern const Bitboard BLACK_DOUBLE_ROW;

extern const int COL_SHIFT;
extern const int COL_HEIGHT;

std::string print_bitboard(const Bitboard &b);

const int COL_HEIGHT = 12;

/* The valid board
0000000000
0000000000
0111111110
0111111110
0111111110
0111111110
0111111110
0111111110
0111111110
0111111110
0000000000
0000000000
*/ 

/* White Double Row 
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0111111110
0000000000
0000000000
0000000000
*/ 

/* Black Double Row 
0000000000
0000000000
0000000000
0111111110
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
*/ 

#endif
