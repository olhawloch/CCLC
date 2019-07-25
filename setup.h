#ifndef _SETUP_H_
#define _SETUP_H_

#include "boardstate.h"
#include <iostream>
#include "bitboard.h"
#include <string>
#include <sstream>

int convert_col(char letter);
Type convert_piece(char letter);
void setup(BoardState &bs);

#endif
