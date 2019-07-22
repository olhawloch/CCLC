#ifndef _POSN_H_
#define _POSN_H_

#include "bitboard.h"

struct Posn {
	int x;
	int y;
	
	Bitboard to_bitboard()
	{
		Bitboard tmp{0};
		tmp[(y + 2) * COL_SHIFT + x + 1] = 1;
		return tmp;
	}
};

#endif
