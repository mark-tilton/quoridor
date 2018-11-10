import numpy as np

class BoardState:
    
    def __init__(self, players, walls = np.zeros((8, 8))):
        self.players = players
        self.walls = walls
        print(self.walls)

    def rotate(self):
        return BoardState(self.players, np.rot90(self.walls, 2))