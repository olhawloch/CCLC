#include "boardstate.cc"
#include <iostream>

// returns negative if letter is not valid
int convert_col(char letter)
{	
	if ('a' > letter || letter > 'h')
		// not valid
		return -1;
	return letter - 'a' + 1;
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

void setup()
{
	// the two vectors that we are adding these pieces to
	std::vector<Piece> w;
	std::vector<Piece> b;

	Team white{w, Colour::WHITE};
	Team black{b, Colour::BLACK};

	std::vector<Team> teams{white, black};

	// pass this by refrence instead and make in main
	Boardstate bs{Colour::WHITE, "", 0, 0, 0, teams};

	//have to deal with things being public and private,
	//need getters for team vector

	Bitboard tmp{0};
	Bitboard empty{0}; //to initialize moves bitboards

	std::string command;
	std::string line;

	std::string piece_char;
	std::string posn;
	int row;
	int col;

	//this can only happen if we are not currently in the middle of a game
	while (std::cin >> command) {
		if (command == "+") {
			std::get_line(std::cin, line);
			std::stringstream ss(line);
			// get corresponding piece
			ss >> piece_char;
			if (piece_char.length() != 1)
				continue;
			Type type = convert_piece(piece_char[0]);
			if (type == Type::EMPTY)
				continue;
			// get position on board
			ss >> posn;
			if (posn.length() != 2)
				continue;
			col = convert_col(posn[0]);
			if (col < 0)
				continue;
			row = std::stoi(posn[1]);
			Posn add{row - 1, col - 1};
			tmp = add.to_bitboard();
			// see if it's replacing an already existing piece
			bs.teams[0].remove_piece(tmp)
				|| bs.teams[1].remove_piece(tmp);

			if (piece_char[0] < 'a') { //capital -> WHITE
				bs.teams[0].emplace_back(Piece{tmp, empty, empty,
						empty, type, Colour::WHITE});
			} else { //lower ->BLACK {
				bs.teams[1].emplace_back(Piece{tmp, empty, empty,
						empty, type, Colour::BLACK});
			}
			std::cout << bs.print_board();
		} else if (command == "-") {
			std::get_line(std::cin, line);
			std::stringstream ss(line);
			ss >> posn;
			if (posn.length() != 2)
				continue;
			col = convert_col(posn[0]);
			if (col < 0)
				continue;
			row = std::stoi(posn[1]);
			Posn remove{row - 1, col - 1};
			tmp = remove.to_bitboard();
			// try to remove from white first, then black 
			bs.teams[0].remove_piece(tmp)
				|| bs.teams[1].remove_piece(tmp);
			std::cout << bs.print_board();
		} else if (command == "=") {
			std::get_line(std::cin, line);
			std::stringstream ss(line);
			std::string colour;
			ss >> colour;
			ss >> line;
			if (line.length() != 0)
				// ensures one command per line
				continue;
			Colour to_play;
			if (colour == "white")
				to_play = Colour::WHITE;
			else if (colour == "black")
				to_play = Colour::BLACK;
			else
				continue;
			bs.set_colour(to_play);
		} else if (command == "done") {
			std::cout << "Leaving setup mode" << std::endl;
			return;
		} else { //invalid input 
			std::cout << "Invalid input, try again." << std::endl;
		}
	}
}
