import os
import sys
import time
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from core.vector2 import Vector2
from game import Game
from player import Player
from random_player import RandomPlayer
from shortest_path_player import ShortestPathPlayer

from kivy.app import App
from kivy.uix.button import Button
from kivy_board import KivyBoard
from kivy.clock import Clock
from kivy.config import Config
Config.set('input', 'mouse', 'mouse,multitouch_on_demand')
Config.set('graphics', 'width', '600')
Config.set('graphics', 'height', '600')

class QuoridorApp(App):
    
    def build(self):
        #self.player1 = RandomPlayer(0)
        #self.player2 = RandomPlayer(1)
        self.player1 = ShortestPathPlayer(0)
        self.player2 = ShortestPathPlayer(1)
        self.game = Game([self.player1, self.player2])

        self.title = 'Quoridor Bot'
        self.board = KivyBoard()
        self.board.set_game(self.game)
        self.interval = Clock.schedule_interval(self.callback, 0.1)
        return self.board

    def callback(self, dt):
        self.game.take_turn()
        self.board.update()
        #print(self.game.board)
        if self.game.winner is not None:
            self.game.reset()
            #Clock.unschedule(self.interval)


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
    QuoridorApp().run()