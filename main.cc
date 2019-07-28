#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>

#include "posn.h"
#include "piece.h"
#include "team.h"
#include "boardstate.h" 
#include "commandinterpreter.h"
#include "player.h"

using namespace std;

void setup(BoardState &bs);
int play_game(stack<BoardState> &states, Player &one, Player &two);
void default_board(BoardState &bs);


int main()
{
	BoardState bs;
	default_board(bs);

	stack<BoardState> states;
	states.push(bs);

	string command;
	string line;

	Player one{Colour::WHITE};
	Player two{Colour::BLACK};

	while(1) {
		cout << "input: game or setup" << endl;

		cin >> command;
		if (cin.fail())
			break;
		getline(cin, line);
		stringstream ss{line};

		if (command == "setup") {
			setup(bs);
			states.push(bs);
		} else if (command == "game") {
			ss >> command;
			if (command == "human" || command == "computer1" 
					|| command == "computer2"
					|| command == "computer3"
					|| command == "computer4") {
				one.set_strategy(command);
			} else {
				cout << "Incorrect input" << endl;
			}
			ss >> command;
			if (command == "human" || command == "computer1"
					|| command == "computer2"
					|| command == "computer3"
					|| command == "computer4") {
				two.set_strategy(command);
				play_game(states, one, two);
				while(states.size() != 1) {
					states.pop();
				}
			} else {
				cout << "Incorrect input" << endl;
			}
		} else {
			cout << "Incorrect input" << endl;
		}
	}

	cout << "Final Score:\n" << "White: " << one.score
		<< "\n" << "Black: " << two.score << endl;
	return 0;
}

void setup(BoardState &bs)
{
	CommandInterpreter CI{Notation::DEFAULT};
	Bitboard empty{0}; //to initialize moves bitboards

	std::string command;
	std::string line;

	std::string piece_char;
	std::string posn;

	while (1) {
		std::cout << "input: + - = or done" << std::endl;
		if (!(std::cin >> command))
			break;

		std::getline(std::cin, line);
		std::stringstream ss{line};

		if (command == "+") {
			ss >> piece_char;
			if (piece_char.length() != 1) {
				cout << "Invalid input, try again" << endl;
				continue;
			}

			Type type = convert_piece(piece_char[0]);

			ss >> line;

			if (!CI.is_valid_posn(line)) {
				cout << "Invalid input, try again" << endl;
				continue;
			}

			Posn posn = CI.interpret_posn(line);
			Bitboard pos = posn.to_bitboard();

			// see if it's replacing an already existing piece
			bs.teams[0].remove_piece(pos)
				|| bs.teams[1].remove_piece(pos);

			Colour c = (piece_char[0] < 'a')
				? Colour::WHITE : Colour::BLACK;


			if (c == Colour::WHITE) {
				if (type == Type::PAWN && posn.y == 8) {
					std::cout << "Can't put pawn on 8th rank"
						<< std::endl;
					continue;
				}
				bs.teams[0].add_piece(
						Piece{pos, type, Colour::WHITE});
			} else {
				if (type == Type::PAWN && posn.y == 1) {
					std::cout << "Can't put pawn on 1st rank"
						<< std::endl;
					continue;
				}
				bs.teams[1].add_piece(
						Piece{pos, type, Colour::BLACK});
			}
			std::cout << bs.print_board();
		} else if (command == "-") {
			if (!CI.is_valid_posn(line)) {
				cout << "Invalid input, try again" << endl;
				continue;
			}

			Posn posn = CI.interpret_posn(line);
			Bitboard pos = posn.to_bitboard();
			// try to remove from white first, then black 
			bs.teams[0].remove_piece(pos)
				|| bs.teams[1].remove_piece(pos);
			std::cout << bs.print_board();
		} else if (command == "=") {
			std::string colour;
			std::string garbage;
			ss >> colour;
			ss >> garbage;
			if (garbage.length() != 0)
				// ensures one command per line
				continue;
			Colour to_play;
			if (colour == "white") {
				cout << "White is set to play first" << endl;
				to_play = Colour::WHITE;
			} else if (colour == "black") {
				cout << "Black is set to play first" << endl;
				to_play = Colour::BLACK;
			} else {
				cout << "Invalid colour" << endl;
				continue;
			}
			bs.set_turn(to_play);
		} else if (command == "done") {
			Bitboard w_king = bs.teams[0].get_king_pos();
			Bitboard b_king = bs.teams[1].get_king_pos();
			if (w_king.count() != 1 || b_king.count() != 1) {
				cout << "Incorrect number of kings" << endl;
				continue;
			}
			// check no pawns on first or last row
			// check neither king is in check
			bs.calc_sudo_legal_moves();
			Bitboard w_atk = bs.teams[0].get_sudo_legal_moves();
			Bitboard b_atk = bs.teams[1].get_sudo_legal_moves();
			if (bs.teams[0].check(b_atk) || bs.teams[1].check(w_atk)) {
				cout << "Can't start in check" << endl;
				continue;
			}
			cout << "Leaving setup mode" << endl;
			break;
		} else {
			std::cout << "Invalid input, try again." << std::endl;
		}
	} // while

	return;
}

int play_game(stack<BoardState> &states, Player &one, Player &two)
{
	string command;
	// game loop
	while(1) {
		BoardState bs = states.top();
		bs.calc_legal_moves();
		// print board
		cout << bs.print_board();

		string to_play = (bs.get_turn() == Colour::WHITE) 
			? "White" : "Black";

		if (bs.checkmate()) {
			string winner = (bs.get_turn() == Colour::WHITE) 
				? "Black" : "White";
			cout << "Checkmate! " << winner << " wins!" << endl;
			if (winner == "White")
				one.score += 1;
			else 
				two.score += 1;
			return 0;
		} else if (bs.stalemate()) {
			cout << "Stalemate!" << endl;
			one.score += 0.5;
			two.score += 0.5;
			return 1;
		} else if (bs.check()) {
			cout << to_play << " is in check." << endl;
		}
		// command loop
		while(1) {
			cout << to_play
				<< "'s turn: move from to or resign" << endl;
			cin >> command;
			
			if (cin.fail())
				return -1;

			if (command == "move") {
				Player &attack = (bs.get_turn() == Colour::WHITE)
					? one : two;
				// get the move
				Move new_move = attack.strat->choose_move(bs);
				if (new_move.promotion == Type::PAWN) {
					cout << "Invalid move, try again" << endl;
					continue;
				}
				BoardState bs_copy = bs;
				bs_copy.set_castling_rights(new_move);
				bs_copy.move(new_move);
				bs_copy.set_enpassant_sqr(new_move);
				bs_copy.toggle_turn();
				states.push(bs_copy);
				break;
			} if (command == "resign") { 
				string winner = (bs.get_turn() == Colour::WHITE)
					? "Black" : "White";
				cout << winner << " wins!" << endl;
				if (winner == "White")
					one.score += 1;
				else 
					two.score += 1;
				return 0;
			} if (command == "undo") {
				if (states.size() > 1) {
					states.pop();
					break;
				}
				cout << "Can't undo" << endl;
			} else {
				cout << "Incorrect input, try again" << endl;
			}
			
		}
	}
	return 0;
}

void default_board(BoardState &bs)
{
	bs.set_turn(Colour::WHITE);
	bs.set_castling_rights("KQkq");

	Team &white = bs.teams[0];
	Team &black = bs.teams[1];
	const int width = 8;
	for (int x = 0; x < width / 2; ++x) {
		white.add_piece(Piece{Posn{x, 1}.to_bitboard(),
				Type::PAWN, Colour::WHITE});
		white.add_piece(Piece{Posn{width - x - 1, 1}.to_bitboard(),
				Type::PAWN, Colour::WHITE});
		// black
		black.add_piece(Piece{Posn{x, width - 2}.to_bitboard(),
				Type::PAWN, Colour::BLACK});
		black.add_piece(Piece{Posn{width - x - 1, width - 2}.to_bitboard(),
				Type::PAWN, Colour::BLACK});
		if (x == width / 2 - 1) {
			white.add_piece(Piece{Posn{x, 0}.to_bitboard(),
					Type::QUEEN, Colour::WHITE});
			white.add_piece(Piece{Posn{width - x - 1, 0}.to_bitboard(),
					Type::KING, Colour::WHITE});
			// black
			black.add_piece(Piece{Posn{x, width - 1}.to_bitboard(),
					Type::QUEEN, Colour::BLACK});
			black.add_piece(Piece{Posn{width - x - 1, width - 1}.to_bitboard(),
					Type::KING, Colour::BLACK});
		} else {
			white.add_piece(Piece{Posn{x, 0}.to_bitboard(),
					(Type)(2 + x), Colour::WHITE});
			white.add_piece(Piece{Posn{width - x - 1, 0}.to_bitboard(),
					(Type)(2 + x), Colour::WHITE});
			// black
			black.add_piece(Piece{Posn{x, width - 1}.to_bitboard(),
					(Type)(2 + x), Colour::BLACK});
			black.add_piece(Piece{Posn{width - x - 1, width - 1}.to_bitboard(),
					(Type)(2 + x), Colour::BLACK});
		}

	}
}
