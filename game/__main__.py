import os, sys
import numpy as np
import time as time
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from player import Player
from random_player import RandomPlayer
import validation as validation
from core.board_state import BoardState
from core.move import Move
from core.action import ActionType

if __name__ == "__main__":
    # create 2 players
    player1 = RandomPlayer()
    player2 = RandomPlayer()
    players = [player1, player2]
    moves = []
    board = BoardState()
    board.cells[player2.pos[0]][player2.pos[1]] = 1
    board = board.switch()
    board.cells[player1.pos[0]][player1.pos[1]] = 1

    player_index = 0
    game_over = False
    while not game_over:
        if player_index == 0:
            print(board)
        else:
            board = board.switch()
            print(board)
            board = board.switch()

        player = players[player_index]
        is_move_valid = False
        while not is_move_valid:
            action = player.take_action(board)
            move = Move(board, player.pos, action)
            is_move_valid = validation.validate_move(player, move)
        moves.append(move)
        board = move.get_board_state()
        if move.action.type == ActionType.MOVE:
            player.pos = move.action.new_pos
        else
            player.wall_count -= 1

        if player.pos[1] == 0:
            print('Player ' + str(player_index) + ' Wins!!')
            game_over = True
            # flip the board back
            if player_index == 2:
                board = board.switch()
            break;

        # Switch players
        board = board.switch()
        player_index = (player_index + 1) % 2
        time.sleep(0.1)
    print(board)
    