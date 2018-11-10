import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from player import Player
from random_player import RandomPlayer
import validation as validation
from core.board_state import BoardState
from core.move import Move

if __name__ == "__main__":
    # create 2 players
    player1 = RandomPlayer()
    player2 = RandomPlayer()
    players = [player1, player2]
    moves = []
    board_state = BoardState(players)

    player_index = 0
    game_over = False
    while not game_over:
        player = players[player_index]
        is_move_valid = True
        while is_move_valid:
            action = player.take_action(board_state)
            move = Move(board_state, action)
            is_move_valid = validation.validate_move(move)
        moves.append(move)
        board_state = board_state.rotate()

    