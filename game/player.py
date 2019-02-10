import queue
import validation as validation
from core.vector2 import Vector2


class Player:

    def __init__(self, index):
        self.id = -1
        self.index = index
        self.opp_index = 1 if index == 0 else 0
        self.goal_row = 8 if index == 0 else 0
        self.opp_goal_row = 0 if index == 0 else 8
        self.starting_pos = Vector2(4, 0) if index == 0 else Vector2(4, 8)
        self.reset()

    def take_action(self, board_state):
        pass

    def get_valid_move_positions(self, board_state):
        return validation.get_valid_move_positions(self.pos, board_state)

    def reset(self):
        self.pos = self.starting_pos
        self.wall_count = 10
