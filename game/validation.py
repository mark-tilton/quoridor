from core.action import ActionType
from core.board_state import BoardState
import numpy as np


def validate_move(player, opponent, move):
    board_state = move.board_state
    action = move.action

    if action is None:
        return False

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
        player_distance_matrix = clone.get_distance_matrix_from_row(0)
        if player_distance_matrix[player.pos[0]][player.pos[1]] is None:
            return False
        flipped_opp_pos = BoardState.flip_cell_position(opponent.pos)
        opponent_distance_matrix = clone.get_distance_matrix_from_row(8)
        if opponent_distance_matrix[flipped_opp_pos[0]][flipped_opp_pos[1]] is None:
            return False
    return True


def get_valid_move_positions(pos, board_state):
    directions = [np.array([1, 0]), np.array([-1, 0]), np.array([0, 1]), np.array([0, -1])]
    for direction in directions:
        new_pos = pos + direction
        cell_is_valid = board_state.is_cell_index_in_bounds(new_pos)
        cell_is_blocked = cell_is_valid and board_state.is_path_blocked(pos, direction)
        cell_is_occupied = cell_is_valid and board_state.cells[new_pos[0], new_pos[1]] != 0
        if cell_is_valid and not cell_is_blocked:
            if not cell_is_occupied:
                yield new_pos
            else:
                for jump_direction in directions:
                    jump_pos = new_pos + jump_direction
                    jump_cell_is_valid = board_state.is_cell_index_in_bounds(jump_pos)
                    jump_cell_is_blocked = jump_cell_is_valid and board_state.is_path_blocked(new_pos, jump_direction)
                    jump_cell_is_occupied = jump_cell_is_valid and board_state.cells[jump_pos[0], jump_pos[1]] != 0
                    if jump_cell_is_valid and not jump_cell_is_blocked and not jump_cell_is_occupied:
                        yield jump_pos
