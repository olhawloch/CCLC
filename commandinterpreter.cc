#include "commandinterpreter.h"
#include <cassert>
#include <sstream>

CommandInterpreter::CommandInterpreter(Notation syntax): syntax{syntax}
{
}

// returns EMPTY if letter not valid
static Type convert_piece(char letter)
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

// returns negative if letter is not valid
static int convert_col(char letter)
{	
	if ('a' > letter || letter > 'h')
		// not valid
		return -1;
	return letter - 'a';
}

static bool is_valid_default(std::string s)
{
	std::stringstream ss(s);
	std::string from;
	std::string to;

	ss >> from;
	if (from.length() != 2)
		return false;
	if (convert_col(from[0]) < 0)
		return false;
	if (from[1] < '1' || from[1] > '8')
		return false;
	ss >> to;
	if (to.length() != 2)
		return false;
	if (convert_col(to[0]) < 0)
		return false;
	if (to[1] < '1' || to[1] > '8')
		return false;

	return true;
}

// default notation is: from to promotion
static Move interpret_default(std::string s)
{
	int x;
	int y;
	Type promotion;
	std::stringstream ss(s);
	std::string tmp;

	ss >> tmp;
	x = convert_col(tmp[0]);
	y = tmp[1] - '1';
	Posn from{x, y};
	ss >> tmp;
	x = convert_col(tmp[0]);
	y = tmp[1] - '1';
	Posn to{x, y};
	std::string promot;
	ss >> promot;
	if (promot.length() == 0)
		promotion = Type::EMPTY;
	else
		promotion = convert_piece(promot[0]);

	return Move{to, from, promotion};
}

bool CommandInterpreter::is_valid_notation(std::string s)
{
	bool ret;

	if (syntax == Notation::DEFAULT) {
		ret = is_valid_default(s);
	} else if (syntax == Notation::ALGEBRAIC) {
		assert(0);
		//ret = is_valid_algebraic(s);
	}

	return ret;
}

Move CommandInterpreter::interpret_move(std::string s)
{
	Move ret;

	if (syntax == Notation::DEFAULT) {
		ret = interpret_default(s);
	} else if (syntax == Notation::ALGEBRAIC) {
		assert(0);
		//ret = interpret_algebraic(s);
	}

	return ret;
}
