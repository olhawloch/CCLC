#include "strategy.h"

Strategy::~Strategy()
{
}

// returns a move with promotion set to pawn if invalid input
Move Strategy::choose_move(BoardState &bs)
{
	CommandInterpreter CI{Notation::DEFAULT};
	std::string line;

	std::getline(std::cin, line);

	Move tmp{Posn{0, 0}, Posn{0, 0}, Type::PAWN};
	Move m;

	if (CI.is_valid_move(line))
		m = CI.interpret_move(line);
	else {
		std::cout << "1 Invalid move, try again" << std::endl;
		return tmp;
	}

	Team &active = (bs.get_turn() == Colour::WHITE)
		? bs.teams[0] : bs.teams[1];

	if (!active.is_valid_move(m)) {
		std::cout << "2 Invalid move, try again" << std::endl;
		return tmp;
	}

	return m;
}
