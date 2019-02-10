import numpy as np
import queue
from core.vector2 import Vector2
from core.action import ActionType

directions = [Vector2(1, 0), Vector2(-1, 0), Vector2(0, 1), Vector2(0, -1)]


class BoardState:

    def __init__(self, player_positions=[], player_walls=[], walls=np.zeros((8, 8), np.int8)):
        self.player_positions = player_positions
        self.player_walls = player_walls
        self.walls = walls
        self.distance_matrix = None
        self.debug_matrix = None

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
                    if self.is_cell_occupied(Vector2(cell_x, cell_y)):
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
        return BoardState(list(self.player_positions), list(self.player_walls), np.copy(self.walls))

    def apply_action(self, action, player_index):
        new_board = self.clone()
        if action.type == ActionType.MOVE:
            new_board.player_positions[player_index] = action.new_pos
        elif action.type == ActionType.BLOCK:
            new_board.walls[action.block_pos.x][action.block_pos.y] = action.block_orientation
            new_board.player_walls[player_index] -= 1
        return new_board

    def switch(self):
        return BoardState(
            [Vector2(8, 8) - self.player_positions[0], Vector2(8, 8) - self.player_positions[1]],
            np.rot90(self.walls, 2))

    def is_cell_occupied(self, pos):
        return any(x == pos for x in self.player_positions)

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

    # For a given wall placement get which paths are blocked.
    @staticmethod
    def get_blocked_paths(wall, orientation):
        middle_offset = Vector2(0.5, 0.5)
        c_wall = wall + middle_offset
        direction = Vector2(0, 1) if orientation == 1 else Vector2(1, 0)
        c_dir = direction / 2
        perp = Vector2(c_dir.y, c_dir.x)
        return ((c_wall - perp - c_dir, c_wall - perp + c_dir), (c_wall + perp - c_dir, c_wall + perp + c_dir))

    def is_wall_index_in_bounds(self, i):
        return i.x >= 0 and i.y >= 0 and i.x < 8 and i.y < 8

    def is_cell_index_in_bounds(self, i):
        return i.x >= 0 and i.y >= 0 and i.x < 9 and i.y < 9

    def is_valid_wall(self, pos, orientation):
        if self.walls[pos.x, pos.y] != 0:
            return False
        shift_amount = Vector2(1, 0) if orientation == 1 else Vector2(0, 1)
        point_a = pos + shift_amount
        point_b = pos - shift_amount
        if self.is_wall_index_in_bounds(point_a) and self.walls[point_a.x, point_a.y] == orientation:
            return False
        if self.is_wall_index_in_bounds(point_b) and self.walls[point_b.x, point_b.y] == orientation:
            return False
        return True

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

    def get_deviation_matrix(self, dist, start):
        waves = queue.Queue()
        first_wave = queue.Queue()
        waves.put(first_wave)

        deviation_matrix = np.full((9, 9), None)
        deviation_matrix[start.x, start.y] = 0
        valid_moves = self.get_valid_moves(start, start)
        min_distance = -1
        for move in valid_moves:
            first_wave.put(move)
            move_distance = dist[move.x, move.y]
            if min_distance == -1 or move_distance < min_distance:
                min_distance = move_distance

        wave_count = 0
        while not waves.empty() and wave_count < 7:
            wave_count += 1
            next_wave = queue.Queue()
            current_wave = waves.get()
            while not current_wave.empty():
                cell = current_wave.get()
                distance = dist[cell.x, cell.y]
                deviation_matrix[cell.x, cell.y] = distance - min_distance
                for direction in directions:
                    new_position = cell + direction
                    if (self.is_cell_index_in_bounds(new_position)
                        and deviation_matrix[new_position.x, new_position.y] == None
                        and not self.is_path_blocked(cell, direction)):
                        next_wave.put(cell + direction)
            min_distance -= 1
            if not next_wave.empty():
                waves.put(next_wave)
        return deviation_matrix

    def get_valid_moves(self, from_pos, opp_pos):
        for pos in self._get_accessible_adjacent_cells(from_pos):
            if pos == opp_pos:
                for move in self.get_valid_moves(opp_pos, opp_pos):
                    yield move
            else:
                yield pos

    def _get_accessible_adjacent_cells(self, pos):
        for direction in directions:
            new_pos = pos + direction
            if not self.is_cell_index_in_bounds(new_pos):
                continue
            if not self.is_path_blocked(pos, direction):
                yield new_pos
