from core.action import ActionType

def validate_move(move):
    if move.action.type == ActionType.MOVE:
        if not move.board_state.is_cell_index_in_bounds(move.action.new_pos):
            return False
    return True

