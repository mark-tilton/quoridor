import random as r
from core.action import *
from core.vector2 import Vector2
from player import Player


class RandomPlayer(Player):

    def take_action(self, opponent, board_state):
        if r.randint(0, 3) > 0:
            valid_moves = list(self.get_valid_move_positions(board_state))
            new_pos = valid_moves[r.randint(0, len(valid_moves) - 1)]
            return create_move(new_pos)
        else:
            wall_pos = Vector2(r.randint(0, 7), r.randint(0, 7))
            wall_orientation = 1 if r.randint(0, 1) == 0 else 2
            return create_block(wall_pos, wall_orientation)
