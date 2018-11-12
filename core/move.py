from core.action import ActionType

class Move:

    def __init__(self, board_state, pos, action):
        self.board_state = board_state
        self.action = action
        self.pos = pos

    
    def get_board_state(self):
        new_board = self.board_state.clone()
        if self.action.type == ActionType.MOVE:
            new_board.cells[self.pos[0]][self.pos[1]] = 0
            new_board.cells[self.action.new_pos[0]][self.action.new_pos[1]] = 1
        elif self.action.type == ActionType.BLOCK:
            new_board.walls[self.action.block_pos[0]][self.action.block_pos[1]] = self.action.block_orientation
        return new_board
