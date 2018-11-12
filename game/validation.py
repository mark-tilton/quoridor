from core.action import ActionType
import numpy as np

def validate_move(player, move):
    board_state = move.board_state
    action = move.action

    if action.type == ActionType.MOVE:
        # check if move to a valid location
        valid_moves = player.get_valid_move_positions(board_state)
        if not any(action.new_pos[0] == pos[0] and action.new_pos[1] == pos[1] for pos in valid_moves):
            return False
    else:
        # player has enough walls
        if player.wall_count == 0:
            return False
        # wall is within bounds
        if not board_state.is_wall_index_in_bounds(action.block_pos):
            return False
        # wall is not on top of another wall
        if board_state.walls[action.block_pos[0], action.block_pos[1]] != 0:
            return False        
        # wall is not directly next to another wall of the same orientation
        shift_amt = np.array([1, 0]) if action.block_orientation == 2 else np.array([0, 1])
        adjacent_point_1 = action.block_pos - shift_amt
        adjacent_point_2 = action.block_pos + shift_amt
        if board_state.is_wall_index_in_bounds(adjacent_point_1) and board_state.walls[adjacent_point_1[0], adjacent_point_1[1]] == action.block_orientation:
            return False
        if board_state.is_wall_index_in_bounds(adjacent_point_2) and board_state.walls[adjacent_point_2[0], adjacent_point_2[1]] == action.block_orientation:
            return False
        # player is not boxed in    
        clone = board_state.clone()
        clone.walls[action.block_pos[0], action.block_pos[1]] = action.block_orientation
        distance_matrix = clone.get_distance_matrix(player.pos)
        if all(distance_matrix[i][0] is None for i in range(9)):
            return False
    return True
