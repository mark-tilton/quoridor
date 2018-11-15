import numpy as np
import queue
import validation as validation


class Player:

    def __init__(self):
        self.reset()


    def take_action(self, board_state):
        pass


    def get_valid_move_positions(self, board_state):
        return validation.get_valid_move_positions(self.pos, board_state)


    def get_distance_matrix(self, board_state):
        matrix = np.full((9, 9), None)
        matrix[self.pos[0], self.pos[1]] = 0
        q = queue.Queue()
        q.put(self.pos)
        while not q.empty():
            cell_pos = q.get()
            distance = matrix[cell_pos[0], cell_pos[1]]
            for cell in validation.get_valid_move_positions(cell_pos, board_state):
                if matrix[cell[0], cell[1]] is None:
                    matrix[cell[0], cell[1]] = distance + 1
                    q.put(cell)
        return matrix


    def reset(self):
        self.pos = np.array([4, 8])
        self.wall_count = 10
