import numpy as np
import random as r
from core.action import *
from player import Player

class RandomPlayer(Player):

    def take_action(self, board_state):
        if r.randint(0, 3) > 0:
            direction = r.randint(0, 3);
            if direction == 0:
                move = np.array([0, 1])
            if direction == 1:
                move = np.array([1, 0])
            if direction == 2:
                move = np.array([0, -1])
            if direction == 3:
                move = np.array([-1, 0])
            return create_move(self.pos + move)
        else:
            wall_pos = np.array([r.randint(0, 7), r.randint(0, 7)])
            wall_orientation = 1 if r.randint(0, 1) == 0 else 2
            return create_block(wall_pos, wall_orientation)
