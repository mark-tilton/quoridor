import numpy as np
import random as r
import queue
from core.action import *
from player import Player

#todo: Implement wall placement logic
class ShortestPathPlayer(Player):


    def take_action(self, board_state):
        if r.randint(0, 1) == 0 and self.wall_count == 0:
            # Block opponents shortest path
            moves = self.get_shortest_path_to_y_level(board_state, 8)
            return create_block(np.array([0, 0]), np.array([0, 0]))
        else:
            # Move along shortest path
            target = self.get_closest_pos_at_y_level(board_state, 0)
            pos = self.get_best_move_towards_pos(board_state, target)
            return create_move(pos)


    def get_closest_pos_at_y_level(self, board_state, y_level):
        # Get distance matrix
        distance_matrix = self.get_distance_matrix(board_state)
        # Find closest cell on desired y level
        closest_cell_distance = None
        closest_cell = None
        for x in range(9):
            distance = distance_matrix[x][y_level]
            if closest_cell_distance is None or distance is not None and distance < closest_cell_distance:
                closest_cell = np.array([x, y_level])
                closest_cell_distance = distance
        return closest_cell


    def get_best_move_towards_pos(self, board_state, target) :
        # Find move that is closest to the closest cell
        distance_matrix = board_state.get_distance_matrix(target)
        best_distance = None
        best_move = None
        for pos in self.get_valid_move_positions(board_state):
            distance = distance_matrix[pos[0], pos[1]]
            if best_distance is None or distance < best_distance:
                best_distance = distance
                best_move = pos
        return best_move

    
    def _get_accessible_adjacent_cells(self, board_state, pos):
        directions = [np.array([1, 0]), np.array([-1, 0]), np.array([0, 1]), np.array([0, -1])]
        for direction in directions:
            new_pos = pos + direction
            cell_is_valid = board_state.is_cell_index_in_bounds(new_pos)
            cell_is_blocked = cell_is_valid and board_state.is_path_blocked(pos, direction)
            if cell_is_valid and not cell_is_blocked:
                yield new_pos
