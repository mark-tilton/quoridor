import numpy as np
import queue
from core.vector2 import Vector2

directions = [Vector2(1, 0), Vector2(-1, 0), Vector2(0, 1), Vector2(0, -1)]


class BoardState:

    def __init__(self, cells=np.zeros((9, 9), np.bool), walls=np.zeros((8, 8), np.int8)):
        self.cells = cells
        self.walls = walls
        self.distance_matrix = None

    def __repr__(self):
        output = ''
        for y in range(19):  # 9 cells + 8 inner walls + 2 outer walls
            for x in range(19):
                cell_char = ' '
                is_vert_wall = x % 2 == 0
                is_hori_wall = y % 2 == 0
                if y == 0 or y == 18:
                    cell_char = '―'
                elif x == 0 or x == 18:
                    cell_char = '|'
                elif not is_vert_wall and not is_hori_wall:  # is this a cell?
                    cell_x = int((x - 1) / 2)
                    cell_y = int((y - 1) / 2)
                    if self.cells[cell_x][cell_y] == 1:
                        cell_char = 'P'
                    elif self.distance_matrix is not None:
                        val = self.distance_matrix[cell_x][cell_y]
                        cell_char = ' ' if val is None else str(val % 10)
                elif is_vert_wall and is_hori_wall:  # Corners
                    cell_char = '·'
                elif is_vert_wall:
                    wall_pos_x = (x - 2) / 2
                    wall_pos_y = (y - 1) / 2
                    if self.is_path_blocked(Vector2(wall_pos_x, wall_pos_y), Vector2(1, 0)):
                        cell_char = '|'
                elif is_hori_wall:
                    wall_pos_x = (x - 1) / 2
                    wall_pos_y = (y - 2) / 2
                    if self.is_path_blocked(Vector2(wall_pos_x, wall_pos_y), Vector2(0, 1)):
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
        orientation = 1 if dir.y == 0 else 2
        points = BoardState.get_wall_points(cell, dir)
        for point in points:
            if self.is_wall_index_in_bounds(point) and self.walls[point.x, point.y] == orientation:
                return True
        return False

    @staticmethod
    def get_wall_points(cell, dir):
        center = cell - Vector2(0.5, 0.5)
        cdir = dir / 2
        perp = Vector2(cdir.y, cdir.x)
        wall = center + cdir
        point1 = (wall + perp).as_int()
        point2 = (wall - perp).as_int()
        return (point1, point2)

    def is_wall_index_in_bounds(self, i):
        return i.x >= 0 and i.y >= 0 and i.x < 8 and i.y < 8

    def is_cell_index_in_bounds(self, i):
        return i.x >= 0 and i.y >= 0 and i.x < 9 and i.y < 9

    def get_distance_matrix(self, pos):
        matrix = np.full((9, 9), None)
        matrix[pos.x, pos.y] = 0
        q = queue.Queue()
        q.put(pos)
        while not q.empty():
            cell_pos = q.get()
            distance = matrix[cell_pos.x, cell_pos.y]
            for cell in self._get_accessible_adjacent_cells(cell_pos):
                if matrix[cell.x, cell.y] is None:
                    matrix[cell.x, cell.y] = distance + 1
                    q.put(cell)
        return matrix

    def get_distance_matrix_from_row(self, row):
        matrix = np.full((9, 9), None)
        q = queue.Queue()
        for x in range(9):
            matrix[x, row] = 0
            q.put(Vector2(x, row))
        while not q.empty():
            cell_pos = q.get()
            distance = matrix[cell_pos.x, cell_pos.y]
            for cell in self._get_accessible_adjacent_cells(cell_pos):
                if matrix[cell.x, cell.y] is None:
                    matrix[cell.x, cell.y] = distance + 1
                    q.put(cell)
        return matrix

    def _get_accessible_adjacent_cells(self, pos):
        for direction in directions:
            new_pos = pos + direction
            if not self.is_cell_index_in_bounds(new_pos):
                continue
            if not self.is_path_blocked(pos, direction):
                yield new_pos
