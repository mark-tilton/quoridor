from enum import Enum

class ActionType(Enum):
    MOVE = 0
    BLOCK = 1

def create_move(new_pos):
    return Action(new_pos, None, None)

def create_block(block_pos, block_orientation):
    return Action(None, block_pos, block_orientation)

class Action:
    
    def __init__(self, new_pos, block_pos, block_orientation):
        self.new_pos = new_pos
        self.block_pos = block_pos
        self.block_orientation = block_orientation
        