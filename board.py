import time, random

from profilehooks import profile

# type out the 8 winning sequences since there isn't a great way to generate them
win_conditions = [0b000000111, 0b000111000, 0b111000000,
                  0b100100100, 0b010010010, 0b001001001,
                  0b100010001, 0b001010100]

# generate the legal moves (0b100000000 is 2^8th)
moves = list(reversed([2**x for x in range(0,9)]))

class board:
    def __init__(self):
        # generate two sets of 9 bitboards, indexed by side to move + 1 / 2
        self.minibitboards =  [[0b000000000 for x in range(2)] for x in range(9)]
        # two bit boards for who has won varying positions on the board
        self.largebitboards = [0b000000000 for x in range(0, 2)]
        # side to move
        self.side_to_move = 1
        
    # the move format is (index into minibitboards, move)
    # these methods need to update the large bitboards as well
    def make(self, board, move):
        side_index = (self.side_to_move + 1) // 2

        # new
        current = self.minibitboards[board][side_index]
        current += move

        # old
        # self.minibitboards[board][side_index] += move
        # current = self.minibitboards[board][side_index]

        if self.is_won(current):
            self.largebitboards[side_index] += moves[board]
        self.side_to_move *= -1

    def unmake(self, board, move):
        self.side_to_move *= -1
        side_index = (self.side_to_move + 1) // 2

        # new
        current = self.minibitboards[board][side_index]
        current -= move

        # old
        # self.minibitboards[board][side_index] -= move
        # current = self.minibitboards[board][side_index]

        if not self.is_won(current):
            if self.largebitboards[side_index] & moves[board] == moves[board]:
                self.largebitboards[side_index] -= moves[board]
                
    # checks if a bitboard is still winnable for a player
    # this takes the bitboard of the OPPOSING player, and checks
    # if theres three blanks in a row in their bitboard
    def is_winnable(self, minibitboard):
        for cond in win_conditions:
            if minibitboard & cond == 0:
                return True
        return False
    
    # checks if a board is won for a given player
    # takes the bitboard of the SAME player
    def is_won(self, minibitboard):
        for cond in win_conditions:
            if minibitboard & cond == cond:
                return True
        return False

    def score(self):
        # positive scores are good for the side that goes first (side to move == 1, second index)
        for bbb in range(2):
            if self.is_won(self.largebitboards[bbb]):
                return 10000 * ((bbb * 2) - 1)

        return 0

    def print_board(self):
        # for each row of the output
        for i in range(9):
            # for each board in the row
            for k in range(0, 3):
                first = bin(self.minibitboards[k + 3 * (i // 3)][0])[2:].zfill(9)[i % 3 * 3:3 + i % 3 * 3]
                second = bin(self.minibitboards[k + 3 * (i // 3)][1])[2:].zfill(9)[i % 3 * 3:3 + i % 3 * 3]
                # for each space in the board
                for j in range(0, 3):
                    if first[j] == "1":
                        print("X",)
                    elif second[j] == "1":
                        print("O",)
                    else:
                        print("-",)
                if k != 2:
                    print("|",)
            if i == 2 or i == 5:
                print("")
                print("______________________",)
            print("")

def speed_test():
	start = time.time()
	count = 0
	board = random.randint(0, 8)
	cell = random.randint(0, 8)
	while count < 10000000:
		foo.make(board, moves[cell])
		foo.unmake(board, moves[cell])
		count += 1
	print (time.time() - start)
foo = board()
#foo.make(4, moves[5])
#foo.print_board()
speed_test()

