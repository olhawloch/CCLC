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

Type convert_piece(char letter);

int main()
{
	BoardState bs;

	string command;
	string line;

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
		cout << "Begin playing :)" << endl;
		getline(cin, line);

		stringstream ss{line};

		ss >> command;

		if (command == "move") {

		} else if (command == "resign") {

		} else {
			cout << "Incorrect input, please try again" << endl;
		}
		break;
	}
}

