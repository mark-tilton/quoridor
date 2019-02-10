import os
import MySQLdb
from core.action import ActionType


class DbAccess:

    def open(self):
        self.connection = MySQLdb.connect(host='96.126.111.73',
                                          user='quoridor',
                                          passwd=str(os.environ.get('quoridor_bot_db_pw')),
                                          db='quoridor_bot')

    def close(self):
        self.connection.close()

    def upload_game(self, game):
        cursor = self.connection.cursor()

        args = (game.players[0].id, # player_1_id
                game.players[1].id, # player_2_id
                game.winner, # winner_id
                0, # OUT: game_id
        )
        result_args = cursor.callproc('game_insert', args)
        game_id = result_args[3]
        cursor.close()

        for index, move in enumerate(game.moves):
            cursor = self.connection.cursor()
            board_state = move.board_state
            walls_1d = ''
            for y in range(8):
                for x in range(8):
                    walls_1d += str(board_state.walls[y][x])

            args = (game_id,
                    index + 1,
                    move.player_index,
                    walls_1d,
                    move.board_state.player_positions[0].x,
                    move.board_state.player_positions[0].y,
                    move.board_state.player_positions[1].x,
                    move.board_state.player_positions[1].y,
                    move.action.new_pos.x if move.action.type == ActionType.MOVE else None,
                    move.action.new_pos.x if move.action.type == ActionType.MOVE else None,
                    move.action.block_pos.x if move.action.type == ActionType.BLOCK else None,
                    move.action.block_pos.y if move.action.type == ActionType.BLOCK else None,
                    move.action.block_orientation - 1 if move.action.type == ActionType.BLOCK else None)
            cursor.callproc('move_insert', args)
            cursor.close()
        self.connection.commit()
