#include "commandinterpreter.h"
#include <cassert>
#include <sstream>

CommandInterpreter::CommandInterpreter(Notation syntax): syntax{syntax}
{
}

// returns EMPTY if letter not valid
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

// returns negative if letter is not valid
static int convert_col(char letter)
{	
	if ('a' > letter || letter > 'h')
		// not valid
		return -1;
	return letter - 'a';
}

static bool is_valid_posn_default(std::string s)
{
	std::stringstream ss(s);
	std::string tmp;

	ss >> tmp;
	if (tmp.length() != 2)
		return false;
	if (convert_col(tmp[0]) < 0)
		return false;
	if (tmp[1] < '1' || tmp[1] > '8')
		return false;

	return true;
}

static Posn interpret_posn_default(std::string s)
{
	int x;
	int y;
	std::stringstream ss(s);
	std::string tmp;

	ss >> tmp;
	x = convert_col(tmp[0]);
	y = tmp[1] - '1';
	return Posn {x, y};
}

bool CommandInterpreter::is_valid_posn(std::string s) const
{
	bool ret;

	if (syntax == Notation::DEFAULT) {
		ret = is_valid_posn_default(s);
	} else if (syntax == Notation::ALGEBRAIC) {
		assert(0);
		//ret = is_valid_algebraic(s);
	}

	return ret;
}

Posn CommandInterpreter::interpret_posn(std::string s) const
{
	Posn ret;

	if (syntax == Notation::DEFAULT) {
		ret = interpret_posn_default(s);
	} else if (syntax == Notation::ALGEBRAIC) {
		assert(0);
		//ret = is_valid_algebraic(s);
	}

	return ret;
}

static bool is_valid_move_default(std::string s)
{
	std::stringstream ss(s);
	std::string from;
	std::string to;
	std::string promotion;

	ss >> from;
	ss >> to;
	
	ss >> promotion;
	if (promotion.length() > 1)
		return false;
	if (promotion.length() == 1
			&& (convert_piece(promotion[0]) == Type::EMPTY 
				|| convert_piece(promotion[0]) == Type::PAWN))
		return false;

	return is_valid_posn_default(from) && is_valid_posn_default(to);
}

// default notation is: from to promotion
static Move interpret_move_default(std::string s)
{
	std::stringstream ss(s);
	Type promotion;
	std::string tmp;

	ss >> tmp;
	Posn from = interpret_posn_default(tmp);
	ss >> tmp;
	Posn to = interpret_posn_default(tmp);

	std::string promot;
	ss >> promot;
	promotion = (promot.length() == 0)
		? Type::EMPTY : convert_piece(promot[0]);

	return Move{to, from, promotion};
}

bool CommandInterpreter::is_valid_move(std::string s) const
{
	bool ret;

	if (syntax == Notation::DEFAULT) {
		ret = is_valid_move_default(s);
	} else if (syntax == Notation::ALGEBRAIC) {
		assert(0);
		//ret = is_valid_algebraic(s);
	}

	return ret;
}

Move CommandInterpreter::interpret_move(std::string s) const
{
	Move ret;

	if (syntax == Notation::DEFAULT) {
		ret = interpret_move_default(s);
	} else if (syntax == Notation::ALGEBRAIC) {
		assert(0);
		//ret = interpret_algebraic(s);
	}

	return ret;
}
