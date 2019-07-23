#include "boardstate.cc"
#include <iostream>

int convert_col(char letter)
{	
	switch (letter) {
	case 'a':
		return 1;
	case 'b':
		return 2;
	case 'c':
		return 3;
	case 'd':
		return 4;
	case 'e':
		return 5;
	case 'f':
		return 6;
	case 'g':
		return 7;
	case 'h':
		return 8;
	default:
		// not from a-h, should never be the case 
		assert(0);
	}
}

Type convert_piece(char letter)
{
	switch (letter) {
	case ('p' || 'P')
		return Type::PAWN;
	case ('r' || 'R')
		return Type::ROOK;
	case ('n' || 'N')
		return Type::KNIGHT;
	case ('b' || 'B') 
		return Type::BISHOP;
	case ('q' || 'Q') 
		return Type::QUEEN;
	case ('k' || 'K') 
		return Type::KING;
	default:
		// type is empty, should never be the case 
		assert(0);
	}
}

int main()
{
	// the two vectors that we are adding these pieces to
	std::vector<Pieces> white;
	std::vector<Pieces> black;

	Team white{white, Colour::WHITE};
	Team black{black, Colour::BLACK};

	std::vector<Team> teams{white, black};

	//have to immediately make it a boardstate so that we can print each time
	Boardstate bs{Colour::WHITE, "", 0, 0, 0, teams};

	//have to deal with things being public and private, need getters for team vector

	Bitboard tmp{0};
	Bitboard empty{0}; //to initialize moves bitboards
	std::string s;
	std::string piece;
	char piece_char;
	std::string posn;
	char col_char;
	int row;
	int col;

	//this can only happen if we are not currently in the middle of a game
	while (cin >> s) {
		if (s == "+") {
			cin >> piece;
			stringstream type(piece);
			type >> piece_char;
			cin >> posn;
			stringstream p(posn);
			p >> col_char;
			col = convert_col(col_char);
			p >> row;
			Type type = convert_piece(piece_char);
			Posn add{row, col};
			tmp = add.to_bitboard();
			//NOTE THAT IF A PIECE ALREADY EXISTS THERE WE MUST REPLACE IT
			if (piece_char < 'a') { //capital -> WHITE
				//add piece with Posn{col, row}
				white.emplace_back(Piece{tmp, empty, empty, empty, type, Colour::WHITE});
			} else { //lower ->BLACK {
				//add piece with Posn{col, row}
				black.emplace_back(Piece{tmp, empty, empty, empty, type, Colour::BLACK});
			}
		} else if (s == "-") {
			cin >> posn;
			stringstream p(posn);
			p >> col_char;
			col = convert_col(col_char);
			p >> row;
			Posn remove{row, col};
			tmp = remove.to_bitboard();
			//remove the piece from that posn, must iterate through each team to find
			//check white vector first
			int size = white.size();
			int found = false;
			for (int i = 0; i < size; ++i) {
				if (white[i].get_pos() == tmp) {
					white.erase(i);
					found = true;
					break;
				}
			}
			if (!found) {
				size = black.size();
				for (int i = 0; i < size; ++i) {
					if (black[i].get_pos() == tmp) {
						black.erase(i);
						break;
					}
				}
			}
		} else if (s == "=") {

		} else if (s == "done") {
			std::cout << "Leaving setup mode" << std::endl;
		} else { //invalid input 
			std::cout << "Invalid input, try again." << std::endl;
		}
	}
}
