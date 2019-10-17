#include <chrono>
#include <array>
#include <ctime>
#include <iostream>

std::array<int,9> moves = {1, 2, 4, 8, 16, 32, 64, 128, 256};
std::array<int,8> wins  = {7, 56, 448, 292, 146, 73, 273, 84};

class board {
	int small[9][2] = {};
	int large[2] = {};
	int side = 1;
	
public:

	void make(int board, int move) {
		int side_index = (side + 1) / 2;
		small[board][side_index] += move;
		if (is_won(small[board][side_index])) {
			large[side_index] += moves[board];
		}
		side *= -1;
	}

	void unmake(int board, int move) {
		side *= -1;
		int side_index = (side + 1) / 2;
		small[board][side_index] -= move;
		if (!is_won(small[board][side_index])) {
			if ((large[side_index] & moves[board]) == moves[board]) {
				large[side_index] -= moves[board];
			}
		}
	}

	bool is_won(int board) {
		for (int i = 0; i < wins.size(); i++) {
			if ((board & wins[i]) == wins[i]) {
				return true;
			}
		}
		return false;
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
					if ((small[board_offset][0] & selected) > 0) {
						std::cout << "X";
					}
					else if ((small[board_offset][1] & selected) > 0) {
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
	
	b.make(3, moves[3]);

	b.print();
}

