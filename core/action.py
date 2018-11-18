from enum import Enum


class ActionType(Enum):
    MOVE = 0
    BLOCK = 1


def create_move(new_pos):
    return Action(new_pos, None, None, ActionType.MOVE)


def create_block(block_pos, block_orientation):
    return Action(None, block_pos, block_orientation, ActionType.BLOCK)


class Action:

    def __init__(self, new_pos, block_pos, block_orientation, type):
        self.new_pos = new_pos
        self.block_pos = block_pos
        self.block_orientation = block_orientation
        self.type = type
