from core.action import ActionType


class Move:

    def __init__(self, board_state, player_index, action):
        self.board_state = board_state
        self.player_index = player_index
        self.action = action

    def get_board_state(self):
        new_board = self.board_state.clone()
        if self.action.type == ActionType.MOVE:
            new_board.player_positions[self.player_index] = self.action.new_pos
        elif self.action.type == ActionType.BLOCK:
            new_board.walls[self.action.block_pos.x][self.action.block_pos.y] = self.action.block_orientation
        return new_board
