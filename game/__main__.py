import os
import sys
import time
import numpy as np
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from game import Game
from player import Player
from random_player import RandomPlayer
from shortest_path_player import ShortestPathPlayer
from core.action import ActionType
from training import train as train


def print_progress(iteration, total, start_time):
    percent = iteration / float(total)
    percent_text = ('{0:.2f}').format(100 * percent)
    if percent != 0:
        time_elapsed = time.time() - start_time
        est_time_remaining = time_elapsed / percent - time_elapsed
        minutes = int(est_time_remaining / 60)
        seconds = int(est_time_remaining % 60)
        time_remaining_text = ''
        if minutes > 0:
            time_remaining_text += '{0}m '.format(minutes)
        time_remaining_text += '{0}s'.format(seconds)
    else:
        time_remaining_text = 'Unknown'
    print('\r' + ' ' * 50, end='\r')
    print('\rProgress: %s%% | Est. Time Remaining: %s ' % (percent_text, time_remaining_text), end='')
    if iteration == total:
        print('\r' + ' ' * 50, end='\r')


if __name__ == "__main__":
    # create 2 players
    player1 = ShortestPathPlayer()
    player2 = ShortestPathPlayer()
    
    game = Game([player1, player2])

    game_count = 2
    player_1_win_count = 0
    player_2_win_count = 0
    total_move_count = 0
    all_winning_moves = []

    start_time = time.time()
    print_progress(0, game_count, start_time)
    for i in range(game_count):
        game.reset()
        winner, winning_moves, losing_moves = game.play(False)
        if winner == 1:
            player_1_win_count += 1
        else:
            player_2_win_count += 1
        total_move_count += len(winning_moves) + len(losing_moves)
        for move in winning_moves:
            all_winning_moves.append(move)
        print_progress(i + 1, game_count, start_time)
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
    
    x_train = []
    y_train = []
    for move in all_winning_moves:
        board_state = move.board_state
        action = move.action

        # Inputs
        distance_matrix_top = board_state.get_distance_matrix_from_row(0)
        distance_matrix_bottom = board_state.get_distance_matrix_from_row(8)
        cells = board_state.cells
        walls = board_state.walls
        pos = move.pos
        inputs = []
        for i in distance_matrix_top.flatten():
            inputs.append(i)
        for i in distance_matrix_bottom.flatten():
            inputs.append(i)
        for i in cells.flatten():
            inputs.append(i)
        for i in walls.flatten():
            inputs.append(i)
        for i in pos.flatten():
            inputs.append(i)
        x_train.append(np.asarray(inputs))

        # Outputs
        action_type = action.type
        if action_type == ActionType.MOVE:
            new_pos = action.new_pos
            out_walls = np.zeros((8, 8))
        else:
            block_pos = action.block_pos
            new_pos = np.array([0, 0])
            out_walls = np.zeros((8, 8))
            out_walls[block_pos[0], block_pos[1]] = action.block_orientation
        outputs = []
        outputs.append(0 if action_type == ActionType.MOVE else 1)
        for i in out_walls.flatten():
            outputs.append(i)
        for i in new_pos.flatten():
            outputs.append(i)
        y_train.append(np.asarray(outputs))
    train.train(np.asarray((1, x_train)), np.asarray((1, y_train)))
