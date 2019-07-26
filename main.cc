#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "posn.h"
#include "piece.h"
#include "team.h"
#include "boardstate.h" 

using namespace std;

int convert_col(char letter);
Type convert_piece(char letter);
void setup(BoardState &bs);


int main()
{
	BoardState bs;

	string command;
	string line;
	
	string from;
	string to;
	int row;
	int col;
	string promotion;

	while(1) {
		//this is where the board is setup, or if command is not setup,
		//a default is created
		cout << "play or setup" << endl;
		getline(cin, line);

		stringstream ss{line};

		ss >> command;

		if (command == "setup") {
			setup(bs);
		} else if (command == "game") {
			// setup default board
			// only setup as of right now
			cout << "This isn't functioning yet and will take you to play with nothing" << endl;
		} else {
			cout << "Incorrect input, play or setup" << endl;
		}
		break;
	}
	
	while(1) {
		//this is where the game is played
		bs.calc_legal_moves();
		if (bs.checkmate()) {
			string winner = (bs.get_turn() == Colour::WHITE) 
				? "Black" : "White";
			cout << winner << " wins!" << endl;
			break;
		} else if (bs.stalemate()) {
			cout << "Tie!" << endl;
			break;
		}

		cout << "Begin playing: move from to or resign" << endl;
		cin >> command;
		if (cin.fail())
			break;

		getline(cin, line);
		stringstream ss{line};

		if (command == "move") {
			ss >> from;
			if (from.length() != 2)
				continue;
			ss >> to;
			if (to.length() != 2)
				continue;

			col = convert_col(from[0]);
			row = from[1] - '0';
			Posn f{col - 1, row - 1};
			col = convert_col(to[0]);
			row = to[1] - '0';
			Posn t{col - 1, row - 1};

			ss >> promotion;

			if (promotion.length() > 1) // should be 0 or 1
				continue;

			Type piece_type = convert_piece(promotion[0]);

			Move m{t, f, piece_type};

			if (bs.get_turn() == Colour::WHITE) {
				cout << "we in here" << endl;
				if (bs.teams[0].is_valid_move(m)) {
					bs.move(m);
					bs.set_turn(Colour::BLACK);
				} else {
					cout << "Invalid move, white try again" << endl;
				}
			} else {
				if (bs.teams[1].is_valid_move(m)) {
					bs.move(m);
					bs.set_turn(Colour::WHITE);
				} else {
					cout << "Invalid move, black try again" << endl;
				}
			}
			cout << bs.print_board() << endl;
		} else if (command == "resign") {
			cout << "game ended" << endl;
			break;
		} else {
			cout << "Incorrect input, please try again" << endl;
		}
	}
}

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
		std::cout << "input: + - = or done" << std::endl;
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
