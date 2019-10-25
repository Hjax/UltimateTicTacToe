#include <chrono>
#include <array>
#include <ctime>
#include <iostream>
#include <vector>

std::array<unsigned short,9> ALL_MOVES = {1, 2, 4, 8, 16, 32, 64, 128, 256};
std::array<unsigned short,8> ALL_WINS  = {7, 56, 448, 292, 146, 73, 273, 84};

std::array<bool, 512> ALL_WON_BOARDS;

unsigned short FULL_BOARD = 511;
unsigned short BOARD_MASK = 240;
unsigned short SQUARE_MASK = 15;

unsigned short DRAW = 0;
unsigned short XWIN = 1;
unsigned short YWIN = 2;
unsigned short NO_RESULT = 3;

bool X = 0;
bool O = 1;

static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

class board {


public:

	unsigned short int small[2][9] = {};
	unsigned short int large[2] = {};


	unsigned short int moves[81] = {};
	unsigned short int move_count = 0;


	bool side = false;

	bool draw = false;
	bool x_wins = false;
	bool o_wins = false;

	int rollouts = 0;
	int wins = 0;

	// The move index of the last move
	unsigned short last_move = 999;

	void make(unsigned short move) {

		unsigned short board  = (move & BOARD_MASK) >> 4;
		unsigned short square = move & SQUARE_MASK;

		std::cout << "making ";
		std::cout << board;
		std::cout << " ";
		std::cout << square;
		std::cout << "\n";

		last_move = square;

		small[side][board] |= ALL_MOVES[square];
		if (is_won(small[side][board])) {
			large[side] |= ALL_MOVES[board];
		}
		side = !side;

		print();
	}

	static bool is_won(unsigned short board) {
		return ALL_WON_BOARDS[board];
	}

	static bool is_drawn(unsigned short board1, unsigned short board2) {
		for (int i = 0; i < 8; i++) {
			if ((board1 & ALL_WINS[i]) == 0 || (board2 & ALL_WINS[i]) == 0) {
				return false;
			}
		}
		return true;
	}

	bool any_board() {

		if (last_move != 999) {
			return (large[0] & large[1] & ALL_MOVES[last_move]) > 0 || (large[0] & large[1]) == FULL_BOARD;
		}

		return true;

	}

	int score() {
		if (is_won(large[X])) {
			return XWIN;
		}
		else if (is_won(large[O])) {
			return YWIN;
		}
		else if (is_drawn(large[X], large[O])) {
			return DRAW;
		}
		return NO_RESULT;
	}

	static unsigned short move_to_char(unsigned short board, unsigned short square) {
		return (board << 4) + square;
	}

	void movegen() {
		move_count = 0;
		if (any_board()) {
			for (int i = 0; i < 9; i++) {
				if (!is_won(small[0][i]) && !is_won(small[1][i]) && !is_drawn(small[0][i], small[1][i])) {
					for (int m = 0; m < 9; m++) {
						if (((small[0][i] | small[1][i]) & ALL_MOVES[m]) == 0) {
							moves[move_count++] = move_to_char(i, m);
						}
					}
				}
			}
		}
		else {
			for (int m = 0; m < 9; m++) {
				if (((small[0][last_move] | small[1][last_move]) & ALL_MOVES[m]) == 0) {
					moves[move_count++] = move_to_char(last_move, m);
				}
			}
		}
	}

	void make_random_move() {
		if (move_count == 0) {
			movegen();
		}
		if (move_count > 0) {
			make(moves[xorshf96() % move_count]);
			move_count = 0;
		}
	}

	static int rollout(board b) {
		
		while (b.score() == NO_RESULT) {
			b.make_random_move();
		}

		return b.score();
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
						selected = 2 << (square_offset - 1);
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
			std::cout << "\n";
		}
		std::cout << "\n";
	}
};

int main() {

	for (int b = 0; b < 512; b++) {
		ALL_WON_BOARDS[b] = false;
		for (int i = 0; i < ALL_WINS.size(); i++) {
			if ((b & ALL_WINS[i]) == ALL_WINS[i]) {
				ALL_WON_BOARDS[b] = true;
				break;
			}
		}
	}

	board b;

	auto begin = std::chrono::high_resolution_clock::now();
	
	//b.rollout(b);

	for (int i = 0; i < 100000; i++) {

		b.rollout(b);

	}

	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - begin;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

	std::cout << ms;

}

