import numpy as np
import random as r
import queue
from core.action import *
from player import Player

#todo: Implement wall placement logic
#todo: Account for jumping when finding shortest path
class ShortestPathPlayer(Player):


    def take_action(self, board_state):
        if r.randint(0, 1) == 0 and self.wall_count > 0:
            # Block opponents shortest path
            moves = self.get_shortest_path_to_y_level(board_state, 8)
            return create_block(np.array([0, 0]), np.array([0, 0]))
        else:
            # Move along shortest path
            moves = self.get_shortest_path_to_y_level(board_state, 0)
            return create_move(moves[0])


    def get_shortest_path_to_y_level(self, board_state, y_level):
        # Get distance matrix
        distance_matrix = board_state.get_distance_matrix(self.pos)
        # Find closest cell on desired y level
        closest_cell_distance = None
        closest_cell = None
        for x in range(9):
            distance = distance_matrix[x][y_level]
            if closest_cell_distance is None or distance < closest_cell_distance:
                closest_cell = np.array([x, y_level])
                closest_cell_distance = distance
        # Find path to closest cell
        moves = []
        moves.append(closest_cell)
        if closest_cell_distance == 1:
            return moves
        path_found = False
        pos = closest_cell
        while not path_found:
            for cell in self._get_accessible_adjacent_cells(board_state, pos):
                distance = distance_matrix[cell[0], cell[1]]
                if distance == distance_matrix[pos[0], pos[1]] - 1:
                    moves.append(cell)
                    pos = cell
                    if distance == 1:
                        path_found = True
                        break
        # Reverse order of moves so they are in the order that they would need to be made
        moves.reverse()
        return moves

    
    def _get_accessible_adjacent_cells(self, board_state, pos):
        directions = [np.array([1, 0]), np.array([-1, 0]), np.array([0, 1]), np.array([0, -1])]
        for direction in directions:
            new_pos = pos + direction
            cell_is_valid = board_state.is_cell_index_in_bounds(new_pos)
            cell_is_blocked = cell_is_valid and board_state.is_path_blocked(pos, direction)
            if cell_is_valid and not cell_is_blocked:
                yield new_pos
