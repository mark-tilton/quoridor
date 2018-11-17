import time
import numpy as np
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
        winner = None
        game_over = False
        players = self.players
        while not game_over:
            if print_boards:
                if self.player_index == 0:
                    print(self.board)
                else:
                    self.board = self.board.switch()
                    print(self.board)
                    self.board = self.board.switch()
            player = players[self.player_index]
            opponent = players[(self.player_index + 1) % 2]
            is_move_valid = False
            while not is_move_valid:
                action = player.take_action(opponent, self.board)
                move = Move(self.board, player.pos, action)
                is_move_valid = validation.validate_move(player, opponent, move)
            self.moves.append(move)
            self.board = move.get_board_state()
            if move.action.type == ActionType.MOVE:
                player.pos = move.action.new_pos
            else:
                player.wall_count -= 1

            if player.pos[1] == 0:
                winner = self.player_index + 1
                if print_boards:
                    print('Player ' + str(self.player_index + 1) + ' Wins!!')
                game_over = True
                # flip the board back
                if self.player_index == 1:
                    self.board = self.board.switch()
                break

            # Switch players
            self.board = self.board.switch()
            self.player_index = (self.player_index + 1) % 2
            if print_boards:
                time.sleep(0.1)
        if print_boards:
            print(self.board)
        return (winner, len(self.moves))

    def reset(self):
        self.moves = []
        self.player_index = 0
        player1 = self.players[0]
        player2 = self.players[1]
        player1.reset()
        player2.reset()
        self.board = BoardState()
        self.board.cells[player2.pos[0]][player2.pos[1]] = 1
        self.board = self.board.switch()
        self.board.cells[player1.pos[0]][player1.pos[1]] = 1
