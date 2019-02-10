from kivy.uix.widget import Widget
from kivy.graphics import Rectangle, Line, Ellipse, Color
from core.vector2 import Vector2


class KivyBoard(Widget):

    def __init__(self, **kwargs):
        super(KivyBoard, self).__init__(**kwargs)
        self.game = None

    def set_game(self, game):
        self.game = game

    def update(self):        
        wall_thickness = 1
        board_size = min(self.width, self.height)
        origin = Vector2((self.width - board_size) / 2, (self.height - board_size) / 2)
        cell_width = (board_size - (wall_thickness * 10)) / 9.0
        cell_height = (board_size - (wall_thickness * 10)) / 9.0
        self.canvas.clear()
        with self.canvas:
            # Draw background
            Color(.85, .85, .85, 1)
            Rectangle(pos=(origin.x, origin.y), size=(board_size, board_size))      

            # Draw grid
            Color(.5, .5, .5, 1)
            for x in range(1, 9):
                x_coord = origin.x + x * cell_width + x * wall_thickness
                Line(points=[x_coord, origin.y, x_coord, origin.y + board_size],
                    width=wall_thickness, cap='square', joint='miter')
            for y in range(1, 9):
                y_coord = origin.y + y * cell_height + y * wall_thickness
                Line(points=[origin.x, y_coord, origin.x + board_size, y_coord],
                    width=wall_thickness, cap='square', joint='miter')

            if self.game is None:
                return

            # Draw player 1
            Color(1, 0, 0, 1)
            player1 = self.game.players[0]
            p1_pos = Vector2(origin.x + player1.pos.x * cell_width + (player1.pos.x + 1) * wall_thickness, 
                origin.y + (8 - player1.pos.y) * cell_height + (8 - player1.pos.y + 1) * wall_thickness)
            Ellipse(pos=(p1_pos.x + cell_width / 4, p1_pos.y + cell_height / 4), size=(cell_width / 2, cell_height / 2))            

            # Draw player 2
            Color(0, 0, 1, 1)
            player2 = self.game.players[1]
            p2_pos = Vector2(origin.x + player2.pos.x * cell_width + (player2.pos.x + 1) * wall_thickness, 
                origin.y + (8 - player2.pos.y) * cell_height + (8 - player2.pos.y + 1) * wall_thickness)
            Ellipse(pos=(p2_pos.x + cell_width / 4, p2_pos.y + cell_height / 4), size=(cell_width / 2, cell_height / 2))

            # Draw debug information
            Color(0, 0, 0, 1)
            if self.game.board.debug_matrix:
                for y in range(9):
                    for x in range(9):
                        center = Vector2(origin.x + (x + 1) * cell_width + (x + 1) * wall_thickness,
                                         origin.y + (8 - y) * cell_height + (8 - y) * wall_thickness)
                        Label(text=self.board.debug_matrix[y][x], pos=(center.x, center.y))

            # Draw walls
            Color(0, 0, 0, 1)
            for y in range(8):
                for x in range(8):
                    orientation = self.game.board.walls[x][y]
                    wall_scale_factor = 1
                    if orientation != 0:
                        center = Vector2(origin.x + (x + 1) * cell_width + (x + 1) * wall_thickness,
                                         origin.y + (8 - y) * cell_height + (8 - y) * wall_thickness)
                        Ellipse(pos=(center.x-4, center.y-4), size=(8, 8))
                        if orientation == 1:
                            p1 = Vector2(center.x, center.y - cell_height * wall_scale_factor)
                            p2 = Vector2(center.x, center.y + cell_height * wall_scale_factor)
                            Line(points=[p1.x, p1.y, p2.x, p2.y], width=wall_thickness*2, cap='square', joint='miter')
                        else:
                            p1 = Vector2(center.x - cell_width * wall_scale_factor, center.y)
                            p2 = Vector2(center.x + cell_width * wall_scale_factor, center.y)
                            Line(points=[p1.x, p1.y, p2.x, p2.y], width=wall_thickness*2, cap='square', joint='miter')
