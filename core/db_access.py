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


    def upload_moves(self, moves, winner):
        cursor = self.connection.cursor()
        for move in moves:
            board_state = move.board_state

            wall_positions_bytes = board_state.get_wall_position_bytes()
            wall_orientation_bytes = board_state.get_wall_orientation_bytes()
            player_positions = None

            args = (wall_positions_bytes, 
                wall_orientation_bytes,
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


da = DataAccess()
da.open()
moves = []
da.upload_moves(moves, False)
da.close()
