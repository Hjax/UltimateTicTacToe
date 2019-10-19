#include <chrono>
#include <array>
#include <ctime>
#include <iostream>
#include <vector>

std::array<unsigned short,9> ALL_MOVES = {1, 2, 4, 8, 16, 32, 64, 128, 256};
std::array<unsigned short,8> ALL_WINS  = {7, 56, 448, 292, 146, 73, 273, 84};

unsigned short FULL_BOARD = 511;
unsigned short BOARD_MASK = 240;
unsigned short SQUARE_MASK = 15;


class board {
	unsigned short int small[2][9] = {};
	unsigned short int large[2] = {};
	std::vector<unsigned short> moves;
	bool side = false;

	bool draw = false;
	bool x_wins = false;
	bool o_wins = false;

	// The bitmap (one value true) of the last move
	unsigned short last_move = 999;
	
public:

	void make(unsigned short move) {
		unsigned short board  = move & BOARD_MASK;
		unsigned short square = move & SQUARE_MASK;


		small[side][board] += ALL_MOVES[square];
		if (is_won(small[side][board])) {
			large[side] += ALL_MOVES[board];
		}
		side = !side;
	}

	static bool is_won(unsigned short board) {
		for (int i = 0; i < ALL_WINS.size(); i++) {
			if ((board & ALL_WINS[i]) == ALL_WINS[i]) {
				return true;
			}
		}
		return false;
	}

	static bool is_drawn(unsigned short board1, unsigned short board2) {
		for (int i = 0; i < ALL_WINS.size(); i++) {
			if ((board1 & ALL_WINS[i]) == 0 || (board2 & ALL_WINS[i]) == 0) {
				return false;
			}
		}
		return true;
	}

	bool any_board() {

		if (last_move != 999) {
			return (large[0] & large[1] & last_move) > 0 || (large[0] & large[1]) == FULL_BOARD;
		}

		return true;

	}

	static unsigned short move_to_char(unsigned short board, unsigned short square) {
		return (board << 4) + square;
	}

	void movegen() {
		if (any_board()) {
			for (int i = 0; i < 9; i++) {
				if (!is_won(small[0][i]) && !is_won(small[1][i]) && !is_drawn(small[0][i], small[1][i])) {
					for (int m = 0; m < 9; m++) {
						if ((small[0][i] & small[1][i] & ALL_MOVES[m]) == 0) {
							moves.emplace_back(move_to_char(i, m));
						}
					}
				}
			}
		}
		else {
			for (int m = 0; m < 9; m++) {
				if ((small[0][last_move] & small[1][last_move] & ALL_MOVES[m]) == 0) {
					moves.emplace_back(move_to_char(last_move, m));
				}
			}
		}
	}

	static int rollout(board b) {

	}

	void print() {
		for (int row = 0; row < 9; row++) {
			for (int board = 0; board < 3; board++) {
				for (int square = 0; square < 3; square++) {
					int board_offset = board + (row / 3) * 3;
					int square_offset = square + (3 * (row % 3));
					int selected;
					if (square_offset == 0) {
						selected = 1;
					}
					else {
						selected = 2 << square_offset - 1;
					}
					if (is_won(small[0][board_offset])) {
						std::cout << "X";
					}  
					else if (is_won(small[1][board_offset])) {
						std::cout << "O";
					}
					else if ((small[0][board_offset] & selected) > 0) {
						std::cout << "X";
					}
					else if ((small[1][board_offset] & selected) > 0) {
						std::cout << "O";
					}
					else {
						std::cout << "-";
					}
					std::cout << "|";
				}
			}
			if (row == 2 || row == 5) {
				std::cout << "\n__________________";
			}
			std::cout << "\n";
		}
	}
};

int main() {

	board b;
	
	b.movegen();

	std::cout << "done";


}

