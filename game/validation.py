from core.action import ActionType
from core.board_state import BoardState
from core.vector2 import Vector2


def validate_move(player, opponent, move):
    board_state = move.board_state
    action = move.action

    if action is None:
        return False

    if action.type == ActionType.MOVE:
        # check if move to a valid location
        valid_moves = player.get_valid_move_positions(board_state)
        if not any(action.new_pos.x == pos.x and action.new_pos.y == pos.y for pos in valid_moves):
            return False
    else:
        # player has enough walls
        if player.wall_count == 0:
            return False
        # wall is within bounds
        if not board_state.is_wall_index_in_bounds(action.block_pos):
            return False
        # wall is not on top of another wall
        if board_state.walls[action.block_pos.x, action.block_pos.y] != 0:
            return False
        # wall is not directly next to another wall of the same orientation
        shift_amt = Vector2(1, 0) if action.block_orientation == 2 else Vector2(0, 1)
        adjacent_point_1 = action.block_pos - shift_amt
        adjacent_point_2 = action.block_pos + shift_amt
        if board_state.is_wall_index_in_bounds(adjacent_point_1) and board_state.walls[adjacent_point_1.x, adjacent_point_1.y] == action.block_orientation:
            return False
        if board_state.is_wall_index_in_bounds(adjacent_point_2) and board_state.walls[adjacent_point_2.x, adjacent_point_2.y] == action.block_orientation:
            return False
        # player is not boxed in
        clone = board_state.clone()
        clone.walls[action.block_pos.x, action.block_pos.y] = action.block_orientation
        player_distance_matrix = clone.get_distance_matrix_from_row(player.goal_row)
        if player_distance_matrix[player.pos.x][player.pos.y] is None:
            return False
        opponent_distance_matrix = clone.get_distance_matrix_from_row(opponent.goal_row)
        if opponent_distance_matrix[opponent.pos.x][opponent.pos.y] is None:
            return False
    return True


def get_valid_move_positions(pos, board_state):
    directions = [Vector2(1, 0), Vector2(-1, 0), Vector2(0, 1), Vector2(0, -1)]
    for direction in directions:
        new_pos = pos + direction
        cell_is_valid = board_state.is_cell_index_in_bounds(new_pos)
        cell_is_blocked = cell_is_valid and board_state.is_path_blocked(pos, direction)
        cell_is_occupied = cell_is_valid and board_state.cells[new_pos.x, new_pos.y] != 0
        if cell_is_valid and not cell_is_blocked:
            if not cell_is_occupied:
                yield new_pos
            else:
                for jump_direction in directions:
                    jump_pos = new_pos + jump_direction
                    jump_cell_is_valid = board_state.is_cell_index_in_bounds(jump_pos)
                    jump_cell_is_blocked = jump_cell_is_valid and board_state.is_path_blocked(new_pos, jump_direction)
                    jump_cell_is_occupied = jump_cell_is_valid and board_state.cells[jump_pos.x, jump_pos.y] != 0
                    if jump_cell_is_valid and not jump_cell_is_blocked and not jump_cell_is_occupied:
                        yield jump_pos
