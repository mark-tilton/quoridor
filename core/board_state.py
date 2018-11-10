import numpy as np

class BoardState:
    
    def __init__(self, players, walls = np.zeros((8, 8), np.int8)):
        self.players = players
        self.walls = walls
        print(self.walls)


    def rotate(self):
        return BoardState(self.players, np.rot90(self.walls, 2))


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


# Test code
print('Initial State')
board = BoardState()
board.walls[0][0] = 1
board.walls[1][2] = 2
board.walls[7][7] = 1
wall_position_bytes = board.get_wall_position_bytes()
wall_orientation_bytes = board.get_wall_orientation_bytes()

print(board.walls)
print()

print('Cleared')
board.walls[0][0] = 0
board.walls[1][2] = 0
board.walls[7][7] = 0
print(board.walls)
print()

print('Loaded From Bytes')
board.set_walls_from_bytes(wall_position_bytes, wall_orientation_bytes)
print(board.walls)
