from kivy.uix.widget import Widget
from kivy.graphics import Rectangle, Line, Ellipse, Color
from core.vector2 import Vector2


class KivyBoard(Widget):

    def __init__(self, **kwargs):
        super(KivyBoard, self).__init__(**kwargs)
        self.game = None

    def update(self, game):        
        wall_thickness = 1
        cell_width = (self.width - (wall_thickness * 10)) / 9.0
        cell_height = (self.height - (wall_thickness * 10)) / 9.0
        with self.canvas:
            # Draw background
            Color(.85, .85, .85, 1)
            Rectangle(pos=(0, 0), size=(self.width, self.height))      

            # Draw grid
            Color(.5, .5, .5, 1)
            for x in range(1, 9):
                x_coord = x * cell_width + x * wall_thickness
                Line(points=[x_coord, 0, x_coord, self.height], width=wall_thickness, cap='square', joint='miter')
            for y in range(1, 9):
                y_coord = y * cell_height + y * wall_thickness
                Line(points=[0, y_coord, self.width, y_coord], width=wall_thickness, cap='square', joint='miter')

            # Draw player 1
            Color(1, 0, 0, 1)
            player1 = game.players[0]
            p1_pos = Vector2(player1.pos.x * cell_width + (player1.pos.x + 1) * wall_thickness, 
                (8 - player1.pos.y) * cell_height + (8 - player1.pos.y + 1) * wall_thickness)
            Ellipse(pos=(p1_pos.x + cell_width / 4, p1_pos.y + cell_height / 4), size=(cell_width / 2, cell_height / 2))

            # Draw player 2
            Color(0, 0, 1, 1)
            player2 = game.players[1]
            p2_pos = Vector2(player2.pos.x * cell_width + (player2.pos.x + 1) * wall_thickness, 
                (8 - player2.pos.y) * cell_height + (8 - player2.pos.y + 1) * wall_thickness)
            Ellipse(pos=(p2_pos.x + cell_width / 4, p2_pos.y + cell_height / 4), size=(cell_width / 2, cell_height / 2))

            # Draw walls
            Color(0, 0, 0, 1)
            for y in range(8):
                for x in range(8):
                    orientation = game.board.walls[x][y]
                    if orientation != 0:
                        center = Vector2((x + 1) * cell_width + (x + 1) * wall_thickness,
                                         (8 - y) * cell_height + (8 - y) * wall_thickness)
                        if orientation == 1:
                            Line(points=[center.x, center.y - cell_height, center.x, center.y + cell_height], width=wall_thickness*2, cap='square', joint='miter')
                        else:
                            Line(points=[center.x - cell_width, center.y, center.x + cell_width, center.y], width=wall_thickness*2, cap='square', joint='miter')
