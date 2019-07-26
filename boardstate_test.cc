#include <iostream>
#include <string>
#include <sstream>
#include "piece.h"
#include "posn.h"
#include "team.h"
#include <vector>
#include "boardstate.h"
#include "setup.h"

using namespace std;

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
		//this is where the board is setup, or if command is not setup, a default is created
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
		cout << "Begin playing: move from to or resign" << endl;
		getline(cin, line);

		stringstream ss{line};

		ss >> command;

		if (command == "move") {
			ss >> from; //read in e1
			cout << from << endl;
			if (from.length() != 2)
				continue;
			ss >> to;
			cout << to << endl;
			if (to.length() != 2)
				continue;

			col = convert_col(from[0]);
			row = from[1] - '0';
			Posn f{row - 1, col - 1};
			col = convert_col(to[0]);
			row = to[1] - '0';
			Posn t{row - 1, col - 1};

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

