import numpy as np
import queue
from core.board_state import BoardState
from core.action import Action

class Player:

    def __init__(self, pos = np.array([4, 8]), wall_count = 10):
        self.wall_count = wall_count 
        self.pos = pos


    def take_action(self, board_state):
        pass


    def get_valid_move_positions(self, board_state):
        return self._get_valid_move_positions(self.pos, board_state)


    def _get_valid_move_positions(self, pos, board_state):
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
                        jump_cell_is_blocked = jump_cell_is_valid and board_state.is_path_blocked(new_pos, direction)
                        jump_cell_is_occupied = jump_cell_is_valid and board_state.cells[jump_pos[0], jump_pos[1]] != 0
                        if jump_cell_is_valid and not jump_cell_is_blocked and not jump_cell_is_occupied:
                            yield jump_pos



    def get_distance_matrix(self, board_state):
        matrix = np.full((9, 9), None)
        matrix[self.pos[0], self.pos[1]] = 0
        q = queue.Queue()
        q.put(self.pos)
        while not q.empty():
            cell_pos = q.get()
            distance = matrix[cell_pos[0], cell_pos[1]]
            for cell in self._get_valid_move_positions(cell_pos, board_state):
                if matrix[cell[0], cell[1]] is None:
                    matrix[cell[0], cell[1]] = distance + 1
                    q.put(cell)
        return matrix
