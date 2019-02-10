import random as r
import queue
from core.action import *
from core.move import Move
from core.board_state import BoardState
from player import Player
import validation as validation


class ShortestPathPlayer(Player):

    def take_action(self, opponent, board_state):
        if r.randint(0, 1) == 0 and self.wall_count > 0:
            # Block along opponent's shortest path
            # Calculate board state and opponents position (from our perspective)
            opp_dist = board_state.get_distance_matrix_from_row(self.opp_goal_row)
            opp_pos = opponent.pos
            # Play the game forward from the opponents perspective, just moving.
            for i in range(1):
                opp_action = self.get_best_move(opp_pos, board_state, opp_dist)
                opp_move = Move(board_state, self.opp_index, opp_action)
                board_state = opp_move.get_board_state()
                last_pos = opp_pos
                opp_pos = opp_action.new_pos
            # Block the path from the last position to the latest position
            direction = opp_pos - last_pos
            orientation = 1 if direction.y == 0 else 2
            points = BoardState.get_wall_points(last_pos, direction)
            for point in points:
                if (board_state.is_wall_index_in_bounds(point) and board_state.walls[point.x, point.y] == 0):
                    return create_block(point, orientation)

        # Move along shortest path
        distance_matrix = board_state.get_distance_matrix_from_row(self.goal_row)
        board_state.debug_matrix = distance_matrix
        return self.get_best_move(self.pos, board_state, distance_matrix)

    @staticmethod
    def get_best_move(pos, board_state, distance_matrix):
        best_distance = None
        best_move = None
        for pos in validation.get_valid_move_positions(pos, board_state):
            distance = distance_matrix[pos.x, pos.y]
            if best_distance is None or distance < best_distance:
                best_distance = distance
                best_move = pos
        return create_move(best_move)
