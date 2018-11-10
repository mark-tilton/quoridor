import numpy as np

class BoardState:
    
    def __init__(self):
        self.walls = np.zeros((8, 8))
        print(self.walls)