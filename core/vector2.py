
class Vector2:

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def as_int(self):
        return Vector2(int(self.x), int(self.y))

    def __add__(self, other):
        return Vector2(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector2(self.x - other.x, self.y - other.y)

    def __truediv__(self, other):
        return Vector2(self.x / other, self.y / other)
