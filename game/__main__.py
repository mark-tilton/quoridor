import os, sys
import time
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from game import Game
from player import Player
from random_player import RandomPlayer
from shortest_path_player import ShortestPathPlayer

if __name__ == "__main__":
    # create 2 players
    player1 = ShortestPathPlayer()
    player2 = ShortestPathPlayer()
    
    game = Game([player1, player2])

    game_count = 5
    player_1_win_count = 0
    player_2_win_count = 0
    total_move_count = 0

    start_time = time.time()
    for i in range(game_count):
        game.reset()
        winner, move_count = game.play(False)
        if winner == 1:
            player_1_win_count += 1
        else:
            player_2_win_count += 1
        total_move_count += move_count
    elapsed_time = (time.time() - start_time) * 1000

    print('================================================')
    print('Player 1 Wins            : ' + str(player_1_win_count))
    print('Player 2 Wins            : ' + str(player_2_win_count))
    print('Total Games              : ' + str(game_count))
    print('Elapsed Time             : ' + ('%.2f' % elapsed_time) + ' ms')
    print('Avg. Time per Move       : ' + ('%.2f' % (elapsed_time / total_move_count)) + ' ms')
    print('Avg. Time per Game       : ' + ('%.2f' % (elapsed_time / game_count)) + ' ms')
    print('Avg. Move Count per Game : ' + ('%.2f' % (total_move_count / game_count)))
    print('================================================')
    