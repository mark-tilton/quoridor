import queue
import validation as validation


class Player:

    def __init__(self, goal_row, starting_pos):
        self.goal_row = goal_row
        self.opp_goal_row = 8 if goal_row == 0 else 0
        self.starting_pos = starting_pos
        self.reset()

    def take_action(self, board_state):
        pass

    def get_valid_move_positions(self, board_state):
        return validation.get_valid_move_positions(self.pos, board_state)

    def reset(self):
        self.pos = self.starting_pos
        self.wall_count = 10
