#include "strategy.h"

Strategy::~Strategy()
{
}

Move Strategy::choose_move(BoardState &bs)
{
	CommandInterpreter CI{Notation::DEFAULT};
	std::string line;

	while(1) {
		std::getline(std::cin, line);

		Move m;
		if (CI.is_valid_notation(line))
			m = CI.interpret_move(line);
		else {
			std::cout << "Invalid move, try again" << std::endl;
			continue;
		}

		if (bs.get_turn() == Colour::WHITE) {
			if (!bs.teams[0].is_valid_move(m)) {
				std::cout << "{" << m.from.x << ", " << m.from.y << "} " 
					<< "{" << m.to.x << ", " << m.to.y << "} " << std::endl;

				std::cout << "Invalid move, try again" << std::endl;
				continue;
			}
		} else {
			if (!bs.teams[1].is_valid_move(m)) {
				std::cout << "{" << m.from.x << ", " << m.from.y << "} " 
					<< "{" << m.to.x << ", " << m.to.y << "} " << std::endl;
				std::cout << "Invalid move, try again" << std::endl;
				continue;
			}
		}
		return m;
	}
}
