import time
import validation as validation
from core.board_state import BoardState
from core.move import Move
from core.action import ActionType


class Game:

    def __init__(self, players):
        self.players = players
        self.player_index = 0
        self.board = BoardState()
        self.reset()

    def play(self, print_boards=False):
        while self.winner is None:
            self.take_turn()

    def take_turn(self):
        if self.winner is not None:
            return
        player = self.players[self.player_index]
        opponent = self.players[player.opp_index]
        action = player.take_action(opponent, self.board)
        move = Move(self.board, player.index, action)
        is_move_valid = validation.validate_move(player, opponent, move)
        if not is_move_valid:
            self.winner = player.opp_index
            return
        self.moves.append(move)
        self.board = move.get_board_state()
        if move.action.type == ActionType.MOVE:
            player.pos = move.action.new_pos
        else:
            player.wall_count -= 1

        if player.pos.y == player.goal_row:
            self.winner = self.player_index + 1

        # Switch players
        self.player_index = opponent.index
    
    def reset(self):
        self.moves = []
        self.player_index = 0
        self.winner = None
        player1 = self.players[0]
        player2 = self.players[1]
        player1.reset()
        player2.reset()
        self.board = BoardState()
        self.board.player_positions = []
        self.board.player_positions.append(player1.pos)
        self.board.player_positions.append(player2.pos)
        self.board.player_walls = [10, 10]
