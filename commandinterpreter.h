#ifndef _COMMANDINTERPRETER_H_
#define _COMMANDINTERPRETER_H_

#include "notation.h"
#include "posn.h"
#include "move.h"

class CommandInterpreter {
	Notation syntax;

	public:
	explicit CommandInterpreter(Notation syntax);
	bool is_valid_notation(std::string s);
	Move interpret_move(std::string s);
};

#endif
