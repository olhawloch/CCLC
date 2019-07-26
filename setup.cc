#include "setup.h"

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

void setup(BoardState &bs)
{
	Bitboard tmp{0};
	Bitboard empty{0}; //to initialize moves bitboards

	std::string command;
	std::string line;

	std::string piece_char;
	std::string posn;
	int row;
	int col;

	int w_king = 0;
	int b_king = 0;

	//this can only happen if we are not currently in the middle of a game
	while (1) {
		std::cout << "enter + - = or done" << std::endl;
		if (!(std::cin >> command))
			break;
		if (command == "+") {
			std::getline(std::cin, line);
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
			row = posn[1] - '0';
			Posn add{col - 1, row - 1};
			tmp = add.to_bitboard();
			// see if it's replacing an already existing piece
			bs.teams[0].remove_piece(tmp)
				|| bs.teams[1].remove_piece(tmp);

			if (piece_char[0] < 'a') { //capital -> WHITE
				if (type == Type::PAWN) {
					if (row == 8) {
						std::cout << "Cannot put a white pawn on 8th rank, please try again" << std::endl;
						continue;
					}
				}
				bs.teams[0].add_piece(Piece{tmp, type, Colour::WHITE});
			} else { //lower ->BLACK 
				if (type == Type::PAWN) {
					if (row == 1) {
						std::cout << "Cannot put a black pawn on 1st rank, please try again" << std::endl;
						continue;
					}
				}
				bs.teams[1].add_piece(Piece{tmp, type, Colour::BLACK});
			}
			std::cout << bs.print_board();
		} else if (command == "-") {
			std::getline(std::cin, line);
			std::stringstream ss(line);
			ss >> posn;
			if (posn.length() != 2)
				continue;
			col = convert_col(posn[0]);
			if (col < 0)
				continue;
			row = posn[1] - '0';
			Posn remove{col - 1, row - 1};
			tmp = remove.to_bitboard();
			// try to remove from white first, then black 
			bs.teams[0].remove_piece(tmp)
				|| bs.teams[1].remove_piece(tmp);
			std::cout << bs.print_board();
		} else if (command == "=") {
			std::getline(std::cin, line);
			std::stringstream ss(line);
			std::string colour;
			std::string garbage;
			ss >> colour;
			std::cout << colour << std::endl;
			ss >> garbage;
			std::cout << garbage << std::endl;
			if (garbage.length() != 0)
				// ensures one command per line
				continue;
			Colour to_play;
			if (colour == "white") {
				std::cout << "white is set to play first" << std::endl;
				to_play = Colour::WHITE;
			} else if (colour == "black") {
				std::cout << "black is set to play first" << std::endl;
				to_play = Colour::BLACK;
			} else {
				std::cout << "That is not a valid colour" << std::endl;
				continue;
			}
			bs.set_turn(to_play);
		} else if (command == "done") {
			Bitboard w_king = bs.teams[0].get_king_pos();
			Bitboard b_king = bs.teams[1].get_king_pos();
			if (w_king.count() == 1 && b_king.count() == 1) {
				// check no pawns on first or last row
				// check neither king is in check
				bs.calc_sudo_legal_moves();
				Team &w_team = bs.teams[0];
				Team &b_team = bs.teams[1];
				Bitboard w_atk = bs.teams[0].get_sudo_legal_moves();
				Bitboard b_atk = bs.teams[1].get_sudo_legal_moves();
				if (w_team.check(b_atk) || b_team.check(w_atk)) {
					std::cout << "Can't start in check" << std::endl;
					continue;
				} else {
					std::cout << "Leaving setup mode" << std::endl;
					return;
				}
			} else {
				std::cout << "Please ensure both teams have 1 king on the board" << std::endl;
				continue;
			}
		} else { //invalid input 
			std::cout << "Invalid input, try again." << std::endl;
		}
	}
}
