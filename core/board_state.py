import numpy as np
import queue

class BoardState:
    
    def __init__(self, cells = np.zeros((9, 9), np.bool), walls = np.zeros((8, 8), np.int8)):
        self.cells = cells
        self.walls = walls


    def __repr__(self):
        output = ''
        for y in range(19): # 9 cells + 8 inner walls + 2 outer walls
            for x in range(19):
                cell_char = ' '
                is_vert_wall = x % 2 == 0
                is_hori_wall = y % 2 == 0
                if y == 0 or y == 18:
                    cell_char = '―'
                elif x == 0 or x == 18:
                    cell_char = '|'
                elif not is_vert_wall and not is_hori_wall: # is this a cell?
                    cell_x = int((x - 1) / 2)
                    cell_y = int((y - 1) / 2)
                    if self.cells[cell_x][cell_y] == 1:
                        cell_char = 'P'
                elif is_vert_wall and is_hori_wall: # Corners
                    cell_char = '·'
                elif is_vert_wall:
                    wall_pos_x = (x - 2) / 2
                    wall_pos_y = (y - 1) / 2
                    if self.is_path_blocked(np.array([wall_pos_x, wall_pos_y]), np.array([1, 0])):
                        cell_char = '|'
                elif is_hori_wall:
                    wall_pos_x = (x - 1) / 2
                    wall_pos_y = (y - 2) / 2
                    if self.is_path_blocked(np.array([wall_pos_x, wall_pos_y]), np.array([0, 1])):
                        cell_char = '―'
                output += cell_char
            output += '\n'
        return output


    def clone(self):
        return BoardState(np.copy(self.cells), np.copy(self.walls))


    def switch(self):
        return BoardState(
            np.rot90(self.cells, 2), 
            np.rot90(self.walls, 2))


    def is_path_blocked(self, cell, dir):
        orientation = 1 if dir[1] == 0 else 2
        center = cell - np.array([0.5, 0.5])
        cdir = dir / 2
        perp = np.array([cdir[1], cdir[0]])
        wall = center + cdir
        point1 = (wall + perp).astype(int)
        point2 = (wall - perp).astype(int)
        blocked = False
        if self.is_wall_index_in_bounds(point1):
            blocked |= self.walls[point1[0], point1[1]] == orientation
        if self.is_wall_index_in_bounds(point2):
            blocked |= self.walls[point2[0], point2[1]] == orientation
        return blocked


    def is_wall_index_in_bounds(self, i):
        return i[0] >= 0 and i[1] >= 0 and i[0] < 8 and i[1] < 8


    def is_cell_index_in_bounds(self, i):
        return i[0] >= 0 and i[1] >= 0 and i[0] < 9 and i[1] < 9

    
    def get_distance_matrix(self, pos):
        matrix = np.full((9, 9), None)
        matrix[pos[0], pos[1]] = 0
        q = queue.Queue()
        q.put(pos)
        while not q.empty():
            cell_pos = q.get()
            distance = matrix[cell_pos[0], cell_pos[1]]
            for cell in self._get_accessible_adjacent_cells(cell_pos):
                if matrix[cell[0], cell[1]] is None:
                    matrix[cell[0], cell[1]] = distance + 1
                    q.put(cell)
        return matrix


    def _get_accessible_adjacent_cells(self, pos):
        directions = [np.array([1, 0]), np.array([-1, 0]), np.array([0, 1]), np.array([0, -1])]
        for direction in directions:
            new_pos = pos + direction
            cell_is_valid = self.is_cell_index_in_bounds(new_pos)
            cell_is_blocked = cell_is_valid and self.is_path_blocked(pos, direction)
            if cell_is_valid and not cell_is_blocked:
                yield new_pos


    def get_wall_position_bytes(self):
        wall_values = []
        for y in range(8):
            for x in range(8):
                wall_values.append(self.walls[x][y] != 0)
        return list(self._get_bytes(wall_values))


    def get_wall_orientation_bytes(self):
        wall_values = []
        for y in range(8):
            for x in range(8):
                value = self.walls[x][y]
                wall_values.append(value - 1 if value != 0 else 0)
        return list(self._get_bytes(wall_values))


    def set_walls_from_bytes(self, wall_position_bytes, wall_orientation_bytes):
        wall_positions = list(self._get_bools(wall_position_bytes))
        wall_orientations = list(self._get_bools(wall_orientation_bytes))
        i = 0
        for y in range(8):
            for x in range(8):
                self.walls[x][y] = wall_orientations[i] + 1 if wall_positions[i] != 0 else 0 
                i = i + 1


    @staticmethod
    def _get_bytes(values):
        done = False
        iterator = iter(values)
        while not done:
            byte = 0
            for _ in range(0, 8):
                try:
                    bit = next(iterator)
                except StopIteration:
                    bit = 0
                    done = True
                byte = (byte << 1) | bit
            yield byte


    @staticmethod
    def _get_bools(values):
        for b in values:
            for i in reversed(range(8)):
                yield (b >> i) & 1    


# # Test code
# board = BoardState()
# board.walls[0][0] = 1
# board.walls[1][2] = 2
# board.walls[7][7] = 1
# board.cells[2][2] = 1
# board.cells[7][5] = 1

# print("Initial Board")
# print(board)

# board = board.switch()
# print("Rotated Board")
# print(board)
# #print(board.is_wall_index_in_bounds(np.array([0, 0])))
# #print(board.is_path_blocked(np.array([0, 0]), np.array([1, 0])))
