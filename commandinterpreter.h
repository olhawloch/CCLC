#ifndef _COMMANDINTERPRETER_H_
#define _COMMANDINTERPRETER_H_

#include "notation.h"
#include "posn.h"
#include "move.h"

class CommandInterpreter {
	Notation syntax;

	public:
	explicit CommandInterpreter(Notation syntax);

	bool is_valid_posn(std::string s) const;
	Posn interpret_posn(std::string s) const;
	bool is_valid_move(std::string s) const;
	Move interpret_move(std::string s) const;
};

Type convert_piece(char letter);

#endif
