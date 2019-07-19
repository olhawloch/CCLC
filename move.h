#ifndef _MOVE_H_
#define _MOVE_H_

#include "posn.h"
#include "type.h"

struct Move {
	Posn to;
	Posn from;
	Type promotion;
};

#endif
