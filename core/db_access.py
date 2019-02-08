import os
import mysql.connector


class DbAccess:

    def open(self):
        self.connection = mysql.connector.connect(host='96.126.111.73',
                                                  user='quoridor',
                                                  password=str(os.environ.get('quoridor_bot_db_pw')),
                                                  database='quoridor_bot')

    def close(self):
        self.connection.close()

    def insert_game(self):
        cursor = self.connection.cursor()
        args = (1, # player_1_id
                2, # player_2_id
                2, # winner_id
                0, # OUT: game_id
        )
        result_args = cursor.callproc('insert_game', args)
        self.connection.commit()
        cursor.close()
        return result_args[3]

    def upload_moves(self, moves, winner):
        cursor = self.connection.cursor()
        for move in moves:
            board_state = move.board_state
            walls_1d = []
            for y in range(8):
                for x in range(8):
                    walls_1d.append(board_state.walls[y][x])
            player_positions = None

            args = (walls_1d,
                    player_positions,
                    move.cur_pos_x,
                    move.cur_pos_y,
                    move.move_x,
                    move.move_y,
                    move.block_x,
                    move.block_y,
                    move.block_orientation,
                    1 if winner else 0,
                    0 if winner else 1)
            cursor.callproc('move_upsert', args)
        cursor.close()


da = DbAccess()
da.open()
moves = []
game_id = da.insert_game()
print(game_id)
da.close()
