import numpy as np
import random as r
import queue
from core.action import *
from player import Player

#todo: Implement blocking logic
class ShortestPathPlayer(Player):


    def take_action(self, board_state):
        if r.randint(0, 1) == 0 and self.wall_count == 0:
            # Block along opponent's shortest path
            pass
        else:
            # Move along shortest path
            distance_matrix = board_state.get_distance_matrix_from_top_row()
            best_distance = None
            best_move = None
            for pos in self.get_valid_move_positions(board_state):
                distance = distance_matrix[pos[0], pos[1]]
                if best_distance is None or distance < best_distance:
                    best_distance = distance
                    best_move = pos
            return create_move(best_move)
