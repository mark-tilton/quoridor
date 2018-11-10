import numpy as np
from core.board_state import BoardState
from core.action import Action

class Player:

    def __init__(self, pos = np.array([5, 9]), wall_count = 10):
        self.wall_count = wall_count 
        self.pos = pos


    def take_action(self, board_state):
        pass