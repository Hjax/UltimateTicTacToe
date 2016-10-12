import time, random


# type out the 8 winning sequences since there isn't a great way to generate them
win_conditions = [0b000000111, 0b000111000, 0b111000000,
                  0b100100100, 0b010010010, 0b001001001,
                  0b100010001, 0b001010100]
# generate the legal moves (0b100000000 is 2^8th)
moves = [2**x for x in range(0,9)]
class board:
    def __init__(self):
        # generate two sets of 9 bitboards, indexed by side to move + 1 / 2
        self.minibitboards =  [[0b000000000 for x in range(2)] for x in range(9)]
        # two bit boards for who has won varying positions on the board
        self.largebitboards = [0b000000000 for x in range(0, 2)]
        # two bitboards for boards that can not be won by a given side
        self.drawbitboards =  [0b000000000 for x in range(0, 2)]
        # side to move
        self.side_to_move = 1
    # the move format is (index into minibitboards, move)
    # these methods need to update the large bitboards as well
    def make(self, board, move):
        self.minibitboards[board][self.side_to_move + 1 / 2] |= move
        self.side_to_move *= -1
    def unmake(self, board, move):
        self.minibitboards[board][self.side_to_move + 1 / 2] ^= move
        self.side_to_move *= -1
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
        
def speed_test():
	start = time.time()
	count = 0
	while time.time() - start < 2:
		board = random.choice(range(0, 9))
		cell = random.choice(range(0, 9))
		foo.make(board, moves[cell])
		foo.is_winnable(foo.minibitboards[board][foo.side_to_move + 1 / 2])
		foo.is_won(foo.minibitboards[board][foo.side_to_move + 1 / 2])
		foo.unmake(board, moves[cell])
		count += 1
	print(count)
foo = board()
speed_test()