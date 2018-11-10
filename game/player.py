import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from core.board_state import BoardState

class Player:

    def __init__(self):
        self.wall_count = 10

    def take_action(board_state):
        return Action()